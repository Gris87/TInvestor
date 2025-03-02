#pragma once



#include <QString>

#pragma warning(push)
#pragma warning(disable : 4996)
#include <quazip/quazip.h>
#include <quazip/quazipfile.h>
#pragma warning(pop)



class IQZipFile
{
public:
    IQZipFile()
    {
    }
    virtual ~IQZipFile() = default;

    IQZipFile(const IQZipFile& another)            = delete;
    IQZipFile& operator=(const IQZipFile& another) = delete;

    virtual bool       open(QIODevice::OpenMode mode) = 0;
    virtual QByteArray readAll()                      = 0;
    virtual void       close()                        = 0;
};
