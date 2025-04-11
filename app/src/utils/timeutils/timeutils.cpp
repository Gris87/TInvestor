#include "src/utils/timeutils/timeutils.h"

#include <QDebug>



constexpr int SLEEP_STEP_IN_MS = 100;



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
    qDebug() << "Sleeping for" << ms << "ms";

    bool res = false;

    const int count = ms / SLEEP_STEP_IN_MS;

    for (int i = 0; i < count && !res; ++i)
    {
        QThread::msleep(SLEEP_STEP_IN_MS);

        res = parentThread->isInterruptionRequested();
    }

    return res;
}
