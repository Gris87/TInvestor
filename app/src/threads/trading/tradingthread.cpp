#include "src/threads/trading/tradingthread.h"

#include <QDebug>



TradingThread::TradingThread(
    IGrpcClient* grpcClient, const QString& accountId, const QString& instrumentId, double expectedCost, QObject* parent
) :
    ITradingThread(parent),
    mMutex(new QMutex()),
    mGrpcClient(grpcClient),
    mAccountId(accountId),
    mInstrumentId(instrumentId),
    mExpectedCost(expectedCost)
{
    qDebug() << "Create TradingThread";
}

TradingThread::~TradingThread()
{
    qDebug() << "Destroy TradingThread";

    delete mMutex;
}

void TradingThread::run()
{
    qDebug() << "Running TradingThread";

    blockSignals(false);

    // TODO: Implement it
    qInfo() << mInstrumentId << mExpectedCost;

    emit tradingCompleted(mInstrumentId);

    qDebug() << "Finish TradingThread";
}

void TradingThread::setExpectedCost(double expectedCost)
{
    const QMutexLocker lock(mMutex);

    mExpectedCost = expectedCost;
}

double TradingThread::expectedCost() const
{
    const QMutexLocker lock(mMutex);

    return mExpectedCost;
}

void TradingThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}
