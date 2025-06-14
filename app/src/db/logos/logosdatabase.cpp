#include "src/db/logos/logosdatabase.h"

#include <QCoreApplication>
#include <QDebug>



LogosDatabase::LogosDatabase(IDirFactory* dirFactory, IFileFactory* fileFactory) :
    ILogosDatabase(),
    mFileFactory(fileFactory)
{
    qDebug() << "Create LogosDatabase";

    const std::shared_ptr<IDir> dir = dirFactory->newInstance();

    const bool ok = dir->mkpath(qApp->applicationDirPath() + "/data/instruments/logos");
    Q_ASSERT_X(ok, "LogosDatabase::LogosDatabase()", "Failed to create dir");
}

LogosDatabase::~LogosDatabase()
{
    qDebug() << "Destroy LogosDatabase";
}

void LogosDatabase::readLogo(const QString& instrumentId, QPixmap* logo)
{
    const std::shared_ptr<IFile> logoFile =
        mFileFactory->newInstance(QString("%1/data/instruments/logos/%2.png").arg(qApp->applicationDirPath(), instrumentId));

    if (logoFile->open(QIODevice::ReadOnly))
    {
        const QByteArray content = logoFile->readAll();
        logoFile->close();

        if (!logo->loadFromData(content, "PNG"))
        {
            *logo = QPixmap();
        }
    }
    else
    {
        *logo = QPixmap();
    }
}

void LogosDatabase::writeLogo(const QString& instrumentId, QPixmap* logo)
{
    const std::shared_ptr<IFile> logoFile =
        mFileFactory->newInstance(QString("%1/data/instruments/logos/%2.png").arg(qApp->applicationDirPath(), instrumentId));

    const bool ok = logoFile->open(QIODevice::WriteOnly);
    Q_ASSERT_X(ok, "LogosDatabase::writeLogo()", "Failed to open file");

    logo->save(logoFile->getDevice(), "PNG");
    logoFile->close();
}
