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

public:
    ProcInfo();
    ProcInfo(QMap<QString, QString> procInfoMap);
    ProcInfo(QString name, pid_t pid, pid_t ppid, QString state);
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
};

#endif // PROCINFO_H
