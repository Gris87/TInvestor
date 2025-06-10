#include "src/threads/trading/tradingthread.h"

#include <QDebug>

#include "src/grpc/utils.h"



constexpr qint64 MS_IN_SECOND = 1000LL;
constexpr qint64 SLEEP_DELAY  = 30LL * MS_IN_SECOND; // 30 seconds



TradingThread::TradingThread(
    IInstrumentsStorage* instrumentsStorage,
    IGrpcClient*         grpcClient,
    ILogsThread*         logsThread,
    ITimeUtils*          timeUtils,
    const QString&       accountId,
    const QString&       instrumentId,
    double               expectedCost,
    QObject*             parent
) :
    ITradingThread(parent),
    mMutex(new QMutex()),
    mInstrumentsStorage(instrumentsStorage),
    mGrpcClient(grpcClient),
    mLogsThread(logsThread),
    mTimeUtils(timeUtils),
    mAccountId(accountId),
    mInstrumentId(instrumentId),
    mExpectedCost(expectedCost)
{
    qDebug() << "Create TradingThread";
}

TradingThread::~TradingThread()
{
    qDebug() << "Destroy TradingThread";

    delete mMutex;
}

void TradingThread::run()
{
    qDebug() << "Running TradingThread";

    blockSignals(false);

    if (trade())
    {
        emit tradingCompleted(mInstrumentId);
    }

    qDebug() << "Finish TradingThread";
}

void TradingThread::setExpectedCost(double expectedCost)
{
    const QMutexLocker lock(mMutex);

    mExpectedCost = expectedCost;
}

double TradingThread::expectedCost() const
{
    const QMutexLocker lock(mMutex);

    return mExpectedCost;
}

void TradingThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}

bool TradingThread::trade()
{
    const qint32 lot = getInstrumentLot();

    while (true)
    {
        const std::shared_ptr<tinkoff::PortfolioResponse> tinkoffPortfolio =
            mGrpcClient->getPortfolio(QThread::currentThread(), mAccountId);

        if (QThread::currentThread()->isInterruptionRequested() || tinkoffPortfolio == nullptr)
        {
            return false;
        }

        const double cost     = handlePortfolioResponse(*tinkoffPortfolio);
        const double expected = expectedCost();

        const double delta = expected - cost;

        bool completed = false;

        if (delta < 0)
        {
            completed = sell(lot, -delta, expected == 0);
        }
        else
        {
            completed = buy(lot, delta);
        }

        if (completed)
        {
            break;
        }

        if (mTimeUtils->interruptibleSleep(SLEEP_DELAY, this))
        {
            return false;
        }

        break;
    }

    return true;
}

qint32 TradingThread::getInstrumentLot() const
{
    const QMutexLocker lock(mInstrumentsStorage->getMutex());
    const Instruments& instrumentsData = mInstrumentsStorage->getInstruments();

    Q_ASSERT_X(instrumentsData.contains(mInstrumentId), "TradingThread::trade()", "Data about instrument not found");

    return instrumentsData.value(mInstrumentId).lot;
}

double TradingThread::handlePortfolioResponse(const tinkoff::PortfolioResponse& tinkoffPortfolio)
{
    for (int i = 0; i < tinkoffPortfolio.positions_size(); ++i)
    {
        const tinkoff::PortfolioPosition& position = tinkoffPortfolio.positions(i);

        if (QString::fromStdString(position.instrument_uid()) == mInstrumentId)
        {
            const double available    = quotationToDouble(position.quantity());
            const double avgPriceFifo = quotationToDouble(position.average_position_price_fifo());

            return available * avgPriceFifo;
        }
    }

    return 0;
}

bool TradingThread::sell(qint32 lot, double delta, bool sellAll)
{
    const std::shared_ptr<tinkoff::GetOrderBookResponse> tinkoffOrderBook =
        mGrpcClient->getOrderBook(QThread::currentThread(), mInstrumentId, 1);

    if (QThread::currentThread()->isInterruptionRequested() || tinkoffOrderBook == nullptr)
    {
        return false;
    }

    if (tinkoffOrderBook->asks_size() > 0)
    {
        const Quotation askPrice = quotationConvert(tinkoffOrderBook->asks(0).price());
        qInfo() << mInstrumentId << lot << delta << sellAll << askPrice.units << askPrice.nano;
    }

    return false;
}

bool TradingThread::buy(qint32 lot, double delta)
{
    const std::shared_ptr<tinkoff::GetOrderBookResponse> tinkoffOrderBook =
        mGrpcClient->getOrderBook(QThread::currentThread(), mInstrumentId, 1);

    if (QThread::currentThread()->isInterruptionRequested() || tinkoffOrderBook == nullptr)
    {
        return false;
    }

    if (tinkoffOrderBook->bids_size() > 0)
    {
        const Quotation bidPrice = quotationConvert(tinkoffOrderBook->bids(0).price());
        qInfo() << mInstrumentId << lot << delta << bidPrice.units << bidPrice.nano;
    }

    return false;
}
