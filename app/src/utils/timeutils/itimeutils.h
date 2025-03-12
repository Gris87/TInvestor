#pragma once



#include <QThread>



class ITimeUtils
{
public:
    ITimeUtils()
    {
    }
    virtual ~ITimeUtils() = default;

    ITimeUtils(const ITimeUtils& another)            = delete;
    ITimeUtils& operator=(const ITimeUtils& another) = delete;

    virtual bool interruptibleSleep(int ms, QThread* parentThread) = 0;
};
