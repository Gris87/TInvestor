#include "src/utils/fs/file/file.h"

#include <QDebug>



File::File(const QString& name) :
    IFile(),
    mFile(name)
{
    qDebug() << "Create File";
}

File::~File()
{
    qDebug() << "Destroy File";
}

bool File::open(QIODevice::OpenMode flags)
{
    return mFile.open(flags);
}

qint64 File::read(char* data, qint64 maxlen)
{
    return mFile.read(data, maxlen);
}

QByteArray File::readAll()
{
    return mFile.readAll();
}

qint64 File::write(const char* data, qint64 len)
{
    return mFile.write(data, len);
}

qint64 File::write(const QByteArray& data)
{
    return mFile.write(data);
}

qint64 File::size()
{
    return mFile.size();
}

void File::close()
{
    mFile.close();
}

bool File::exists()
{
    return mFile.exists();
}
