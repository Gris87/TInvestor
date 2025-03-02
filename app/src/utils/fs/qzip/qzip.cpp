#include "src/utils/fs/qzip/qzip.h"

#include <QDebug>



QZip::QZip(const QString& /*name*/) :
    IQZip()
{
    qDebug() << "Create QZip";
}

QZip::~QZip()
{
    qDebug() << "Destroy QZip";
}
