#include "src/threads/bidirtrading/bidirtradingthread.h"

#include <QDebug>

#include "src/grpc/utils.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";

constexpr float  HUNDRED_PERCENT             = 100.0f;
constexpr float  MINIMUM_YIELD_PERCENT       = 0.10f;
constexpr float  MAXIMUM_LOSE_PERCENT        = 1.50f;
constexpr float  REQUIRED_PRICE_FALL_PERCENT = 0.50f;
constexpr int    MINUTES_TO_DOUBLE_CHECK     = 5;
constexpr qint64 MS_IN_SECOND                = 1000LL;
constexpr qint64 SLEEP_DELAY                 = 30LL * MS_IN_SECOND; // 30 seconds
constexpr qint64 ORDER_CANCEL_DELAY          = 3LL * MS_IN_SECOND;  // 3 seconds
constexpr qint64 ORDER_RETRY_DELAY           = 1LL * MS_IN_SECOND;  // 1 second



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
    const float commission = mUserStorage->getCommission();
    mUserStorage->readUnlock();

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

        const std::shared_ptr<tinkoff::PortfolioResponse> tinkoffPortfolio =
            mGrpcRetryClient->getValidPortfolio(QThread::currentThread(), mAccountId);

        if (QThread::currentThread()->isInterruptionRequested() || tinkoffPortfolio == nullptr)
        {
            return false;
        }

        double totalCost          = 0.0;
        double instrumentCost     = 0.0;
        qint64 instrumentLots     = 0;
        double instrumentAvgPrice = -1.0;

        calculateTotalCostAndInstrumentCost(*tinkoffPortfolio, totalCost, instrumentCost, instrumentLots, instrumentAvgPrice);

        double      bidPrice = quotationToDouble(tinkoffOrderBook->bids(0).price());
        double      askPrice = quotationToDouble(tinkoffOrderBook->asks(0).price());
        const float spread   = ((askPrice / bidPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;

        if (instrumentAvgPrice > 0)
        {
            const float part  = (instrumentCost / totalCost) * HUNDRED_PERCENT;
            const float yield = ((bidPrice / instrumentAvgPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;

            if (!isNeedToSellAsap(QDateTime::currentMSecsSinceEpoch(), part, yield, commission))
            {
                askPrice = qMax(
                    askPrice, instrumentAvgPrice * (HUNDRED_PERCENT + MINIMUM_YIELD_PERCENT + (2 * commission)) / HUNDRED_PERCENT
                );
            }
        }

        int maxQuantity = 0;

        for (int i = 0; i < tinkoffOrderBook->bids_size(); ++i)
        {
            const tinkoff::Order& bid = tinkoffOrderBook->bids(i);

            if (bid.quantity() > maxQuantity)
            {
                maxQuantity = bid.quantity();
                bidPrice    = quotationToDouble(bid.price());
            }
        }

        const qint64 coefBuy  = qRound64(bidPrice / quotationToDouble(mMinPriceIncrement));
        const qint64 coefSell = static_cast<qint64>(std::ceil(askPrice / quotationToDouble(mMinPriceIncrement)));

        const Quotation buyPrice  = quotationMultiply(mMinPriceIncrement, coefBuy);
        const Quotation sellPrice = quotationMultiply(mMinPriceIncrement, coefSell);

        qint64 lotsToBuy = 0;

        if (spread > mConfig->getHugeSpread() && isGoodToBuy(bidPrice))
        {
            for (int i = 0; i < tinkoffOrderBook->bids_size(); ++i)
            {
                lotsToBuy = qMax(lotsToBuy, tinkoffOrderBook->bids(i).quantity());
            }

            for (int i = 0; i < tinkoffOrderBook->asks_size(); ++i)
            {
                lotsToBuy = qMax(lotsToBuy, tinkoffOrderBook->asks(i).quantity());
            }

            const bool   limitStockPurchase     = mConfig->isHugeSpreadLimitStockPurchase();
            const double limitStockPurchasePart = mConfig->getHugeSpreadLimitStockPurchasePart();
            const bool   limitByTurnover        = mConfig->isHugeSpreadLimitByTurnover();
            const double limitByTurnoverPercent = mConfig->getHugeSpreadLimitByTurnoverPercent();

            const double lotPrice = mInstrumentLot * bidPrice;

            mStock->readLock();
            const qint64 turnover = mStock->meta.turnover;
            mStock->readUnlock();

            const qint64 lotsToKeep = mTradeUtils->calculateAmountOfLotsToBuy(
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
            lotsToBuy = qMax(qMin(lotsToKeep - instrumentLots, lotsToBuy), 0);
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

bool BiDirTradingThread::isGoodToBuy(float price)
{
    const float maximumPrice = price / (1 - (REQUIRED_PRICE_FALL_PERCENT / HUNDRED_PERCENT));

    mStock->readLock();

    for (int i = qMax(mStock->operational.detailedData.size() - MINUTES_TO_DOUBLE_CHECK, 0);
         i < mStock->operational.detailedData.size();
         ++i)
    {
        if (mStock->operational.detailedData.at(i).price > maximumPrice)
        {
            mStock->readUnlock();

            return true;
        }
    }

    for (int i = qMax(mStock->data.size() - MINUTES_TO_DOUBLE_CHECK, 0); i < mStock->data.size(); ++i)
    {
        if (mStock->data.at(i).price > maximumPrice)
        {
            mStock->readUnlock();

            return true;
        }
    }

    mStock->readUnlock();

    return false;
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
