#include "src/utils/fs/zip/qzipfile/qzipfilefactory.h"

#include <QDebug>

#include "src/utils/fs/zip/qzipfile/qzipfile.h"



QZipFileFactory::QZipFileFactory() :
    IQZipFileFactory()
{
    qDebug() << "Create QZipFileFactory";
}

QZipFileFactory::~QZipFileFactory()
{
    qDebug() << "Destroy QZipFileFactory";
}

std::shared_ptr<IQZipFile> QZipFileFactory::newInstance(QuaZip* zip)
{
    return std::shared_ptr<IQZipFile>(new QZipFile(zip));
}
