#include "src/utils/timeutils/timeutils.h"

#include <QDebug>



TimeUtils::TimeUtils() :
    ITimeUtils()
{
    qDebug() << "Create TimeUtils";
}

TimeUtils::~TimeUtils()
{
    qDebug() << "Destroy TimeUtils";
}

bool TimeUtils::interruptibleSleep(int ms, QThread* parentThread)
{
    bool res = false;

    int count = ms / 100;

    for (int i = 0; i < count && !res; ++i)
    {
        QThread::msleep(100);

        res = parentThread->isInterruptionRequested();
    }

    return res;
}
