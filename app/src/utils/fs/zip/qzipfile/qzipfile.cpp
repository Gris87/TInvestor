#include "src/utils/fs/zip/qzipfile/qzipfile.h"

#include <QDebug>



QZipFile::QZipFile(QuaZip* zip) :
    IQZipFile(),
    mQuaZipFile(QuaZipFile(zip))
{
    qDebug() << "Create QZipFile";
}

QZipFile::~QZipFile()
{
    qDebug() << "Destroy QZipFile";
}

bool QZipFile::open(QIODevice::OpenMode mode)
{
    return mQuaZipFile.open(mode);
}

QByteArray QZipFile::readAll()
{
    return mQuaZipFile.readAll();
}

void QZipFile::close()
{
    mQuaZipFile.close();
}
