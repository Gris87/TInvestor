#include "src/storage/logos/logosstorage.h"

#include <QDebug>



LogosStorage::LogosStorage(ILogosDatabase* logosDatabase) :
    ILogosStorage(),
    mLogosDatabase(logosDatabase),
    mRwMutex(new QReadWriteLock()),
    mLogos(),
    mNotFoundLogo()
{
    qDebug() << "Create LogosStorage";

    mNotFoundLogo.load(":/assets/images/no_image.png", "PNG");
}

LogosStorage::~LogosStorage()
{
    qDebug() << "Destroy LogosStorage";

    delete mRwMutex;

    for (auto it = mLogos.constBegin(); it != mLogos.constEnd(); ++it)
    {
        delete it.value();
    }
}

void LogosStorage::readFromDatabase()
{
    mLogos = mLogosDatabase->prepareLogos();
}

void LogosStorage::readLock()
{
    mRwMutex->lockForRead();
}

void LogosStorage::readUnlock()
{
    mRwMutex->unlock();
}

void LogosStorage::writeLock()
{
    mRwMutex->lockForWrite();
}

void LogosStorage::writeUnlock()
{
    mRwMutex->unlock();
}

void LogosStorage::setLogo(const QString& instrumentId, const QPixmap& logo)
{
    Logo* logoInStorage = mLogos.value(instrumentId);

    if (logoInStorage != nullptr)
    {
        logoInStorage->pixmap = logo;
    }
    else
    {
        logoInStorage         = new Logo();
        logoInStorage->pixmap = logo;

        mLogos[instrumentId] = logoInStorage;
    }

    mLogosDatabase->writeLogo(instrumentId, &logoInStorage->pixmap);
}

QPixmap* LogosStorage::getLogo(const QString& instrumentId)
{
    if (instrumentId == "")
    {
        return nullptr;
    }

    Logo* res = mLogos.value(instrumentId);

    if (res == nullptr)
    {
        return &mNotFoundLogo;
    }

    res->mutex.lock();
    if (res->pixmap.isNull())
    {
        mLogosDatabase->readLogo(instrumentId, &res->pixmap);
    }
    res->mutex.unlock();

    return &res->pixmap;
}
