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

std::shared_ptr<IDir> DirFactory::newInstance(const QString& path)
{
    return std::shared_ptr<IDir>(new Dir(path));
}
