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
    bool                 asap,
    float                avgPrice,
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
        asap,
        avgPrice,
        expectedCost,
        cause,
        parent
    );
}
