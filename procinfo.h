#ifndef PROCINFO_H
#define PROCINFO_H
#include <qstring.h>
#include "model.h"


class ProcInfo: public Model
{
protected:
    pid_t pid;
    pid_t ppid;
    QString state;
    QString cmd;
//    long long virtual_memory;
//    long long resident_memory;

public:
    ProcInfo();
    ProcInfo(QMap<QString, QString> procInfoMap);
    ProcInfo(QString name, pid_t pid, pid_t ppid, QString state, QString cmd);//, long long virtual_memory, long long resident_memory);
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

//    long long getVirtualMemory() {return virtual_memory;};
//    void setVirtualMemory(long long virtual_memory) {this->virtual_memory = virtual_memory;};
//    pid_t getPid() {return pid;};
//    void setPid(pid_t pid) {this->pid = pid;};
};

#endif // PROCINFO_H
