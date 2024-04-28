#ifndef WORKER_H
#define WORKER_H
#include <QObject>
#include <QList>
#include <unistd.h>

#include "procinfo.h"
#include "filesystem.h"

class Worker: public QObject
{
    Q_OBJECT
private:
    QAtomicInt stopFlag;
public:
    Worker();
    QList<ProcInfo> get_processes();
    QList<FileSystem> get_fileSystems();

public slots:
    void timer_updateTables();
    void stop();

signals:
    void process_updated(QList<ProcInfo> list);
    void fileSystems_updated(QList<FileSystem> list);
};

#endif // WORKER_H
