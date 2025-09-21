#include "src/threads/bidirtrading/bidirtradingthreadfactory.h"

#include <QDebug>

#include "src/threads/bidirtrading/bidirtradingthread.h"



BiDirTradingThreadFactory::BiDirTradingThreadFactory() :
    IBiDirTradingThreadFactory()
{
    qDebug() << "Create BiDirTradingThreadFactory";
}

BiDirTradingThreadFactory::~BiDirTradingThreadFactory()
{
    qDebug() << "Destroy BiDirTradingThreadFactory";
}

IBiDirTradingThread* BiDirTradingThreadFactory::newInstance(
    IInstrumentsStorage* instrumentsStorage,
    IUserStorage*        userStorage,
    IConfig*             config,
    ITimeUtils*          timeUtils,
    ITradeUtils*         tradeUtils,
    IGrpcClient*         grpcClient,
    IGrpcRetryClient*    grpcRetryClient,
    ILogsThread*         logsThread,
    const QString&       accountId,
    Stock*               stock,
    const QString&       cause,
    QObject*             parent
) const
{
    return new BiDirTradingThread(
        instrumentsStorage,
        userStorage,
        config,
        timeUtils,
        tradeUtils,
        grpcClient,
        grpcRetryClient,
        logsThread,
        accountId,
        stock,
        cause,
        parent
    );
}
