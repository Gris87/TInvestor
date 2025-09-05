#include "src/threads/bidirtradingcontrol/bidirtradingcontrolthread.h"

#include <QDateTime>
#include <QDebug>



constexpr qint64 MS_IN_SECOND       = 1000LL;
constexpr qint64 ONE_MINUTE         = 60LL * MS_IN_SECOND;
constexpr qint64 DETECTION_INTERVAL = 15LL * ONE_MINUTE; // 15 minutes



BiDirTradingControlThread::BiDirTradingControlThread(
    IStocksStorage* stocksStorage, IConfig* config, IGrpcClient* grpcClient, QObject* parent
) :
    IBiDirTradingControlThread(parent),
    mStocksStorage(stocksStorage),
    mConfig(config),
    mGrpcClient(grpcClient),
    mAccountId(),
    mLastDetectionTimestamp()
{
    qDebug() << "Create BiDirTradingControlThread";
}

BiDirTradingControlThread::~BiDirTradingControlThread()
{
    qDebug() << "Destroy BiDirTradingControlThread";
}

void BiDirTradingControlThread::run()
{
    qDebug() << "Running BiDirTradingControlThread";

    blockSignals(false);

    const qint64 timestamp = QDateTime::currentMSecsSinceEpoch();

    if (timestamp - mLastDetectionTimestamp > DETECTION_INTERVAL)
    {
        detectHugeSpreadStocks();
        mLastDetectionTimestamp = timestamp;
    }

    qDebug() << "Finish BiDirTradingControlThread";
}

void BiDirTradingControlThread::setAccountId(const QString& accountId)
{
    mAccountId = accountId;
}

void BiDirTradingControlThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}

void BiDirTradingControlThread::detectHugeSpreadStocks()
{
    if (mConfig->isTradeHugeSpread())
    {
        /*
        mStocksStorage->readLock();
        QList<Stock*> stocks = mStocksStorage->getStocks();
        mStocksStorage->readUnlock();
        */

        // TODO: Implement
    }

    if (mConfig->isTradeLiquidityEtfDaily())
    {
        // TODO: Implement
    }
}
