#ifndef CPUUSAGE_H
#define CPUUSAGE_H
#include <QPair>


class CpuUsage
{
public:
    CpuUsage(int pid);
    double getUptime();
    QPair<double, double> getIdleAndWorkTimes();

private:
    int pid;
};

#endif // CPUUSAGE_H
