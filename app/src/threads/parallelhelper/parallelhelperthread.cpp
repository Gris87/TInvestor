#include "src/threads/parallelhelper/parallelhelperthread.h"

#include <QReadWriteLock>



#ifndef TESTING_MODE
// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
static int            _cpuCount;
static QReadWriteLock _cpuCountRwMutex; // clazy:exclude=non-pod-global-static
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)



void setCpuCount(const QString& cpuUsage)
{
    const QWriteLocker lock(&_cpuCountRwMutex);

    if (cpuUsage == "MINIMUM")
    {
        _cpuCount = 1;
    }
    else if (cpuUsage == "NORMAL")
    {
        _cpuCount = qMax(QThread::idealThreadCount() / 2, 1);
    }
    else if (cpuUsage == "OPTIMAL")
    {
        _cpuCount = qMax(QThread::idealThreadCount() - 1, 1);
    }
    else
    {
        _cpuCount = QThread::idealThreadCount();
    }
}

int getCpuCount()
{
    const QReadLocker lock(&_cpuCountRwMutex);

    return _cpuCount;
}
#else
void setCpuCount(const QString& /*cpuUsage*/)
{
}

int getCpuCount()
{
    return 1;
}
#endif
