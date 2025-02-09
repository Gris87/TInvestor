#include "src/fs/file/file.h"

#include <QDebug>



File::File() :
    IFile()
{
    qDebug() << "Create File";
}

File::~File()
{
    qDebug() << "Destroy File";
}
