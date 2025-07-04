#include "src/threads/cleanup/cleanupthread.h"

#include <QDateTime>
#include <QDebug>



constexpr qint64 MS_IN_SECOND = 1000LL;
constexpr qint64 ONE_MINUTE   = 60LL * MS_IN_SECOND;
constexpr qint64 ONE_HOUR     = 60LL * ONE_MINUTE;
constexpr qint64 ONE_DAY      = 24LL * ONE_HOUR;
constexpr qint64 ONE_MONTH    = 31LL * ONE_DAY;



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

    blockSignals(false);

    const qint64 obsoleteTimestamp = QDateTime::currentMSecsSinceEpoch() - (mConfig->getStorageMonthLimit() * ONE_MONTH);

    mStocksStorage->readLock();
    mStocksStorage->deleteObsoleteData(obsoleteTimestamp);
    mStocksStorage->readUnlock();

    qDebug() << "Finish CleanupThread";
}

void CleanupThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}
