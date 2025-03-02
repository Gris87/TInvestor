#pragma once



#pragma warning(push)
#pragma warning(disable : 4996)
#include <quazip/quazip.h>
#pragma warning(pop)



class IQZip
{
public:
    IQZip()
    {
    }
    virtual ~IQZip() = default;

    IQZip(const IQZip& another)            = delete;
    IQZip& operator=(const IQZip& another) = delete;

    virtual bool open(QuaZip::Mode mode) = 0;
    virtual void close()                 = 0;
};
