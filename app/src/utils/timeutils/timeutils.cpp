#include "src/utils/timeutils/timeutils.h"

#include <QDebug>



constexpr int SLEEP_STEP_IN_MS            = 100;
constexpr int NORMAL_SESSION_START_HOUR   = 10;
constexpr int NORMAL_SESSION_START_MINUTE = 0;
constexpr int NORMAL_SESSION_END_HOUR     = 18;
constexpr int NORMAL_SESSION_END_MINUTE   = 40;



TimeUtils::TimeUtils() :
    ITimeUtils(),
    mMoscowTimezone("Europe/Moscow")
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

bool TimeUtils::isWorkingHours(qint64 timestamp)
{
    const QDateTime dateTime  = QDateTime::fromMSecsSinceEpoch(timestamp, mMoscowTimezone);
    const int       dayOfWeek = dateTime.date().dayOfWeek();

    if (dayOfWeek == Qt::Saturday || dayOfWeek == Qt::Sunday)
    {
        return false;
    }

    const QTime time      = dateTime.time();
    const QTime startTime = QTime(NORMAL_SESSION_START_HOUR, NORMAL_SESSION_START_MINUTE);
    const QTime endTime   = QTime(NORMAL_SESSION_END_HOUR, NORMAL_SESSION_END_MINUTE);

    return time >= startTime && time < endTime;
}
