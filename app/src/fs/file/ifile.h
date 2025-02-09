#pragma once



#include <QByteArray>
#include <QIODevice>



class IFile
{
public:
    IFile()
    {
    }
    virtual ~IFile() = default;

    IFile(const IFile& another)            = delete;
    IFile& operator=(const IFile& another) = delete;

    virtual bool       open(QIODevice::OpenMode flags)     = 0;
    virtual qint64     read(char* data, qint64 maxlen)     = 0;
    virtual QByteArray readAll()                           = 0;
    virtual qint64     write(const char* data, qint64 len) = 0;
    virtual qint64     write(const QByteArray& data)       = 0;
    virtual qint64     size()                              = 0;
    virtual void       close()                             = 0;
};
