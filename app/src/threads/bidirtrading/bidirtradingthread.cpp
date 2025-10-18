#include "src/threads/bidirtrading/bidirtradingthread.h"

#include <QDebug>

#include "src/grpc/utils.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";

constexpr float  HUNDRED_PERCENT          = 100.0f;
constexpr float  MINIMUM_YIELD_PERCENT    = 0.10f;
constexpr float  MAXIMUM_LOSE_PERCENT     = 2.00f;
constexpr float  MINIMUM_HUGE_BID_PERCENT = 25.0f;
constexpr int    ORDER_BOOK_DEPTH         = 20;
constexpr qint64 MS_IN_SECOND             = 1000LL;
constexpr qint64 SLEEP_DELAY              = 30LL * MS_IN_SECOND; // 30 seconds
constexpr qint64 ORDER_CANCEL_DELAY       = 3LL * MS_IN_SECOND;  // 3 seconds
constexpr qint64 ORDER_RETRY_DELAY        = 1LL * MS_IN_SECOND;  // 1 second



BiDirTradingThread::BiDirTradingThread(
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
    BiDirMode            bidirMode,
    const QString&       cause,
    QObject*             parent
) :
    IBiDirTradingThread(parent),
    mRwMutex(new QReadWriteLock()),
    mInstrumentsStorage(instrumentsStorage),
    mUserStorage(userStorage),
    mConfig(config),
    mTimeUtils(timeUtils),
    mTradeUtils(tradeUtils),
    mGrpcClient(grpcClient),
    mGrpcRetryClient(grpcRetryClient),
    mLogsThread(logsThread),
    mAccountId(accountId),
    mStock(stock),
    mBidirMode(bidirMode),
    mTerminateTrading(),
    mInstrumentId(),
    mInstrumentLot(),
    mMinPriceIncrement(),
    mBuyOrderId(),
    mSellOrderId()
{
    qDebug() << "Create BiDirTradingThread";

    mStock->readLock();
    mInstrumentId = mStock->meta.instrumentId;
    mStock->readUnlock();

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

void BiDirTradingThread::setMode(BiDirMode bidirMode, const QString& cause)
{
    const QWriteLocker lock(mRwMutex);

    if (bidirMode > mBidirMode)
    {
        mBidirMode = bidirMode;

        mLogsThread->addLog(LOG_LEVEL_DEBUG, mInstrumentId, cause);
    }
}

BiDirMode BiDirTradingThread::bidirMode() const
{
    const QReadLocker lock(mRwMutex);

    return mBidirMode;
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

    mUserStorage->readLock();
    const float commission = mUserStorage->getCommission();
    mUserStorage->readUnlock();

    while (!mTerminateTrading)
    {
        const std::shared_ptr<tinkoff::GetOrderBookResponse> tinkoffOrderBook =
            mGrpcClient->getOrderBook(QThread::currentThread(), mInstrumentId, ORDER_BOOK_DEPTH);

        if (QThread::currentThread()->isInterruptionRequested() || tinkoffOrderBook == nullptr)
        {
            return false;
        }

        if (tinkoffOrderBook->bids_size() <= 0 || tinkoffOrderBook->asks_size() <= 0)
        {
            mLogsThread->addLog(LOG_LEVEL_VERBOSE, mInstrumentId, tr("Impossible to continue reselling"));

            break;
        }

        const std::shared_ptr<tinkoff::PortfolioResponse> tinkoffPortfolio =
            mGrpcRetryClient->getValidPortfolio(QThread::currentThread(), mAccountId);

        if (QThread::currentThread()->isInterruptionRequested() || tinkoffPortfolio == nullptr)
        {
            return false;
        }

        qint64    lotsToBuy  = -1;
        qint64    lotsToSell = -1;
        Quotation buyPrice;
        Quotation sellPrice;

        calculateBuySellPriceAndLots(
            *tinkoffOrderBook, *tinkoffPortfolio, commission, lotsToBuy, lotsToSell, buyPrice, sellPrice
        );

        bool needToCancelBuy  = false;
        bool needToCancelSell = false;
        bool needToOrderBuy   = false;
        bool needToOrderSell  = false;

        checkIfNeedToCancelAndCreateOrder(mBuyOrderId, lotsToBuy, buyPrice, needToCancelBuy, needToOrderBuy);
        checkIfNeedToCancelAndCreateOrder(mSellOrderId, lotsToSell, sellPrice, needToCancelSell, needToOrderSell);

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
            sellWithPrice(sellPrice);
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
            const tinkoff::OrderExecutionReportStatus status = tinkoffOrder->execution_report_status();

            if (status == tinkoff::EXECUTION_REPORT_STATUS_NEW || status == tinkoff::EXECUTION_REPORT_STATUS_PARTIALLYFILL)
            {
                if (quotationConvert(tinkoffOrder->initial_security_price()) == price &&
                    tinkoffOrder->lots_requested() - tinkoffOrder->lots_executed() == amountOfLots)
                {
                    needToOrder = false;
                }
                else
                {
                    needToCancel = true;
                }
            }
        }
    }
}

void BiDirTradingThread::sellWithPrice(const Quotation& price)
{
    while (true)
    {
        const std::shared_ptr<tinkoff::GetMaxLotsResponse> tinkoffMaxLots =
            mGrpcClient->getMaxLots(QThread::currentThread(), mAccountId, mInstrumentId, price);

        if (QThread::currentThread()->isInterruptionRequested() || tinkoffMaxLots == nullptr)
        {
            return;
        }

        const qint64 amountToSell = tinkoffMaxLots->sell_limits().sell_max_lots();

        if (amountToSell > 0)
        {
            const std::shared_ptr<tinkoff::PostOrderResponse> tinkoffOrder = mGrpcClient->postOrder(
                QThread::currentThread(), mAccountId, mInstrumentId, tinkoff::ORDER_DIRECTION_SELL, amountToSell, price
            );

            if (QThread::currentThread()->isInterruptionRequested() || tinkoffOrder == nullptr)
            {
                if (mTimeUtils->interruptibleSleep(ORDER_RETRY_DELAY, QThread::currentThread()))
                {
                    return;
                }

                continue;
            }

            if (tinkoffOrder->execution_report_status() != tinkoff::EXECUTION_REPORT_STATUS_REJECTED)
            {
                mSellOrderId = QString::fromStdString(tinkoffOrder->order_id());

                break;
            }

            if (mTimeUtils->interruptibleSleep(ORDER_RETRY_DELAY, QThread::currentThread()))
            {
                return;
            }
        }
        else
        {
            return;
        }
    }
}

void BiDirTradingThread::buyWithPrice(qint64 amountOfLots, const Quotation& price)
{
    while (true)
    {
        const std::shared_ptr<tinkoff::GetMaxLotsResponse> tinkoffMaxLots =
            mGrpcClient->getMaxLots(QThread::currentThread(), mAccountId, mInstrumentId, price);

        if (QThread::currentThread()->isInterruptionRequested() || tinkoffMaxLots == nullptr)
        {
            return;
        }

        const qint64 amountToBuy = qMin(amountOfLots, tinkoffMaxLots->buy_limits().buy_max_lots());

        if (amountToBuy > 0)
        {
            const std::shared_ptr<tinkoff::PostOrderResponse> tinkoffOrder = mGrpcClient->postOrder(
                QThread::currentThread(), mAccountId, mInstrumentId, tinkoff::ORDER_DIRECTION_BUY, amountToBuy, price
            );

            if (QThread::currentThread()->isInterruptionRequested() || tinkoffOrder == nullptr)
            {
                if (mTimeUtils->interruptibleSleep(ORDER_RETRY_DELAY, QThread::currentThread()))
                {
                    return;
                }

                continue;
            }

            if (tinkoffOrder->execution_report_status() != tinkoff::EXECUTION_REPORT_STATUS_REJECTED)
            {
                mBuyOrderId = QString::fromStdString(tinkoffOrder->order_id());

                break;
            }

            if (mTimeUtils->interruptibleSleep(ORDER_RETRY_DELAY, QThread::currentThread()))
            {
                return;
            }
        }
        else
        {
            return;
        }
    }
}

bool BiDirTradingThread::isNeedToSellAsap(qint64 timestamp, float part, float yield, float commission)
{
    if (mTimeUtils->isMorningSession(timestamp))
    {
        return false;
    }

    ISellDecision4Config* sellDecision4Config = chooseDecisionConfig()->getSellDecision4Config();

    if (sellDecision4Config->isEnabled() && yield < -sellDecision4Config->getLoseYield() + (2 * commission))
    {
        return true;
    }

    // TODO: Limits for huge bid mode

    return mConfig->isHugeSpreadLimitStockPurchase() && part < mConfig->getHugeSpreadLimitStockPurchasePart() * 2 &&
           yield < -MAXIMUM_LOSE_PERCENT;
}

void BiDirTradingThread::calculateTotalCostAndInstrumentCost(
    const tinkoff::PortfolioResponse& tinkoffPortfolio,
    double&                           totalCost,
    double&                           instrumentCost,
    qint64&                           instrumentLots,
    double&                           instrumentAvgPrice
)
{
    totalCost          = 0.0;
    instrumentCost     = 0.0;
    instrumentLots     = 0;
    instrumentAvgPrice = -1.0;

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
                instrumentCost     = cost;
                instrumentLots     = quotationToDouble(position.quantity()) / mInstrumentLot;
                instrumentAvgPrice = quotationToDouble(position.average_position_price());
            }

            totalCost += cost;
        }
    }
}

void BiDirTradingThread::calculateBuySellPriceAndLots(
    const tinkoff::GetOrderBookResponse& tinkoffOrderBook,
    const tinkoff::PortfolioResponse&    tinkoffPortfolio,
    float                                commission,
    qint64&                              lotsToBuy,
    qint64&                              lotsToSell,
    Quotation&                           buyPrice,
    Quotation&                           sellPrice
)
{
    double totalCost          = 0.0;
    double instrumentCost     = 0.0;
    qint64 instrumentLots     = 0;
    double instrumentAvgPrice = -1.0;

    calculateTotalCostAndInstrumentCost(tinkoffPortfolio, totalCost, instrumentCost, instrumentLots, instrumentAvgPrice);

    qint64 maxQuantity = 0;

    for (int i = 0; i < tinkoffOrderBook.bids_size(); ++i)
    {
        maxQuantity = qMax(maxQuantity, tinkoffOrderBook.bids(i).quantity());
    }

    const double bidPrice   = calculateBidPrice(tinkoffOrderBook, maxQuantity);
    const double askPrice   = calculateAskPrice(tinkoffOrderBook, totalCost, instrumentCost, instrumentAvgPrice, commission);
    const qint64 lotsToKeep = calculateLotsToKeep(totalCost, bidPrice);

    const qint64 coefBuy  = qRound64(bidPrice / quotationToDouble(mMinPriceIncrement));
    const qint64 coefSell = static_cast<qint64>(std::ceil(askPrice / quotationToDouble(mMinPriceIncrement)));

    lotsToBuy  = qMax(qMin(lotsToKeep - instrumentLots, maxQuantity), 0);
    lotsToSell = instrumentLots;

    buyPrice  = quotationMultiply(mMinPriceIncrement, coefBuy);
    sellPrice = quotationMultiply(mMinPriceIncrement, coefSell);
}

double BiDirTradingThread::calculateBidPrice(const tinkoff::GetOrderBookResponse& tinkoffOrderBook, qint64 maxQuantity)
{
    const double topBidPrice = quotationToDouble(tinkoffOrderBook.bids(0).price());
    const double topAskPrice = quotationToDouble(tinkoffOrderBook.asks(0).price());

    double res = topBidPrice;

    // TODO: Buy for huge bid mode

    const float hugeSpread = mConfig->getHugeSpread();

    for (int i = 0; i < tinkoffOrderBook.bids_size(); ++i)
    {
        if (tinkoffOrderBook.bids(i).quantity() > 0)
        {
            const double curPrice        = quotationToDouble(tinkoffOrderBook.bids(i).price());
            const float  spread          = ((topAskPrice / curPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;
            const float  quantityPercent = (tinkoffOrderBook.bids(i).quantity() * HUNDRED_PERCENT) / maxQuantity;

            if (spread >= hugeSpread && quantityPercent >= MINIMUM_HUGE_BID_PERCENT)
            {
                res = curPrice;

                break;
            }
        }
    }

    return res;
}

double BiDirTradingThread::calculateAskPrice(
    const tinkoff::GetOrderBookResponse& tinkoffOrderBook,
    double                               totalCost,
    double                               instrumentCost,
    double                               instrumentAvgPrice,
    float                                commission
)
{
    const double topBidPrice = quotationToDouble(tinkoffOrderBook.bids(0).price());
    const double topAskPrice = quotationToDouble(tinkoffOrderBook.asks(0).price());

    double res = topAskPrice;

    if (instrumentAvgPrice > 0)
    {
        const float part  = (instrumentCost / totalCost) * HUNDRED_PERCENT;
        const float yield = ((topBidPrice / instrumentAvgPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;

        if (!isNeedToSellAsap(QDateTime::currentMSecsSinceEpoch(), part, yield, commission))
        {
            const double minimumSellPrice =
                instrumentAvgPrice * (HUNDRED_PERCENT + MINIMUM_YIELD_PERCENT + (2 * commission)) / HUNDRED_PERCENT;

            for (int i = 0; i < tinkoffOrderBook.asks_size(); ++i)
            {
                if (tinkoffOrderBook.asks(i).quantity() > 0)
                {
                    const double curPrice = quotationToDouble(tinkoffOrderBook.asks(i).price());

                    if (curPrice >= minimumSellPrice)
                    {
                        res = curPrice;

                        break;
                    }
                }
            }
        }
    }

    return res;
}

qint64 BiDirTradingThread::calculateLotsToKeep(double totalCost, double bidPrice)
{
    const double lotPrice = mInstrumentLot * bidPrice;

    // TODO: Limits for huge bid mode

    const bool   limitStockPurchase     = mConfig->isHugeSpreadLimitStockPurchase();
    const double limitStockPurchasePart = mConfig->getHugeSpreadLimitStockPurchasePart();
    const bool   limitByTurnover        = mConfig->isHugeSpreadLimitByTurnover();
    const double limitByTurnoverPercent = mConfig->getHugeSpreadLimitByTurnoverPercent();

    mStock->readLock();
    const qint64 turnover = mStock->meta.turnover;
    mStock->readUnlock();

    return mTradeUtils->calculateAmountOfLotsToBuy(
        limitStockPurchase,
        limitStockPurchasePart,
        limitByTurnover,
        limitByTurnoverPercent,
        totalCost,
        totalCost,
        turnover,
        lotPrice,
        lotPrice
    );
}

IDecisionMakerConfig* BiDirTradingThread::chooseDecisionConfig()
{
    if (mConfig->isSimulatorConfigCommon())
    {
        return mConfig->getSimulatorConfig();
    }

    return mConfig->getAutoPilotConfig();
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
