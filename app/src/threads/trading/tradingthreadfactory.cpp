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
    IInstrumentsStorage* instrumentsStorage,
    ITimeUtils*          timeUtils,
    IGrpcClient*         grpcClient,
    ILogsThread*         logsThread,
    const QString&       accountId,
    const QString&       instrumentId,
    double               expectedCost,
    const QString&       cause,
    QObject*             parent
) const
{
    return new TradingThread(
        instrumentsStorage, timeUtils, grpcClient, logsThread, accountId, instrumentId, expectedCost, cause, parent
    );
}
