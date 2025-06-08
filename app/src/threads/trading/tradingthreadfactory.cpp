#include "src/threads/trading/tradingthreadfactory.h"

#include <QDebug>

#include "src/threads/trading/tradingthread.h"



TradingThreadFactory::TradingThreadFactory() :
    ITradingThreadFactory()
{
    qDebug() << "Create TradingThreadFactory";
}

TradingThreadFactory::~TradingThreadFactory()
{
    qDebug() << "Destroy TradingThreadFactory";
}

ITradingThread* TradingThreadFactory::newInstance(
    IGrpcClient* grpcClient, const QString& accountId, const QString& instrumentId, double expectedCost, QObject* parent
) const
{
    return new TradingThread(grpcClient, accountId, instrumentId, expectedCost, parent);
}
