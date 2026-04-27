#include "src/utils/fs/dir/dir.h"

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

bool Dir::removeRecursively()
{
    return mDir.removeRecursively();
}

QStringList Dir::entryList(const QStringList& nameFilters, QDir::Filters filters, QDir::SortFlags sort)
{
    return mDir.entryList(nameFilters, filters, sort);
}
