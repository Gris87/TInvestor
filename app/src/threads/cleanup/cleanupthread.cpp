#include "src/threads/cleanup/cleanupthread.h"

#include <QDateTime>
#include <QDebug>
#include <QMutexLocker>



CleanupThread::CleanupThread(IConfig* config, IStocksStorage* stocksStorage, QObject* parent) :
    ICleanupThread(parent),
    mConfig(config),
    mStocksStorage(stocksStorage)
{
    qDebug() << "Create CleanupThread";
}

CleanupThread::~CleanupThread()
{
    qDebug() << "Destroy CleanupThread";
}

void CleanupThread::run()
{
    qDebug() << "Running CleanupThread";

    qint64 obsoleteTimestamp = QDateTime::currentSecsSinceEpoch() - mConfig->getStorageMonthLimit() * 31 * 24 * 60 * 60;

    QMutexLocker lock(mStocksStorage->getMutex());
    mStocksStorage->deleteObsoleteData(obsoleteTimestamp);

    qDebug() << "Finish CleanupThread";
}
