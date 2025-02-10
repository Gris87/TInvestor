#include "src/utils/fs/file/filefactory.h"

#include <QDebug>

#include "src/utils/fs/file/file.h"



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
