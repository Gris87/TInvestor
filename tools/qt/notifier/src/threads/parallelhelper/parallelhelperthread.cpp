#include "src/threads/parallelhelper/parallelhelperthread.h"



#ifndef TESTING_MODE
int getCpuCount()
{
    return QThread::idealThreadCount();
}
#else
int getCpuCount()
{
    return 1;
}
#endif
