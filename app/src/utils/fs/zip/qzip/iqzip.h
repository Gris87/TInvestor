#pragma once



#include <QString>

#ifdef Q_OS_WINDOWS
#pragma warning(push)
#pragma warning(disable : 4996)
#endif

#include <quazip/quazip.h>

#ifdef Q_OS_WINDOWS
#pragma warning(pop)
#endif



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
