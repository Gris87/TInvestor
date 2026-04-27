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

std::shared_ptr<IFile> FileFactory::newInstance(const QString& name) const
{
    return std::shared_ptr<IFile>(new File(name));
}
