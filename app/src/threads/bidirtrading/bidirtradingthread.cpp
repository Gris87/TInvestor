#include "src/threads/bidirtrading/bidirtradingthread.h"

#include <QDebug>

#include "src/grpc/utils.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";
const char* const TMON_UID  = "498ec3ff-ef27-4729-9703-a5aac48d5789";

constexpr float  HUNDRED_PERCENT      = 100.0f;
constexpr qint64 MS_IN_SECOND         = 1000LL;
constexpr qint64 SLEEP_DELAY          = 30LL * MS_IN_SECOND; // 30 seconds
constexpr qint64 ORDER_CANCEL_DELAY   = 3LL * MS_IN_SECOND;  // 3 seconds
constexpr qint64 SLEEP_BEFORE_REQUEST = 1LL * MS_IN_SECOND;  // 1 second



BiDirTradingThread::BiDirTradingThread(
    IInstrumentsStorage* instrumentsStorage,
    IConfig*             config,
    ITimeUtils*          timeUtils,
    ITradeUtils*         tradeUtils,
    IGrpcClient*         grpcClient,
    ILogsThread*         logsThread,
    const QString&       accountId,
    const QString&       instrumentId,
    qint64               turnover,
    const QString&       cause,
    QObject*             parent
) :
    IBiDirTradingThread(parent),
    mRwMutex(new QReadWriteLock()),
    mInstrumentsStorage(instrumentsStorage),
    mConfig(config),
    mTimeUtils(timeUtils),
    mTradeUtils(tradeUtils),
    mGrpcClient(grpcClient),
    mLogsThread(logsThread),
    mAccountId(accountId),
    mInstrumentId(instrumentId),
    mTurnover(turnover),
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

    delete mRwMutex;
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

void BiDirTradingThread::setTurnover(qint64 turnover)
{
    const QWriteLocker lock(mRwMutex);

    mTurnover = turnover;
}

qint64 BiDirTradingThread::turnover() const
{
    const QReadLocker lock(mRwMutex);

    return mTurnover;
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
        const std::shared_ptr<tinkoff::GetOrderBookResponse> tinkoffOrderBook =
            mGrpcClient->getOrderBook(QThread::currentThread(), mInstrumentId, 3);

        if (QThread::currentThread()->isInterruptionRequested() || tinkoffOrderBook == nullptr)
        {
            return false;
        }

        if (tinkoffOrderBook->bids_size() <= 0 || tinkoffOrderBook->asks_size() <= 0)
        {
            mLogsThread->addLog(LOG_LEVEL_VERBOSE, mInstrumentId, tr("Impossible to continue reselling"));

            break;
        }

        const float bidPrice = quotationToFloat(tinkoffOrderBook->bids(0).price());
        const float askPrice = quotationToFloat(tinkoffOrderBook->asks(0).price());

        bool good = true;

        if (mInstrumentId != TMON_UID)
        {
            const float spread = (askPrice / bidPrice) * HUNDRED_PERCENT - HUNDRED_PERCENT;
            good               = spread > mConfig->getHugeSpread();
        }

        if (good)
        {
            const std::shared_ptr<tinkoff::PortfolioResponse> tinkoffPortfolio = getValidPortfolio();

            if (QThread::currentThread()->isInterruptionRequested() || tinkoffPortfolio == nullptr)
            {
                return false;
            }

            const double lotPrice = mInstrumentLot * bidPrice;

            const qint64 coefBuy  = static_cast<qint64>(std::floor(bidPrice / quotationToDouble(mMinPriceIncrement)));
            const qint64 coefSell = static_cast<qint64>(std::ceil(askPrice / quotationToDouble(mMinPriceIncrement)));

            const Quotation buyPrice  = quotationMultiply(mMinPriceIncrement, coefBuy);
            const Quotation sellPrice = quotationMultiply(mMinPriceIncrement, coefSell);

            double totalCost      = 0.0;
            qint64 instrumentLots = 0;

            calculateTotalCostAndInstrumentLots(*tinkoffPortfolio, totalCost, instrumentLots);

            const qint64 lotsToKeep = mTradeUtils->calculateAmountOfLotsToBuy(
                mConfig, QDateTime::currentMSecsSinceEpoch(), totalCost, totalCost, turnover(), lotPrice, lotPrice
            );
            qint64 lotsToBuy = qMax(lotsToKeep - instrumentLots, 0);

            for (int i = 0; i < tinkoffOrderBook->bids_size(); ++i)
            {
                lotsToBuy = qMin(lotsToBuy, tinkoffOrderBook->bids(i).quantity());
            }

            for (int i = 0; i < tinkoffOrderBook->asks_size(); ++i)
            {
                lotsToBuy = qMin(lotsToBuy, tinkoffOrderBook->asks(i).quantity());
            }

            bool needToCancelBuy  = false;
            bool needToCancelSell = false;
            bool needToOrderBuy   = false;
            bool needToOrderSell  = false;

            checkIfNeedToCancelAndCreateOrder(mBuyOrderId, lotsToBuy, buyPrice, needToCancelBuy, needToOrderBuy);
            checkIfNeedToCancelAndCreateOrder(mSellOrderId, instrumentLots, sellPrice, needToCancelSell, needToOrderSell);

            if (needToCancelBuy)
            {
                cancelBuyOrder();
            }

            if (needToCancelSell)
            {
                cancelSellOrder();
            }

            if (needToCancelBuy || needToCancelSell)
            {
                if (mTimeUtils->interruptibleSleep(ORDER_CANCEL_DELAY, QThread::currentThread()))
                {
                    return false;
                }
            }

            if (needToOrderBuy)
            {
                buyWithPrice(lotsToBuy, buyPrice);
            }

            if (needToOrderSell)
            {
                sellWithPrice(instrumentLots, sellPrice);
            }
        }

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

void BiDirTradingThread::calculateTotalCostAndInstrumentLots(
    const tinkoff::PortfolioResponse& tinkoffPortfolio, double& totalCost, qint64& instrumentLots
)
{
    totalCost      = 0.0;
    instrumentLots = 0;

    for (int i = 0; i < tinkoffPortfolio.positions_size() && !QThread::currentThread()->isInterruptionRequested(); ++i)
    {
        const tinkoff::PortfolioPosition& position = tinkoffPortfolio.positions(i);

        const QString instrumentId = QString::fromStdString(position.instrument_uid());

        if (instrumentId == RUBLE_UID)
        {
            totalCost += quotationToDouble(position.quantity());
        }
        else
        {
            const double cost = quotationToDouble(position.quantity()) * quotationToFloat(position.average_position_price_fifo());

            if (instrumentId == mInstrumentId)
            {
                instrumentLots = quotationToDouble(position.quantity()) / mInstrumentLot;
            }

            totalCost += cost;
        }
    }
}

void BiDirTradingThread::checkIfNeedToCancelAndCreateOrder(
    const QString& orderId, qint64 amountOfLots, const Quotation& price, bool& needToCancel, bool& needToOrder
)
{
    needToCancel = false;
    needToOrder  = amountOfLots > 0;

    if (orderId != "")
    {
        const std::shared_ptr<tinkoff::OrderState> tinkoffOrder =
            mGrpcClient->getOrderState(QThread::currentThread(), mAccountId, orderId);

        if (!QThread::currentThread()->isInterruptionRequested() && tinkoffOrder != nullptr)
        {
            if (quotationConvert(tinkoffOrder->initial_order_price()) == price &&
                tinkoffOrder->lots_requested() - tinkoffOrder->lots_executed() == amountOfLots)
            {
                needToOrder = false;
            }
            else
            {
                needToCancel = tinkoffOrder->execution_report_status() != tinkoff::EXECUTION_REPORT_STATUS_FILL;
            }
        }
    }
}

void BiDirTradingThread::sellWithPrice(qint64 amountOfLots, const Quotation& price)
{
    qInfo() << "SELL" << amountOfLots << quotationToDouble(price);
}

void BiDirTradingThread::buyWithPrice(qint64 amountOfLots, const Quotation& price)
{
    qInfo() << "BUY" << amountOfLots << quotationToDouble(price);
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
