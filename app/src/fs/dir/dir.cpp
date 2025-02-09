#include "src/fs/dir/dir.h"

#include <QDebug>



Dir::Dir(const QString& path) :
    IDir(),
    mDir(path)
{
    qDebug() << "Create Dir";
}

Dir::~Dir()
{
    qDebug() << "Destroy Dir";
}

bool Dir::mkpath(const QString& dirPath)
{
    return mDir.mkpath(dirPath);
}
