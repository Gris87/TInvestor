#include "src/storage/logos/logosstorage.h"

#include <QDebug>



LogosStorage::LogosStorage(ILogosDatabase* logosDatabase) :
    ILogosStorage(),
    mLogosDatabase(logosDatabase),
    mMutex(new QMutex()),
    mLogos()
{
    qDebug() << "Create LogosStorage";
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
        logoInStorage = new QPixmap(logo);
        mLogos[instrumentId] = logoInStorage;
    }

    mLogosDatabase->writeLogo(instrumentId, logoInStorage);
}

QPixmap* LogosStorage::getLogo(const QString& instrumentId)
{
    QPixmap* res = mLogos.value(instrumentId);

    if (res == nullptr)
    {
        res = new QPixmap();
        mLogosDatabase->readLogo(instrumentId, res);
        mLogos[instrumentId] = res;
    }

    return res;
}
