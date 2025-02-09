#pragma once



class IDir
{
public:
    IDir()
    {
    }
    virtual ~IDir() = default;

    IDir(const IDir& another)            = delete;
    IDir& operator=(const IDir& another) = delete;
};
