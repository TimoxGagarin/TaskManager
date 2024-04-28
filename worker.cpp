#include <qdebug.h>
#include <qfile.h>
#include <QIODevice>
#include <QDir>
#include <QDirIterator>
#include <qlist.h>
#include <qstringlist.h>
#include <qthread.h>

#include <sys/statvfs.h>
#include <mntent.h>

#include "procinfo.h"
#include "filesystem.h"
#include "worker.h"

Worker::Worker()
{

}

QList<ProcInfo> Worker::get_processes() {
    QList<ProcInfo> ret;
    QDir dir("/proc");
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
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
        // qDebug() << processInfo;
        ret.append(ProcInfo(processInfo));
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

void Worker::timer_updateTables() {
    while(!stopFlag.load()){
        emit process_updated(get_processes());
        emit fileSystems_updated(get_fileSystems());
        QThread::sleep(5);
    }
}

void Worker::stop(){
    stopFlag.store(true);
}
