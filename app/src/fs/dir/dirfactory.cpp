#include "src/fs/dir/dirfactory.h"

#include <QDebug>

#include "src/fs/dir/dir.h"



DirFactory::DirFactory() :
    IDirFactory()
{
    qDebug() << "Create DirFactory";
}

DirFactory::~DirFactory()
{
    qDebug() << "Destroy DirFactory";
}

IDir* DirFactory::newInstance()
{
    return new Dir();
}
