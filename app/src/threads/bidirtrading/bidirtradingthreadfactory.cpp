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
    ITimeUtils*          timeUtils,
    IGrpcClient*         grpcClient,
    ILogsThread*         logsThread,
    const QString&       accountId,
    const QString&       instrumentId,
    const QString&       cause,
    QObject*             parent
) const
{
    return new BiDirTradingThread(instrumentsStorage, timeUtils, grpcClient, logsThread, accountId, instrumentId, cause, parent);
}
