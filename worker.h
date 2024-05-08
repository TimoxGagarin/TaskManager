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
    typedef struct cpuStruct
    {
        long long unsigned idle, nonIdle;
    } cpuStruct;
    QList<QVector<double>> cpuPlotData;
    QVector<cpuStruct> prevCpuTimes;
    QAtomicInt stopFlag;
public:
    Worker();
    QList<ProcInfo> get_processes();
    QList<FileSystem> get_fileSystems();

    QVector<cpuStruct> getCpuTimes();
    QVector<double> calculateCpuPercentages(QVector<cpuStruct> now, QVector<cpuStruct> prev);
    void updateCpu();

public slots:
    void timer_updateTables();
    void timer_updatePlots();
    void stop();

signals:
    void process_updated(QList<ProcInfo> list);
    void fileSystems_updated(QList<FileSystem> list);
    void updateCpuPlotSIG(const QVector<QVector<double>> &values);
};

#endif // WORKER_H
