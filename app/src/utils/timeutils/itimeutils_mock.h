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
};
