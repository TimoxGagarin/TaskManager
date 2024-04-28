#include "procinfo.h"
#include <qmap.h>
#include <qdebug.h>

#include <signal.h>
#include <sys/wait.h>

ProcInfo::ProcInfo()
{

}

ProcInfo::~ProcInfo(){
    name.clear();
    state.clear();
}

ProcInfo::ProcInfo(QString name, pid_t pid, pid_t ppid, QString state, QString cmd) : Model(name){
    this->pid = pid;
    this->ppid = ppid;
    this->state = state;
    this->cmd = cmd;
}

ProcInfo::ProcInfo(QMap<QString, QString> procInfoMap){
    this->name = procInfoMap["Name"];
    this->pid = procInfoMap["Pid"].toInt();
    this->ppid = procInfoMap["Ppid"].toInt();
    this->state = procInfoMap["State"];
    this->cmd = procInfoMap["cmd"];
}

QString ProcInfo::toString(){
    return this->name + ", " +
           QString::number(this->pid) + ", " +
           QString::number(this->ppid) + ", " +
           this->state + ", " +
           this->cmd;
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
