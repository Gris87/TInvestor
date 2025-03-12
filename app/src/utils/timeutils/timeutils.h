#pragma once



#include "src/utils/timeutils/itimeutils.h"



class TimeUtils : public ITimeUtils
{
public:
    TimeUtils();
    ~TimeUtils();

    TimeUtils(const TimeUtils& another)            = delete;
    TimeUtils& operator=(const TimeUtils& another) = delete;

    bool interruptibleSleep(int ms, QThread* parentThread) override;
};
