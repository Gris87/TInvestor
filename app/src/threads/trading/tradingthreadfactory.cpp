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

ITradingThread* TradingThreadFactory::newInstance(IGrpcClient* grpcClient, QObject* parent) const
{
    return new TradingThread(grpcClient, parent);
}
