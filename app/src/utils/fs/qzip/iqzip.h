#pragma once



class IQZip
{
public:
    IQZip()
    {
    }
    virtual ~IQZip() = default;

    IQZip(const IQZip& another)            = delete;
    IQZip& operator=(const IQZip& another) = delete;
};
