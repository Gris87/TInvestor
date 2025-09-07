#include "src/threads/bidirtrading/bidirtradingthread.h"

#include <QDebug>

#include "src/grpc/utils.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";

constexpr qint64 MS_IN_SECOND         = 1000LL;
constexpr qint64 SLEEP_DELAY          = 30LL * MS_IN_SECOND; // 30 seconds
constexpr qint64 SLEEP_BEFORE_REQUEST = 1LL * MS_IN_SECOND;  // 1 second



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
    mInstrumentId(instrumentId),
    mTerminateTrading(),
    mInstrumentLot(),
    mMinPriceIncrement(),
    mBuyOrderId(),
    mSellOrderId(),
    mLastBuyOrderPrice(),
    mLastSellOrderPrice()
{
    qDebug() << "Create BiDirTradingThread";

    mLogsThread->addLog(LOG_LEVEL_DEBUG, mInstrumentId, cause);
}

BiDirTradingThread::~BiDirTradingThread()
{
    qDebug() << "Destroy BiDirTradingThread";

    cancelBuyOrder();
    cancelSellOrder();
}

void BiDirTradingThread::run()
{
    qDebug() << "Running BiDirTradingThread";

    blockSignals(false);

    if (trade())
    {
        mLogsThread->addLog(LOG_LEVEL_VERBOSE, mInstrumentId, tr("Reselling completed successfully"));

        cancelBuyOrder();
        cancelSellOrder();

        emit tradingCompleted(mInstrumentId);
    }

    qDebug() << "Finish BiDirTradingThread";
}

void BiDirTradingThread::terminateTrading()
{
    mTerminateTrading = true;
}

void BiDirTradingThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}

bool BiDirTradingThread::trade()
{
    getInstrumentData();

    while (!mTerminateTrading)
    {
        if (mTimeUtils->interruptibleSleep(SLEEP_DELAY, QThread::currentThread()))
        {
            return false;
        }
    }

    return true;
}

void BiDirTradingThread::getInstrumentData()
{
    mInstrumentsStorage->readLock();

    const Instruments& instruments = mInstrumentsStorage->getInstruments();
    Q_ASSERT_X(instruments.contains(mInstrumentId), __FUNCTION__, "Data about instrument not found");
    const Instrument& instrument = instruments.value(mInstrumentId);

    mInstrumentLot     = instrument.lot;
    mMinPriceIncrement = instrument.minPriceIncrement;

    mInstrumentsStorage->readUnlock();
}

std::shared_ptr<tinkoff::PortfolioResponse> BiDirTradingThread::getValidPortfolio()
{
    bool                                        success          = false;
    std::shared_ptr<tinkoff::PortfolioResponse> tinkoffPortfolio = nullptr;

    while (!QThread::currentThread()->isInterruptionRequested() && !success)
    {
        tinkoffPortfolio = mGrpcClient->getPortfolio(QThread::currentThread(), mAccountId);

        if (!QThread::currentThread()->isInterruptionRequested() && tinkoffPortfolio != nullptr)
        {
            if (validatePortfolioResponse(*tinkoffPortfolio))
            {
                success = true;
            }
            else
            {
                qDebug() << "Invalid portfolio received. Try one more time";

                if (mTimeUtils->interruptibleSleep(SLEEP_BEFORE_REQUEST, QThread::currentThread()))
                {
                    break;
                }
            }
        }
        else
        {
            break;
        }
    }

    return tinkoffPortfolio;
}

bool BiDirTradingThread::validatePortfolioResponse(const tinkoff::PortfolioResponse& tinkoffPortfolio)
{
    bool res = true;

    for (int i = 0; i < tinkoffPortfolio.positions_size(); ++i)
    {
        const tinkoff::PortfolioPosition& position = tinkoffPortfolio.positions(i);

        const QString instrumentId = QString::fromStdString(position.instrument_uid());

        if (instrumentId != RUBLE_UID)
        {
            if (position.average_position_price_fifo().units() <= 0 && position.average_position_price_fifo().nano() <= 0)
            {
                res = false;

                break;
            }
        }
    }

    return res;
}

void BiDirTradingThread::cancelBuyOrder()
{
    if (mBuyOrderId != "")
    {
        mGrpcClient->cancelOrder(QThread::currentThread(), mAccountId, mBuyOrderId);
        mBuyOrderId = "";
    }
}

void BiDirTradingThread::cancelSellOrder()
{
    if (mSellOrderId != "")
    {
        mGrpcClient->cancelOrder(QThread::currentThread(), mAccountId, mSellOrderId);
        mSellOrderId = "";
    }
}
