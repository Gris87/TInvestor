#pragma once



#include <QString>

#ifdef Q_OS_WINDOWS
#pragma warning(push)
#pragma warning(disable : 4996)
#endif

#include <quazip/quazip.h>
#include <quazip/quazipfile.h>

#ifdef Q_OS_WINDOWS
#pragma warning(pop)
#endif



class IQZipFile
{
public:
    IQZipFile()          = default;
    virtual ~IQZipFile() = default;

    IQZipFile(const IQZipFile& another)            = delete;
    IQZipFile& operator=(const IQZipFile& another) = delete;

    virtual bool       open(QIODevice::OpenMode mode) = 0;
    virtual QByteArray readAll()                      = 0;
    virtual void       close()                        = 0;
};
