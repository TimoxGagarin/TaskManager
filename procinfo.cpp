#include "procinfo.h"
#include <qmap.h>
#include <qdebug.h>
#include <time.h>

#include <qfile.h>

#include <signal.h>
#include <sys/wait.h>
#include "cpuusage.h"

ProcInfo::ProcInfo()
{

}

ProcInfo::~ProcInfo(){
    name.clear();
    state.clear();
}

ProcInfo::ProcInfo(QString name, pid_t pid, pid_t ppid, QString state, QString cmd, InfoSize virtual_memory, InfoSize resident_memory, float cpu, QDateTime start_time, int priority) :
    Model(name), virtual_memory(virtual_memory), resident_memory(resident_memory){
    this->pid = pid;
    this->ppid = ppid;
    this->state = state;
    this->cmd = cmd;
    this->cpu = cpu;
    this->start_time = start_time;
    this->priority = priority;
}

ProcInfo::ProcInfo(QMap<QString, QString> procInfoMap){
    this->name = procInfoMap["Name"];
    this->pid = procInfoMap["Pid"].toInt();
    this->ppid = procInfoMap["PPid"].toInt();
    this->state = procInfoMap["State"];
    this->cmd = procInfoMap["cmd"];

    if(procInfoMap.contains("VmSize"))
        this->virtual_memory = InfoSize(procInfoMap["VmSize"]);
    else{
        InfoSize to_write = InfoSize();
        to_write.setError("Н/Д");
        this->virtual_memory = to_write;
    }
    if(procInfoMap.contains("VmRSS"))
        this->resident_memory = InfoSize(procInfoMap["VmRSS"]);
    else{
        InfoSize to_write = InfoSize();
        to_write.setError("Н/Д");
        this->resident_memory = to_write;
    }

//    this->cpu = CpuUsage(this->pid).getUptime();

    QFile uptimeFile("/proc/uptime");
    if (!uptimeFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Error: Cannot open /proc/uptime";
    }

    QTextStream uptimeStream(&uptimeFile);
    double systemUptime = uptimeStream.readLine().split(' ')[0].toDouble();
    uptimeFile.close();

    long long currentDateTime = QDateTime::currentSecsSinceEpoch();
    long long systemBootTime = currentDateTime - static_cast<long long>(systemUptime);

    this->start_time = QDateTime::fromSecsSinceEpoch(systemBootTime + procInfoMap["start_time"].toLongLong() / sysconf(_SC_CLK_TCK));
    this->priority = procInfoMap["priority"].toInt();
}

QString ProcInfo::toString(){
    return this->name + ", " +
           QString::number(this->pid) + ", " +
           QString::number(this->ppid) + ", " +
           this->state + ", " +
           this->cmd + ", " +
           this->virtual_memory.toString() + ", " +
           this->resident_memory.toString() + ", " +
           QString::number(this->cpu) + ", " +
           this->start_time.toString() + ", " +
           QString::number(this->priority);
}

int ProcInfo::stopProc(){
    return kill(this->pid, SIGSTOP);
}

int ProcInfo::continueProc(){
    return kill(this->pid, SIGCONT);
}

int ProcInfo::closeProc(){
    kill(this->pid, SIGTERM);
    int status;
    waitpid(this->pid, &status, 0);
    return status;
}

int ProcInfo::interruptProc(){
    return kill(this->pid, SIGINT);
}
