#include "cleanupthread.h"

#include <QDateTime>
#include <QDebug>
#include <QMutexLocker>



CleanupThread::CleanupThread(
    IConfig *config,
    IStocksDatabase *stocksDatabase,
    IStocksStorage *stocksStorage,
    QObject *parent
) :
    ICleanupThread(parent),
    mConfig(config),
    mStocksDatabase(stocksDatabase),
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

    QList<Stock> *stocks = mStocksStorage->getStocks();
    mStocksDatabase->deleteObsoleteData(obsoleteTimestamp, stocks);

    qDebug() << "Finish CleanupThread";
}
