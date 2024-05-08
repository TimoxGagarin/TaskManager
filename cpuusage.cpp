#include "cpuusage.h"
#include <qfile.h>
#include <qdebug.h>
#include <unistd.h>

CpuUsage::CpuUsage(int pid): pid(pid){}

QPair<double, double> CpuUsage::getIdleAndWorkTimes(){
    QString statFilePath = QString("/proc/%1/stat").arg(pid);
    QFile statFile(statFilePath);
    QPair<double, double> result = {0,0};

    if (!statFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open" << statFilePath;
        return result; // Возвращаем -1 в случае ошибки
    }

    QTextStream in(&statFile);
    QString statLine = in.readLine();
    statFile.close();

    QStringList statParts = statLine.split(' ');

    // Индекс, соответствующий полю utime (время CPU в user mode) и stime (время CPU в kernel mode) в /proc/<pid>/stat
    int utimeIndex = 13; // Нумерация с 1
    int stimeIndex = 14;
    int cutimeIndex = 15;
    int cstimeIndex = 16;

    // Получаем значение utime, stime, cutime и cstime
    long utime = statParts[utimeIndex].toLong();
    long stime = statParts[stimeIndex].toLong();
    long cutime = statParts[cutimeIndex].toLong();
    long cstime = statParts[cstimeIndex].toLong();
    return {(double)(stime + cutime + cstime), (double)utime};
}

double CpuUsage::getUptime(){
    double uptime;
    QFile uptimeFile("/proc/uptime");
    if (!uptimeFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open /proc/uptime";
        return -1.0; // Возвращаем -1 в случае ошибки
    }
    QTextStream uptimeStream(&uptimeFile);
    uptimeStream >> uptime;
    uptimeFile.close();
    return uptime;
}


