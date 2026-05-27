#pragma once



#include "src/utils/timeutils/itimeutils.h"

#include <QTimeZone>



class TimeUtils : public ITimeUtils
{
public:
    explicit TimeUtils();
    ~TimeUtils() override;

    TimeUtils(const TimeUtils& another)            = delete;
    TimeUtils& operator=(const TimeUtils& another) = delete;

    bool interruptibleSleep(int ms, QThread* parentThread) override;
    bool isWorkingHours(qint64 timestamp) override;
    bool isWeekend(qint64 timestamp) override;
    bool isNormalOrEveningSession(qint64 timestamp) override;
    bool isTimeBetween(QTime time, QTime startTime, QTime endTime) override;

private:
    QTimeZone mMoscowTimezone;
};
