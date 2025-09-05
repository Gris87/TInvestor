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
    IUserStorage*        userStorage,
    ITimeUtils*          timeUtils,
    IGrpcClient*         grpcClient,
    ILogsThread*         logsThread,
    const QString&       accountId,
    const QString&       instrumentId,
    AsapMode             asapMode,
    float                avgPrice,
    float                price,
    double               expectedCost,
    const QString&       cause,
    QObject*             parent
) const
{
    return new TradingThread(
        instrumentsStorage,
        userStorage,
        timeUtils,
        grpcClient,
        logsThread,
        accountId,
        instrumentId,
        asapMode,
        avgPrice,
        price,
        expectedCost,
        cause,
        parent
    );
}
