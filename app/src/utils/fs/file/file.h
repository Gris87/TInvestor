#pragma once



#include "src/utils/fs/file/ifile.h"

#include <QFile>



class File : public IFile
{
public:
    explicit File(const QString& name);
    ~File() override;

    File(const File& another)            = delete;
    File& operator=(const File& another) = delete;

    bool       open(QIODevice::OpenMode flags) override;
    qint64     read(char* data, qint64 maxlen) override;
    QByteArray readAll() override;
    qint64     write(const char* data, qint64 len) override;
    qint64     write(const QByteArray& data) override;
    qint64     size() override;
    void       close() override;
    bool       exists() override;
    bool       remove() override;

private:
    QFile mFile;
};
