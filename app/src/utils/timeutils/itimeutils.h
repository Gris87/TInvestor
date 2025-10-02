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
    virtual bool isMorningSession(qint64 timestamp)                        = 0;
    virtual bool isTimeBetween(QTime time, QTime startTime, QTime endTime) = 0;
};
