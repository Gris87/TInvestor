#include "src/fs/file/filefactory.h"

#include <QDebug>

#include "src/fs/file/file.h"



FileFactory::FileFactory() :
    IFileFactory()
{
    qDebug() << "Create FileFactory";
}

FileFactory::~FileFactory()
{
    qDebug() << "Destroy FileFactory";
}

IFile* FileFactory::newInstance(const QString& name)
{
    return new File(name);
}
