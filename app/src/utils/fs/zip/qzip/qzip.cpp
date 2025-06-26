#include "src/utils/fs/zip/qzip/qzip.h"

#include <QDebug>



QZip::QZip(QIODevice* device) :
    IQZip(),
    mQuaZip(device)
{
    qDebug() << "Create QZip";
}

QZip::~QZip()
{
    qDebug() << "Destroy QZip";
}

QuaZip* QZip::getZip()
{
    return &mQuaZip;
}

bool QZip::open(QuaZip::Mode mode)
{
    return mQuaZip.open(mode);
}

void QZip::close()
{
    mQuaZip.close();
}

QStringList QZip::getFileNameList()
{
    return mQuaZip.getFileNameList();
}

bool QZip::setCurrentFile(const QString& fileName)
{
    return mQuaZip.setCurrentFile(fileName);
}
