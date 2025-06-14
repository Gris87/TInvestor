#include "src/storage/logos/logosstorage.h"

#include <QDebug>



LogosStorage::LogosStorage(ILogosDatabase* logosDatabase) :
    ILogosStorage(),
    mLogosDatabase(logosDatabase),
    mMutex(new QMutex()),
    mLogos(),
    mNotFoundLogo()
{
    qDebug() << "Create LogosStorage";

    mNotFoundLogo.load(":/assets/images/no_image.png", "PNG");
}

LogosStorage::~LogosStorage()
{
    qDebug() << "Destroy LogosStorage";

    delete mMutex;

    for (auto it = mLogos.constBegin(); it != mLogos.constEnd(); ++it)
    {
        delete it.value();
    }
}

void LogosStorage::readFromDatabase()
{
    mLogos = mLogosDatabase->readLogos();
}

void LogosStorage::lock()
{
    mMutex->lock();
}

void LogosStorage::unlock()
{
    mMutex->unlock();
}

void LogosStorage::setLogo(const QString& instrumentId, const QPixmap& logo)
{
    QPixmap* logoInStorage = mLogos.value(instrumentId);

    if (logoInStorage != nullptr)
    {
        *logoInStorage = logo;
    }
    else
    {
        logoInStorage        = new QPixmap(logo);
        mLogos[instrumentId] = logoInStorage;
    }

    mLogosDatabase->writeLogo(instrumentId, logoInStorage);
}

QPixmap* LogosStorage::getLogo(const QString& instrumentId)
{
    if (instrumentId == "")
    {
        return nullptr;
    }

    QPixmap* res = mLogos.value(instrumentId);

    if (res == nullptr)
    {
        res = &mNotFoundLogo;
    }

    return res;
}
