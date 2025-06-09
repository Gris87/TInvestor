#include "src/threads/trading/tradingthread.h"

#include <QDebug>

#include "src/grpc/utils.h"



TradingThread::TradingThread(
    IInstrumentsStorage* instrumentsStorage,
    IGrpcClient*         grpcClient,
    ILogsThread*         logsThread,
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
    mInstrumentsStorage->getMutex()->lock();
    const Instruments& instrumentsData = mInstrumentsStorage->getInstruments();
    Q_ASSERT_X(instrumentsData.contains(mInstrumentId), "TradingThread::trade()", "Data about instrument not found");
    const qint32 lot = instrumentsData.value(mInstrumentId).lot;
    mInstrumentsStorage->getMutex()->unlock();

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

        bool completed;

        if (delta < 0)
        {
            completed = sell(lot, -delta, expected == 0);
        }
        else
        {
            completed = buy(lot, delta);
        }

        qInfo() << mInstrumentId << lot << cost << expected << completed;

        if (completed)
        {
            break;
        }

        break;
    }

    return true;
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

    if (tinkoffOrderBook->bids_size() > 0)
    {
        Quotation bidPrice = quotationConvert(tinkoffOrderBook->bids(0).price());
        qInfo() << mInstrumentId << lot << delta << sellAll << bidPrice.units << bidPrice.nano;
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

    qInfo() << mInstrumentId << lot << delta;

    return false;
}
