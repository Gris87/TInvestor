#include "src/threads/bidirtrading/bidirtradingthread.h"

#include <QDebug>

#include "src/grpc/utils.h"



BiDirTradingThread::BiDirTradingThread(
    IInstrumentsStorage* instrumentsStorage,
    ITimeUtils*          timeUtils,
    IGrpcClient*         grpcClient,
    ILogsThread*         logsThread,
    const QString&       accountId,
    const QString&       instrumentId,
    const QString&       cause,
    QObject*             parent
) :
    IBiDirTradingThread(parent),
    mInstrumentsStorage(instrumentsStorage),
    mTimeUtils(timeUtils),
    mGrpcClient(grpcClient),
    mLogsThread(logsThread),
    mAccountId(accountId),
    mInstrumentId(instrumentId)
{
    qDebug() << "Create BiDirTradingThread";

    mLogsThread->addLog(LOG_LEVEL_DEBUG, mInstrumentId, cause);
}

BiDirTradingThread::~BiDirTradingThread()
{
    qDebug() << "Destroy BiDirTradingThread";
}

void BiDirTradingThread::run()
{
    qDebug() << "Running BiDirTradingThread";

    blockSignals(false);

    if (trade())
    {
        mLogsThread->addLog(LOG_LEVEL_VERBOSE, mInstrumentId, tr("Reselling completed successfully"));

        emit tradingCompleted(mInstrumentId);
    }

    qDebug() << "Finish BiDirTradingThread";
}

void BiDirTradingThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}

bool BiDirTradingThread::trade()
{
    return true;
}
