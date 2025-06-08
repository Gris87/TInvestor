#include "src/threads/trading/tradingthread.h"

#include <QDebug>



TradingThread::TradingThread(IGrpcClient* grpcClient, QObject* parent) :
    ITradingThread(parent),
    mGrpcClient(grpcClient),
    mAccountId()
{
    qDebug() << "Create TradingThread";
}

TradingThread::~TradingThread()
{
    qDebug() << "Destroy TradingThread";
}

void TradingThread::run()
{
    qDebug() << "Running TradingThread";

    qDebug() << "Finish TradingThread";
}

void TradingThread::setAccountId(const QString& accountId)
{
    mAccountId = accountId;
}

void TradingThread::terminateThread()
{
    requestInterruption();
}
