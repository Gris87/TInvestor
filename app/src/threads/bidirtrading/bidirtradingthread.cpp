#include "src/threads/bidirtrading/bidirtradingthread.h"

#include <QDebug>

#include "src/grpc/utils.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";

constexpr double HUNDRED_PERCENT                     = 100.0;
constexpr double MINIMUM_YIELD_PERCENT               = 0.10;
constexpr double MAXIMUM_LOSE_PERCENT                = 2.00;
constexpr double MINIMUM_BID_PERCENT_FOR_HUGE_BID    = 80.0;
constexpr double MINIMUM_BID_PERCENT_FOR_HUGE_SPREAD = 25.0;
constexpr double SPREAD_FOR_HUGE_BID                 = 0.30;
constexpr int    ORDER_BOOK_DEPTH                    = 20;
constexpr qint64 MS_IN_SECOND                        = 1000LL;
constexpr qint64 SLEEP_DELAY                         = 30LL * MS_IN_SECOND; // 30 seconds
constexpr qint64 ORDER_CANCEL_DELAY                  = 3LL * MS_IN_SECOND;  // 3 seconds
constexpr qint64 ORDER_RETRY_DELAY                   = 1LL * MS_IN_SECOND;  // 1 second



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

// NOLINTBEGIN(readability-function-cognitive-complexity)
bool BiDirTradingThread::trade()
{
    getInstrumentData();

    mUserStorage->readLock();
    const double commission = mUserStorage->getCommission();
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

        std::shared_ptr<tinkoff::OrderState> tinkoffBuyOrder;
        std::shared_ptr<tinkoff::OrderState> tinkoffSellOrder;

        if (mBuyOrderId != "")
        {
            tinkoffBuyOrder = mGrpcClient->getOrderState(QThread::currentThread(), mAccountId, mBuyOrderId);
        }

        if (mSellOrderId != "")
        {
            tinkoffSellOrder = mGrpcClient->getOrderState(QThread::currentThread(), mAccountId, mSellOrderId);
        }

        if (tinkoffBuyOrder != nullptr || tinkoffSellOrder != nullptr)
        {
            removeOwnOrdersFromOrderBook(tinkoffOrderBook.get(), tinkoffBuyOrder, tinkoffSellOrder);
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

        checkIfNeedToCancelAndCreateOrder(tinkoffBuyOrder, lotsToBuy, buyPrice, needToCancelBuy, needToOrderBuy);
        checkIfNeedToCancelAndCreateOrder(tinkoffSellOrder, lotsToSell, sellPrice, needToCancelSell, needToOrderSell);

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
// NOLINTEND(readability-function-cognitive-complexity)

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
    const std::shared_ptr<tinkoff::OrderState>& tinkoffOrder,
    qint64                                      amountOfLots,
    const Quotation&                            price,
    bool&                                       needToCancel,
    bool&                                       needToOrder
)
{
    needToCancel = false;
    needToOrder  = amountOfLots > 0;

    if (tinkoffOrder != nullptr)
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

bool BiDirTradingThread::isNeedToSellAsap(qint64 timestamp, BiDirMode mode, double part, double yield, double commission)
{
    if (mTimeUtils->isMorningSession(timestamp))
    {
        return false;
    }

    ISellDecision3Config* sellDecision3Config = chooseDecisionConfig()->getSellDecision3Config();

    if (sellDecision3Config->isEnabled() && yield < -sellDecision3Config->getLoseYield() + (2 * commission))
    {
        return true;
    }

    if (mode == BIDIR_MODE_HUGE_BID)
    {
        return mConfig->isHugeBidLimitStockPurchase() && part < mConfig->getHugeBidLimitStockPurchasePart() * 2 &&
               yield < -MAXIMUM_LOSE_PERCENT;
    }

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
            const double cost =
                quotationToDouble(position.quantity()) * quotationToDouble(position.average_position_price_fifo());

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

void BiDirTradingThread::removeOwnOrdersFromOrderBook(
    tinkoff::GetOrderBookResponse*              tinkoffOrderBook,
    const std::shared_ptr<tinkoff::OrderState>& tinkoffBuyOrder,
    const std::shared_ptr<tinkoff::OrderState>& tinkoffSellOrder
)
{
    if (tinkoffBuyOrder != nullptr)
    {
        const Quotation orderPrice    = quotationConvert(tinkoffBuyOrder->initial_security_price());
        const qint64    lotsRemaining = tinkoffBuyOrder->lots_requested() - tinkoffBuyOrder->lots_executed();

        for (int i = 0; i < tinkoffOrderBook->bids_size(); ++i)
        {
            const tinkoff::Order& order = tinkoffOrderBook->bids(i);

            if (quotationConvert(order.price()) == orderPrice)
            {
                // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
                const_cast<tinkoff::Order&>(order).set_quantity(order.quantity() - lotsRemaining);

                break;
            }
        }
    }

    if (tinkoffSellOrder != nullptr)
    {
        const Quotation orderPrice    = quotationConvert(tinkoffSellOrder->initial_security_price());
        const qint64    lotsRemaining = tinkoffSellOrder->lots_requested() - tinkoffSellOrder->lots_executed();

        for (int i = 0; i < tinkoffOrderBook->asks_size(); ++i)
        {
            const tinkoff::Order& order = tinkoffOrderBook->asks(i);

            if (quotationConvert(order.price()) == orderPrice)
            {
                // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
                const_cast<tinkoff::Order&>(order).set_quantity(order.quantity() - lotsRemaining);

                break;
            }
        }
    }
}

void BiDirTradingThread::calculateBuySellPriceAndLots(
    const tinkoff::GetOrderBookResponse& tinkoffOrderBook,
    const tinkoff::PortfolioResponse&    tinkoffPortfolio,
    double                               commission,
    qint64&                              lotsToBuy,
    qint64&                              lotsToSell,
    Quotation&                           buyPrice,
    Quotation&                           sellPrice
)
{
    const BiDirMode mode = bidirMode();

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

    const double bidPrice = calculateBidPrice(tinkoffOrderBook, mode, maxQuantity);
    const double askPrice = calculateAskPrice(tinkoffOrderBook, mode, totalCost, instrumentCost, instrumentAvgPrice, commission);
    const qint64 lotsToKeep = calculateLotsToKeep(mode, totalCost, bidPrice);

    const qint64 coefBuy  = qRound64(bidPrice / quotationToDouble(mMinPriceIncrement));
    const qint64 coefSell = static_cast<qint64>(std::ceil(askPrice / quotationToDouble(mMinPriceIncrement)));

    lotsToBuy  = qMax(qMin(lotsToKeep - instrumentLots, maxQuantity), 0);
    lotsToSell = instrumentLots;

    buyPrice  = quotationMultiply(mMinPriceIncrement, coefBuy);
    sellPrice = quotationMultiply(mMinPriceIncrement, coefSell);
}

double
BiDirTradingThread::calculateBidPrice(const tinkoff::GetOrderBookResponse& tinkoffOrderBook, BiDirMode mode, qint64 maxQuantity)
{
    const double topBidPrice = quotationToDouble(tinkoffOrderBook.bids(0).price());
    const double topAskPrice = quotationToDouble(tinkoffOrderBook.asks(0).price());

    double res = topBidPrice;

    if (mode == BIDIR_MODE_HUGE_BID)
    {
        const double hugeBid = mConfig->getHugeBid();

        qint64 bids = 0;
        qint64 asks = 0;

        for (int i = 0; i < tinkoffOrderBook.bids_size(); ++i)
        {
            bids += tinkoffOrderBook.bids(i).quantity();
        }

        for (int i = 0; i < tinkoffOrderBook.asks_size(); ++i)
        {
            asks += tinkoffOrderBook.asks(i).quantity();
        }

        if (bids > 0 && asks > 0)
        {
            const double coef = static_cast<double>(bids) / static_cast<double>(asks);

            if (coef > hugeBid)
            {
                for (int i = 0; i < tinkoffOrderBook.bids_size(); ++i)
                {
                    if (tinkoffOrderBook.bids(i).quantity() > 0)
                    {
                        const double quantityPercent = (tinkoffOrderBook.bids(i).quantity() * HUNDRED_PERCENT) / maxQuantity;

                        if (quantityPercent >= MINIMUM_BID_PERCENT_FOR_HUGE_BID)
                        {
                            res = quotationToDouble(tinkoffOrderBook.bids(i).price());

                            break;
                        }
                    }
                }
            }
            else
            {
                for (int i = 0; i < tinkoffOrderBook.bids_size(); ++i)
                {
                    if (tinkoffOrderBook.bids(i).quantity() > 0)
                    {
                        const double quantityPercent = (tinkoffOrderBook.bids(i).quantity() * HUNDRED_PERCENT) / maxQuantity;

                        if (quantityPercent >= MINIMUM_BID_PERCENT_FOR_HUGE_BID)
                        {
                            const double curPrice = quotationToDouble(tinkoffOrderBook.bids(i).price());
                            const double spread   = ((topAskPrice / curPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;

                            res = curPrice;

                            if (spread >= SPREAD_FOR_HUGE_BID)
                            {
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        const double hugeSpread = mConfig->getHugeSpread();

        for (int i = 0; i < tinkoffOrderBook.bids_size(); ++i)
        {
            if (tinkoffOrderBook.bids(i).quantity() > 0)
            {
                const double quantityPercent = (tinkoffOrderBook.bids(i).quantity() * HUNDRED_PERCENT) / maxQuantity;

                if (quantityPercent >= MINIMUM_BID_PERCENT_FOR_HUGE_SPREAD)
                {
                    const double curPrice = quotationToDouble(tinkoffOrderBook.bids(i).price());
                    const double spread   = ((topAskPrice / curPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;

                    res = curPrice;

                    if (spread >= hugeSpread)
                    {
                        break;
                    }
                }
            }
        }
    }

    return res;
}

double BiDirTradingThread::calculateAskPrice(
    const tinkoff::GetOrderBookResponse& tinkoffOrderBook,
    BiDirMode                            mode,
    double                               totalCost,
    double                               instrumentCost,
    double                               instrumentAvgPrice,
    double                               commission
)
{
    const double topBidPrice = quotationToDouble(tinkoffOrderBook.bids(0).price());
    const double topAskPrice = quotationToDouble(tinkoffOrderBook.asks(0).price());

    double res = topAskPrice;

    if (instrumentAvgPrice > 0)
    {
        const double part  = (instrumentCost / totalCost) * HUNDRED_PERCENT;
        const double yield = ((topBidPrice / instrumentAvgPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;

        if (!isNeedToSellAsap(QDateTime::currentMSecsSinceEpoch(), mode, part, yield, commission))
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

qint64 BiDirTradingThread::calculateLotsToKeep(BiDirMode mode, double totalCost, double bidPrice)
{
    const double lotPrice = mInstrumentLot * bidPrice;

    const bool limitStockPurchase =
        mode == BIDIR_MODE_HUGE_BID ? mConfig->isHugeBidLimitStockPurchase() : mConfig->isHugeSpreadLimitStockPurchase();
    const double limitStockPurchasePart = mode == BIDIR_MODE_HUGE_BID ? mConfig->getHugeBidLimitStockPurchasePart()
                                                                      : mConfig->getHugeSpreadLimitStockPurchasePart();
    const bool   limitByTurnover =
        mode == BIDIR_MODE_HUGE_BID ? mConfig->isHugeBidLimitByTurnover() : mConfig->isHugeSpreadLimitByTurnover();
    const double limitByTurnoverPercent = mode == BIDIR_MODE_HUGE_BID ? mConfig->getHugeBidLimitByTurnoverPercent()
                                                                      : mConfig->getHugeSpreadLimitByTurnoverPercent();

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
