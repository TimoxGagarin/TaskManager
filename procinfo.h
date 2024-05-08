#ifndef PROCINFO_H
#define PROCINFO_H
#include <qstring.h>
#include <qdatetime.h>

#include "model.h"
#include "infosize.h"
#include "cpuusage.h"

class ProcInfo: public Model
{
protected:
    pid_t pid;
    pid_t ppid;
    QString state;
    QString cmd;
    InfoSize virtual_memory;
    InfoSize resident_memory;
    float cpu;
    QDateTime start_time;
    int priority;
public:
    ProcInfo();
    ProcInfo(QMap<QString, QString> procInfoMap);
    ProcInfo(QString name, pid_t pid, pid_t ppid, QString state, QString cmd, InfoSize virtual_memory, InfoSize resident_memory, float cpu, QDateTime start_time, int priority);
    ~ProcInfo();
    QString toString() override;

    int stopProc();
    int continueProc();
    int closeProc();
    int interruptProc();

    pid_t getPid() {return pid;};
    void setPid(pid_t pid) {this->pid = pid;};
    pid_t getPpid() {return ppid;};
    void setPpid(pid_t ppid) {this->ppid = ppid;};
    QString getState() {return state;};
    void setState(QString state) {this->state = state;};
    QString getCmd() {return cmd;};
    void setCmd(QString cmd) {this->cmd = cmd;};
    InfoSize getVirtualMemory() {return virtual_memory;};
    void setVirtualMemory(InfoSize virtual_memory) {this->virtual_memory = virtual_memory;};
    InfoSize getResidentMemory() {return resident_memory;};
    void setResidentMemory(InfoSize resident_memory) {this->resident_memory = resident_memory;};
    float getCPU() {return cpu;};
    void setCPU(float cpu) {this->cpu = cpu;};
    QDateTime getStartTime() {return start_time;};
    void setStartTime(QDateTime start_time) {this->start_time = start_time;};
    int getPriority() {return priority;};
    void setPriority(int priority) {this->priority = priority;};
};

#endif // PROCINFO_H
