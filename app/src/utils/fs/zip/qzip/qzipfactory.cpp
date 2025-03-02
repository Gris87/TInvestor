#include "src/utils/fs/zip/qzip/qzipfactory.h"

#include <QDebug>

#include "src/utils/fs/zip/qzip/qzip.h"



QZipFactory::QZipFactory() :
    IQZipFactory()
{
    qDebug() << "Create QZipFactory";
}

QZipFactory::~QZipFactory()
{
    qDebug() << "Destroy QZipFactory";
}

std::shared_ptr<IQZip> QZipFactory::newInstance(const QString& name)
{
    return std::shared_ptr<IQZip>(new QZip(name));
}
