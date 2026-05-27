#pragma once



#include "src/utils/timeutils/itimeutils.h"

#include <gmock/gmock.h>



class TimeUtilsMock : public ITimeUtils
{
public:
    TimeUtilsMock() :
        ITimeUtils()
    {
    }
    ~TimeUtilsMock() override = default;

    TimeUtilsMock(const TimeUtilsMock& another)            = delete;
    TimeUtilsMock& operator=(const TimeUtilsMock& another) = delete;

    MOCK_METHOD(bool, interruptibleSleep, (int ms, QThread* parentThread), (override));
    MOCK_METHOD(bool, isWorkingHours, (qint64 timestamp), (override));
    MOCK_METHOD(bool, isWeekend, (qint64 timestamp), (override));
    MOCK_METHOD(bool, isNormalOrEveningSession, (qint64 timestamp), (override));
    MOCK_METHOD(bool, isTimeBetween, (QTime time, QTime startTime, QTime endTime), (override));
};
