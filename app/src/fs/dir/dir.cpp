#include "src/fs/dir/dir.h"

#include <QDebug>



Dir::Dir() :
    IDir()
{
    qDebug() << "Create Dir";
}

Dir::~Dir()
{
    qDebug() << "Destroy Dir";
}
