#include "src/threads/cleanup/cleanupthread.h"

#include <QDateTime>
#include <QDebug>
#include <QMutexLocker>



#define ONE_MONTH 2678400000LL // 31 * 24 * 60 * 60 * 1000 // 31 days



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

    qint64 obsoleteTimestamp = QDateTime::currentMSecsSinceEpoch() - mConfig->getStorageMonthLimit() * ONE_MONTH;

    QMutexLocker lock(mStocksStorage->getMutex());
    mStocksStorage->deleteObsoleteData(obsoleteTimestamp);

    qDebug() << "Finish CleanupThread";
}
