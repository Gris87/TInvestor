#include "src/utils/fs/qzip/qzip.h"

#include <QDebug>



QZip::QZip(const QString& name) :
    IQZip(),
    mQuaZip(QuaZip(name))
{
    qDebug() << "Create QZip";
}

QZip::~QZip()
{
    qDebug() << "Destroy QZip";
}

bool QZip::open(QuaZip::Mode mode)
{
    return mQuaZip.open(mode);
}

void QZip::close()
{
    mQuaZip.close();
}
