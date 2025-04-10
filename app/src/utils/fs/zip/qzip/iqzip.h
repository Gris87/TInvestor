#pragma once



#include <QString>

#pragma warning(push)
#pragma warning(disable : 4996)
#include <quazip/quazip.h>
#pragma warning(pop)



class IQZip
{
public:
    IQZip()          = default;
    virtual ~IQZip() = default;

    IQZip(const IQZip& another)            = delete;
    IQZip& operator=(const IQZip& another) = delete;

    virtual QuaZip* getZip() = 0;

    virtual bool open(QuaZip::Mode mode) = 0;
    virtual void close()                 = 0;

    virtual QStringList getFileNameList()                       = 0;
    virtual bool        setCurrentFile(const QString& fileName) = 0;
};
