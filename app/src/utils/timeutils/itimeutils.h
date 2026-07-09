#pragma once



#include <QThread>
#include <QTime>



class ITimeUtils
{
public:
    ITimeUtils()          = default;
    virtual ~ITimeUtils() = default;

    ITimeUtils(const ITimeUtils& another)            = delete;
    ITimeUtils& operator=(const ITimeUtils& another) = delete;

    virtual bool interruptibleSleep(int ms, QThread* parentThread)         = 0;
    virtual bool isWorkingHours(qint64 timestamp)                          = 0;
    virtual bool isMonday(qint64 timestamp)                                = 0;
    virtual bool isWeekend(qint64 timestamp)                               = 0;
    virtual bool isNormalOrEveningSession(qint64 timestamp)                = 0;
    virtual bool isTimeBetween(QTime time, QTime startTime, QTime endTime) = 0;
    virtual bool isTimestampMore(qint64 timestamp, qint64 another)         = 0;
};
