#pragma once



#include <QString>



class IDir
{
public:
    IDir()
    {
    }
    virtual ~IDir() = default;

    IDir(const IDir& another)            = delete;
    IDir& operator=(const IDir& another) = delete;

    virtual bool mkpath(const QString& dirPath) = 0;
};
