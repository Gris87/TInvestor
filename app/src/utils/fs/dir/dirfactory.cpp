#include "src/utils/fs/dir/dirfactory.h"

#include <QDebug>

#include "src/utils/fs/dir/dir.h"



DirFactory::DirFactory() :
    IDirFactory()
{
    qDebug() << "Create DirFactory";
}

DirFactory::~DirFactory()
{
    qDebug() << "Destroy DirFactory";
}

IDir* DirFactory::newInstance(const QString& path)
{
    return new Dir(path);
}
