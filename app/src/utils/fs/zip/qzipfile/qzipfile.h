#pragma once



#include "src/utils/fs/zip/qzipfile/iqzipfile.h"



class QZipFile : public IQZipFile
{
public:
    QZipFile(QuaZip* zip);
    ~QZipFile();

    QZipFile(const QZipFile& another)            = delete;
    QZipFile& operator=(const QZipFile& another) = delete;

    bool open(QIODevice::OpenMode mode) override;
    QByteArray readAll() override;
    void close() override;

private:
    QuaZipFile mQuaZipFile;
};
