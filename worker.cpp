#include <qdebug.h>
#include <qfile.h>
#include <QIODevice>
#include <QDir>
#include <QDirIterator>
#include <qlist.h>
#include <qstringlist.h>
#include <qthread.h>
#include <QRegularExpression>

#include <sys/statvfs.h>
#include <mntent.h>

#include "procinfo.h"
#include "filesystem.h"
#include "worker.h"

Worker::Worker()
{

}


double getCpuUsageByPid(int pid) {
    QString path = QString("/proc/%1/stat").arg(pid);

    // Открываем файл и читаем данные о процессе
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file" << path;
        return -1;
    }

    QTextStream in(&file);
    QString line = in.readLine();
    file.close();

    QStringList fields = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    if (fields.size() < 22) {
        qWarning() << "Invalid format in" << path;
        return -1;
    }

    bool ok;
    long long utime = fields[13].toLongLong(&ok);
    if (!ok) return -1;
    long long stime = fields[14].toLongLong(&ok);
    if (!ok) return -1;
    long long totalTime = utime + stime;
    long long startTime = fields[21].toLongLong(&ok);
    if (!ok) return -1;

    // Чтение информации о времени работы системы
    QFile uptimeFile("/proc/uptime");
    if (!uptimeFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open /proc/uptime";
        return -1;
    }

    QTextStream uptimeStream(&uptimeFile);
    QString uptimeLine = uptimeStream.readLine();
    uptimeFile.close();

    QStringList uptimeFields = uptimeLine.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    if (uptimeFields.isEmpty()) {
        qWarning() << "Failed to read uptime";
        return -1;
    }

    double uptime = uptimeFields[0].toDouble(&ok);
    if (!ok) return -1;

    long long hertz = sysconf(_SC_CLK_TCK);

    double processTimeSeconds = static_cast<double>(totalTime) / hertz;
    double systemUptimeSeconds = uptime - (static_cast<double>(startTime) / hertz);

    double cpuUsage = 100.0 * (processTimeSeconds / systemUptimeSeconds);
    return cpuUsage;
}

QList<ProcInfo> Worker::get_processes() {
    QDir dir("/proc");
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    QList<ProcInfo> ret;
    QFileInfoList proc_list = dir.entryInfoList();

    for (QFileInfo subdir: proc_list) {
        bool ok;
        pid_t pid = subdir.baseName().toInt(&ok);
        if(!ok)
            continue;
        QMap<QString, QString> processInfo;

        // Основная информация о процессе
        QString proc_path = QString("/proc/%1/status").arg(pid);
        QFile file(proc_path);
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream stream(&file);
            while (true) {
                QString line = stream.readLine().replace("\t", " ").trimmed();
                if(line.isEmpty())
                    break;
                int index = line.indexOf(':');
                if (index != -1) {
                    QString key = line.left(index).trimmed();
                    QString value = line.mid(index + 1).trimmed();
                    processInfo.insert(key, value);
                }
            }
        }
        file.close();

        // Информация о командной строке
        QString cmd_path = QString("/proc/%1/cmdline").arg(pid);
        QFile cmdfile(cmd_path);
        if (cmdfile.open(QIODevice::ReadOnly)) {
            QTextStream stream(&cmdfile);
            QString line = stream.readLine();
            processInfo.insert("cmd", line);
        }
        cmdfile.close();

        // Время запуска, приоритет
        QString stat_path = QString("/proc/%1/stat").arg(pid);
        QFile statfile(stat_path);
        if (statfile.open(QIODevice::ReadOnly)) {
            QTextStream stream(&statfile);
            QStringList statFields = stream.readLine().split(' ');
            processInfo.insert("start_time", statFields[21]);
            processInfo.insert("priority", statFields[17]);
        }
        statfile.close();
        ProcInfo procInfo = ProcInfo(processInfo);
        procInfo.setCPU(getCpuUsageByPid(processInfo["Pid"].toInt()));
        ret.append(procInfo);
    }
    return ret;
}

QList<FileSystem> Worker::get_fileSystems(){
    struct mntent *mnt;
    FILE *mounts;
    struct statvfs vfs;
    QList<FileSystem> ret;

    mounts = setmntent("/proc/mounts", "r");
    if (!mounts) {
        qDebug() << "Не удалось открыть /proc/mounts\n";
        return ret;
    }

    while ((mnt = getmntent(mounts)) != NULL) {
        // Пропускаем виртуальные файловые системы
        if (strcmp(mnt->mnt_type, "tmpfs") == 0 || strcmp(mnt->mnt_type, "sysfs") == 0 ||
            strcmp(mnt->mnt_type, "proc") == 0 || strcmp(mnt->mnt_type, "devtmpfs") == 0 ||
            strcmp(mnt->mnt_type, "squashfs") == 0 || strncmp(mnt->mnt_fsname, "overlay", 7) == 0) {
            continue;
        }

        // Проверка, что устройство начинается с "/dev/"
        if (strncmp(mnt->mnt_fsname, "/dev/", 5) == 0) {
            if (statvfs(mnt->mnt_dir, &vfs) == 0) {
                unsigned long total = vfs.f_blocks * vfs.f_frsize;
                unsigned long freee = vfs.f_bfree * vfs.f_frsize;
                unsigned long available = vfs.f_bavail * vfs.f_frsize;
                unsigned long used = total - freee;

                ret.append(FileSystem(QString(mnt->mnt_fsname), QString(mnt->mnt_dir), QString(mnt->mnt_type),
                                      total, freee, available, used));
            }
        }
    }

    endmntent(mounts);
    return ret;
}

QVector<Worker::cpuStruct> Worker::getCpuTimes() {
    QVector<cpuStruct> times;

    QFile file("/proc/stat");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open stat file";
        return times;
    }

    QTextStream in(&file);
    in.readLine();
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(" ", QString::SkipEmptyParts);

        if (fields.size() < 10 || !fields[0].startsWith("cpu"))
            break;

        unsigned long long user = fields[1].toULongLong();
        unsigned long long nice = fields[2].toULongLong();
        unsigned long long system = fields[3].toULongLong();
        unsigned long long idle = fields[4].toULongLong();
        unsigned long long iowait = fields[5].toULongLong();
        unsigned long long irq = fields[6].toULongLong();
        unsigned long long softirq = fields[7].toULongLong();
        unsigned long long steal = fields[8].toULongLong();
        unsigned long long guest = fields[9].toULongLong();
        unsigned long long guestnice = fields[10].toULongLong();

        cpuStruct cpu = {idle + iowait, user + nice + system + irq + softirq + steal + guest + guestnice};
        times.append(cpu);
    }

    file.close();
    return times;
}

QVector<double> Worker::calculateCpuPercentages(QVector<Worker::cpuStruct> now, QVector<Worker::cpuStruct> prev)
{
    QVector<double> times;
    if (now.size() != prev.size())
        return times;

    for(size_t i=0; i < now.size(); i++) {
        cpuStruct n, p;
        n = now.at(i);
        p = prev.at(i);

        long long unsigned totald = ((n.idle + n.nonIdle) - (p.idle + p.nonIdle));
        times.append((totald - (n.idle - p.idle)) / (double)totald * 100.0);
    }
    return times;
}

void Worker::updateCpu()
{
    QVector<cpuStruct> cpuTimes = getCpuTimes();
    if (prevCpuTimes.size() != 0)
    {
        QVector<double> cpuPercentages = calculateCpuPercentages(cpuTimes, prevCpuTimes);
        if (cpuPlotData.size() == 60)
            cpuPlotData.pop_front();
        cpuPlotData.push_back(cpuPercentages);
    }
    prevCpuTimes = cpuTimes;

    QVector<QVector<double>> plottingData;

    if (cpuPlotData.isEmpty())
        return;

    for (int i = 0; i < cpuPlotData.at(0).size(); i++)
    {
        QVector<double> headingVector;
        headingVector.push_back(cpuPlotData.at(0).at(i));
        plottingData.push_back(headingVector);
    }

    for (int i = 1; i < cpuPlotData.size(); i++)
        for (int j = 0; j < cpuPlotData.at(i).size(); j++)
            plottingData[j].append(cpuPlotData.at(i).at(j));

    for (int i = 0; i < plottingData.size(); i++)
        for (int j = 60 - plottingData.at(i).size(); j > 0; j--)
            plottingData[i].prepend((double)0);

    emit updateCpuPlotSIG(plottingData);
}

void Worker::timer_updateTables() {
    while(!stopFlag.load()){
        emit process_updated(get_processes());
        emit fileSystems_updated(get_fileSystems());
        QThread::sleep(5);
    }
}

void Worker::timer_updatePlots() {
    while(!stopFlag.load()){
        updateCpu();
        QThread::sleep(1);
    }
}

void Worker::stop(){
    stopFlag.store(true);
}
