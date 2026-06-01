#include "src/threads/trading/tradingthread.h"

#include <QDebug>

#include "src/grpc/utils.h"



constexpr float  HUNDRED_PERCENT             = 100.0f;
constexpr float  MAXIMUM_PRICE_RAISE_PERCENT = 0.30f;
constexpr float  MINIMUM_YIELD_PERCENT       = 0.40f;
constexpr int    ORDER_BOOK_DEPTH            = 20;
constexpr qint64 MS_IN_SECOND                = 1000LL;
constexpr qint64 SLEEP_DELAY                 = 30LL * MS_IN_SECOND; // 30 seconds
constexpr qint64 ORDER_CANCEL_DELAY          = 3LL * MS_IN_SECOND;  // 3 seconds
constexpr qint64 ORDER_RETRY_DELAY           = 1LL * MS_IN_SECOND;  // 1 second
constexpr double DOUBLE_EPSILON              = 0.0001;



TradingThread::TradingThread(
    IInstrumentsStorage* instrumentsStorage,
    IUserStorage*        userStorage,
    IConfig*             config,
    ITimeUtils*          timeUtils,
    IGrpcClient*         grpcClient,
    IGrpcRetryClient*    grpcRetryClient,
    ILogsThread*         logsThread,
    const QString&       accountId,
    const QString&       instrumentId,
    AsapMode             asapMode,
    float                avgPrice,
    float                price,
    double               expectedCost,
    const QString&       cause,
    QObject*             parent
) :
    ITradingThread(parent),
    mRwMutex(new QReadWriteLock()),
    mInstrumentsStorage(instrumentsStorage),
    mUserStorage(userStorage),
    mConfig(config),
    mTimeUtils(timeUtils),
    mGrpcClient(grpcClient),
    mGrpcRetryClient(grpcRetryClient),
    mLogsThread(logsThread),
    mAccountId(accountId),
    mInstrumentId(instrumentId),
    mAsapMode(asapMode),
    mAvgPrice(avgPrice),
    mPrice(price),
    mExpectedCost(expectedCost),
    mInstrumentLot(),
    mPricePrecision(),
    mMinPriceIncrement(),
    mOrderId(),
    mLastOrderPrice(),
    mLastCost(),
    mLastExpectedCost()
{
    qDebug() << "Create TradingThread";

    mLogsThread->addLog(LOG_LEVEL_DEBUG, mInstrumentId, cause);
}

TradingThread::~TradingThread()
{
    qDebug() << "Destroy TradingThread";

    cancelOrder();

    delete mRwMutex;
}

void TradingThread::run()
{
    qDebug() << "Running TradingThread";

    blockSignals(false);

    if (trade())
    {
        mLogsThread->addLog(LOG_LEVEL_VERBOSE, mInstrumentId, tr("Trade completed successfully"));

        mOrderId = "";

        emit tradingCompleted(mInstrumentId);
    }

    qDebug() << "Finish TradingThread";
}

void TradingThread::setAsapMode(AsapMode asapMode)
{
    const QWriteLocker lock(mRwMutex);

    mAsapMode = qMax(mAsapMode, asapMode);
}

void TradingThread::setAvgPrice(float avgPrice)
{
    const QWriteLocker lock(mRwMutex);

    mAvgPrice = avgPrice;
}

void TradingThread::setExpectedCost(double expectedCost, const QString& cause)
{
    const QWriteLocker lock(mRwMutex);

    if (mExpectedCost != expectedCost)
    {
        mExpectedCost = expectedCost;

        mLogsThread->addLog(LOG_LEVEL_DEBUG, mInstrumentId, cause);
    }
}

AsapMode TradingThread::asapMode() const
{
    const QReadLocker lock(mRwMutex);

    return mAsapMode;
}

float TradingThread::avgPrice() const
{
    const QReadLocker lock(mRwMutex);

    return mAvgPrice;
}

double TradingThread::expectedCost() const
{
    const QReadLocker lock(mRwMutex);

    return mExpectedCost;
}

void TradingThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}

bool TradingThread::trade()
{
    getInstrumentData();

    while (true)
    {
        const std::shared_ptr<tinkoff::PortfolioResponse> tinkoffPortfolio =
            mGrpcRetryClient->getValidPortfolio(QThread::currentThread(), mAccountId);

        if (QThread::currentThread()->isInterruptionRequested() || tinkoffPortfolio == nullptr)
        {
            return false;
        }

        const double cost     = handlePortfolioResponse(*tinkoffPortfolio);
        const double expected = expectedCost();

        const double delta     = expected - cost;
        const bool   completed = delta > 0 ? buy(cost, expected, delta) : sell(cost, expected, -delta);

        if (completed)
        {
            break;
        }

        if (mTimeUtils->interruptibleSleep(SLEEP_DELAY, QThread::currentThread()))
        {
            return false;
        }
    }

    return true;
}

void TradingThread::getInstrumentData()
{
    mInstrumentsStorage->readLock();

    const Instruments& instruments = mInstrumentsStorage->getInstruments();
    Q_ASSERT_X(instruments.contains(mInstrumentId), __FUNCTION__, "Data about instrument not found");
    const Instrument& instrument = instruments.value(mInstrumentId);

    mInstrumentLot     = instrument.lot;
    mPricePrecision    = instrument.pricePrecision;
    mMinPriceIncrement = instrument.minPriceIncrement;

    mInstrumentsStorage->readUnlock();
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

bool TradingThread::buy(double cost, double expected, double delta)
{
    const std::shared_ptr<tinkoff::GetOrderBookResponse> tinkoffOrderBook =
        mGrpcClient->getOrderBook(QThread::currentThread(), mInstrumentId, ORDER_BOOK_DEPTH);

    if (QThread::currentThread()->isInterruptionRequested() || tinkoffOrderBook == nullptr)
    {
        return false;
    }

    std::shared_ptr<tinkoff::OrderState> tinkoffOrder;

    if (mOrderId != "")
    {
        tinkoffOrder = mGrpcClient->getOrderState(QThread::currentThread(), mAccountId, mOrderId);
    }

    removeOwnOrdersFromOrderBook(*tinkoffOrderBook, tinkoffOrder);

    const Quotation price = calculateBuyPrice(*tinkoffOrderBook, asapMode(), cost < 0);

    if (price.units < 0)
    {
        return true;
    }

    if (price.units != 0 || price.nano != 0)
    {
        const float marketPrice = tinkoffOrderBook->asks_size() > 0 ? quotationToDouble(tinkoffOrderBook->asks(0).price()) : 0;

        return buyWithPrice(tinkoffOrder, cost, expected, delta, price, marketPrice);
    }

    return false;
}

bool TradingThread::buyWithPrice(
    const std::shared_ptr<tinkoff::OrderState>& tinkoffOrder,
    double                                      cost,
    double                                      expected,
    double                                      delta,
    const Quotation&                            price,
    float                                       marketPrice
)
{
    if (mOrderId == "" || mLastOrderPrice != price || mLastCost - cost >= DOUBLE_EPSILON ||
        qAbs(mLastExpectedCost - expected) >= DOUBLE_EPSILON)
    {
        if (mOrderId != "")
        {
            cancelOrder(tinkoffOrder);

            if (mTimeUtils->interruptibleSleep(ORDER_CANCEL_DELAY, QThread::currentThread()))
            {
                return false;
            }
        }

        return buyWithPriceOptimalAmount(cost, expected, delta, price, marketPrice);
    }

    if (tinkoffOrder != nullptr)
    {
        informAboutOrderState(*tinkoffOrder);

        const tinkoff::OrderExecutionReportStatus status = tinkoffOrder->execution_report_status();

        if (status == tinkoff::EXECUTION_REPORT_STATUS_FILL)
        {
            return true;
        }

        if (status == tinkoff::EXECUTION_REPORT_STATUS_REJECTED || status == tinkoff::EXECUTION_REPORT_STATUS_CANCELLED)
        {
            mOrderId = "";
        }
    }
    else
    {
        mOrderId = "";
    }

    return false;
}

// NOLINTBEGIN(readability-function-cognitive-complexity)
bool
TradingThread::buyWithPriceOptimalAmount(double cost, double expected, double delta, const Quotation& price, float marketPrice)
{
    while (!QThread::currentThread()->isInterruptionRequested())
    {
        const bool confirmMarginTrade = mConfig->isTradeWithMarginCall();

        const std::shared_ptr<tinkoff::GetMaxLotsResponse> tinkoffMaxLots =
            mGrpcClient->getMaxLots(QThread::currentThread(), mAccountId, mInstrumentId, price);

        if (QThread::currentThread()->isInterruptionRequested() || tinkoffMaxLots == nullptr)
        {
            return false;
        }

        qint64 amountToBuy =
            confirmMarginTrade
                ? qMax(tinkoffMaxLots->buy_limits().buy_max_lots(), tinkoffMaxLots->buy_margin_limits().buy_max_lots())
                : tinkoffMaxLots->buy_limits().buy_max_lots();

        if (expected != 0)
        {
            const double lotPrice      = mInstrumentLot * quotationToDouble(price);
            const qint64 deltaQuantity = qRound64(delta / lotPrice);

            amountToBuy = qMin(deltaQuantity, amountToBuy);
        }
        else
        {
            const double lotPrice      = mInstrumentLot * avgPrice();
            const qint64 deltaQuantity = qRound64(-cost / lotPrice);

            amountToBuy = qMin(deltaQuantity, amountToBuy);
        }

        if (amountToBuy > 0)
        {
            const std::shared_ptr<tinkoff::PostOrderResponse> tinkoffOrder = mGrpcClient->postOrder(
                QThread::currentThread(),
                mAccountId,
                mInstrumentId,
                tinkoff::ORDER_DIRECTION_BUY,
                amountToBuy,
                price,
                confirmMarginTrade,
                GRPC_PRIOIRITY_MAJOR
            );

            if (tinkoffOrder == nullptr)
            {
                mLogsThread->addLog(
                    LOG_LEVEL_WARNING,
                    mInstrumentId,
                    tr("Failed to create order to buy %1 with a price %2")
                        .arg(
                            QString::number(amountToBuy * mInstrumentLot),
                            QString::number(quotationToFloat(price), 'f', mPricePrecision) + " \u20BD"
                        )
                );

                if (mTimeUtils->interruptibleSleep(ORDER_RETRY_DELAY, QThread::currentThread()))
                {
                    return false;
                }

                continue;
            }

            if (tinkoffOrder->execution_report_status() != tinkoff::EXECUTION_REPORT_STATUS_REJECTED)
            {
                mLogsThread->addLog(
                    LOG_LEVEL_VERBOSE,
                    mInstrumentId,
                    tr("Order to buy %1 created with a price %2")
                            .arg(
                                QString::number(amountToBuy * mInstrumentLot),
                                QString::number(quotationToFloat(price), 'f', mPricePrecision) + " \u20BD"
                            ) +
                        (marketPrice > 0
                             ? " " +
                                   tr("while market price %1").arg(QString::number(marketPrice, 'f', mPricePrecision) + " \u20BD")
                             : "")
                );

                mOrderId          = QString::fromStdString(tinkoffOrder->order_id());
                mLastOrderPrice   = price;
                mLastCost         = cost;
                mLastExpectedCost = expected;

                break;
            }

            mLogsThread->addLog(
                LOG_LEVEL_DEBUG,
                mInstrumentId,
                tr("Order to buy %1 rejected with a price %2. Let's try again")
                    .arg(
                        QString::number(amountToBuy * mInstrumentLot),
                        QString::number(quotationToFloat(price), 'f', mPricePrecision) + " \u20BD"
                    )
            );

            if (mTimeUtils->interruptibleSleep(ORDER_RETRY_DELAY, QThread::currentThread()))
            {
                return false;
            }
        }
        else
        {
            return true;
        }
    }

    return false;
}
// NOLINTEND(readability-function-cognitive-complexity)

bool TradingThread::sell(double cost, double expected, double delta)
{
    const std::shared_ptr<tinkoff::GetOrderBookResponse> tinkoffOrderBook =
        mGrpcClient->getOrderBook(QThread::currentThread(), mInstrumentId, ORDER_BOOK_DEPTH);

    if (QThread::currentThread()->isInterruptionRequested() || tinkoffOrderBook == nullptr)
    {
        return false;
    }

    std::shared_ptr<tinkoff::OrderState> tinkoffOrder;

    if (mOrderId != "")
    {
        tinkoffOrder = mGrpcClient->getOrderState(QThread::currentThread(), mAccountId, mOrderId);
    }

    removeOwnOrdersFromOrderBook(*tinkoffOrderBook, tinkoffOrder);

    const Quotation price = calculateSellPrice(*tinkoffOrderBook, asapMode());

    if (price.units != 0 || price.nano != 0)
    {
        const float marketPrice = tinkoffOrderBook->bids_size() > 0 ? quotationToDouble(tinkoffOrderBook->bids(0).price()) : 0;

        return sellWithPrice(tinkoffOrder, cost, expected, delta, price, marketPrice);
    }

    return false;
}

bool TradingThread::sellWithPrice(
    const std::shared_ptr<tinkoff::OrderState>& tinkoffOrder,
    double                                      cost,
    double                                      expected,
    double                                      delta,
    const Quotation&                            price,
    float                                       marketPrice
)
{
    if (mOrderId == "" || mLastOrderPrice != price || cost - mLastCost >= DOUBLE_EPSILON ||
        qAbs(mLastExpectedCost - expected) >= DOUBLE_EPSILON)
    {
        if (mOrderId != "")
        {
            cancelOrder(tinkoffOrder);

            if (mTimeUtils->interruptibleSleep(ORDER_CANCEL_DELAY, QThread::currentThread()))
            {
                return false;
            }
        }

        return sellWithPriceOptimalAmount(cost, expected, delta, price, marketPrice);
    }

    if (tinkoffOrder != nullptr)
    {
        informAboutOrderState(*tinkoffOrder);

        const tinkoff::OrderExecutionReportStatus status = tinkoffOrder->execution_report_status();

        if (status == tinkoff::EXECUTION_REPORT_STATUS_FILL)
        {
            return true;
        }

        if (status == tinkoff::EXECUTION_REPORT_STATUS_REJECTED || status == tinkoff::EXECUTION_REPORT_STATUS_CANCELLED)
        {
            mOrderId = "";
        }
    }
    else
    {
        mOrderId = "";
    }

    return false;
}

// NOLINTBEGIN(readability-function-cognitive-complexity)
bool
TradingThread::sellWithPriceOptimalAmount(double cost, double expected, double delta, const Quotation& price, float marketPrice)
{
    while (!QThread::currentThread()->isInterruptionRequested())
    {
        const std::shared_ptr<tinkoff::GetMaxLotsResponse> tinkoffMaxLots =
            mGrpcClient->getMaxLots(QThread::currentThread(), mAccountId, mInstrumentId, price);

        if (QThread::currentThread()->isInterruptionRequested() || tinkoffMaxLots == nullptr)
        {
            return false;
        }

        qint64 amountToSell = tinkoffMaxLots->sell_limits().sell_max_lots();

        if (expected != 0)
        {
            const double lotPrice      = mInstrumentLot * quotationToDouble(price);
            const qint64 deltaQuantity = qRound64(delta / lotPrice);

            amountToSell = qMin(deltaQuantity, amountToSell);
        }

        if (amountToSell > 0)
        {
            const std::shared_ptr<tinkoff::PostOrderResponse> tinkoffOrder = mGrpcClient->postOrder(
                QThread::currentThread(),
                mAccountId,
                mInstrumentId,
                tinkoff::ORDER_DIRECTION_SELL,
                amountToSell,
                price,
                false,
                GRPC_PRIOIRITY_MAJOR
            );

            if (tinkoffOrder == nullptr)
            {
                mLogsThread->addLog(
                    LOG_LEVEL_WARNING,
                    mInstrumentId,
                    tr("Failed to create order to sell %1 with a price %2")
                        .arg(
                            QString::number(amountToSell * mInstrumentLot),
                            QString::number(quotationToFloat(price), 'f', mPricePrecision) + " \u20BD"
                        )
                );

                if (mTimeUtils->interruptibleSleep(ORDER_RETRY_DELAY, QThread::currentThread()))
                {
                    return false;
                }

                continue;
            }

            if (tinkoffOrder->execution_report_status() != tinkoff::EXECUTION_REPORT_STATUS_REJECTED)
            {
                mLogsThread->addLog(
                    LOG_LEVEL_VERBOSE,
                    mInstrumentId,
                    tr("Order to sell %1 created with a price %2")
                            .arg(
                                QString::number(amountToSell * mInstrumentLot),
                                QString::number(quotationToFloat(price), 'f', mPricePrecision) + " \u20BD"
                            ) +
                        (marketPrice > 0
                             ? " " +
                                   tr("while market price %1").arg(QString::number(marketPrice, 'f', mPricePrecision) + " \u20BD")
                             : "")
                );

                mOrderId          = QString::fromStdString(tinkoffOrder->order_id());
                mLastOrderPrice   = price;
                mLastCost         = cost;
                mLastExpectedCost = expected;

                break;
            }

            mLogsThread->addLog(
                LOG_LEVEL_DEBUG,
                mInstrumentId,
                tr("Order to sell %1 rejected with a price %2. Let's try again")
                    .arg(
                        QString::number(amountToSell * mInstrumentLot),
                        QString::number(quotationToFloat(price), 'f', mPricePrecision) + " \u20BD"
                    )
            );

            if (mTimeUtils->interruptibleSleep(ORDER_RETRY_DELAY, QThread::currentThread()))
            {
                return false;
            }
        }
        else
        {
            return true;
        }
    }

    return false;
}
// NOLINTEND(readability-function-cognitive-complexity)

void TradingThread::removeOwnOrdersFromOrderBook(
    tinkoff::GetOrderBookResponse& tinkoffOrderBook, const std::shared_ptr<tinkoff::OrderState>& tinkoffOrder
)
{
    if (tinkoffOrder != nullptr)
    {
        const Quotation orderPrice    = quotationConvert(tinkoffOrder->initial_security_price());
        const qint64    lotsRemaining = tinkoffOrder->lots_requested() - tinkoffOrder->lots_executed();

        for (int i = 0; i < tinkoffOrderBook.bids_size(); ++i)
        {
            const tinkoff::Order& order = tinkoffOrderBook.bids(i);

            if (quotationConvert(order.price()) == orderPrice)
            {
                // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
                const_cast<tinkoff::Order&>(order).set_quantity(order.quantity() - lotsRemaining);

                return;
            }
        }

        for (int i = 0; i < tinkoffOrderBook.asks_size(); ++i)
        {
            const tinkoff::Order& order = tinkoffOrderBook.asks(i);

            if (quotationConvert(order.price()) == orderPrice)
            {
                // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
                const_cast<tinkoff::Order&>(order).set_quantity(order.quantity() - lotsRemaining);

                return;
            }
        }
    }
}

// NOLINTBEGIN(readability-function-cognitive-complexity)
Quotation TradingThread::calculateBuyPrice(const tinkoff::GetOrderBookResponse& tinkoffOrderBook, AsapMode mode, bool isShort)
{
    Quotation res;

    Quotation limitPrice;
    double    price = -1;

    if (mode == ASAP_MODE_IMMEDIATELY_TRADE && tinkoffOrderBook.asks_size() > 0)
    {
        limitPrice = quotationConvert(tinkoffOrderBook.asks(0).price());
        price      = quotationToDouble(limitPrice);
    }

    if (tinkoffOrderBook.bids_size() > 0 && price < 0)
    {
        limitPrice = quotationConvert(
            tinkoffOrderBook.bids_size() == 1 || tinkoffOrderBook.bids(0).quantity() > 0 ? tinkoffOrderBook.bids(0).price()
                                                                                         : tinkoffOrderBook.bids(1).price()
        );
        price = quotationToDouble(limitPrice);

        if (mode == ASAP_MODE_NONE)
        {
            if (!isShort)
            {
                const double priceRaise = ((price / mPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;

                if (priceRaise > MAXIMUM_PRICE_RAISE_PERCENT)
                {
                    mLogsThread->addLog(
                        LOG_LEVEL_DEBUG,
                        mInstrumentId,
                        tr("Trade interrupted because the price reached %1 with raise %2 from the price %3")
                            .arg(
                                QString::number(price, 'f', mPricePrecision) + " \u20BD",
                                "+" + QString::number(priceRaise, 'f', 2) + "%",
                                QString::number(mPrice, 'f', mPricePrecision) + " \u20BD"
                            )
                    );

                    cancelOrder();
                    res.units = -1;

                    return res;
                }

                const float additionalGap = mConfig->isAdditionalGap() ? mConfig->getAdditionalGapPercent() : 0;
                price                     = price * (1 - (additionalGap / HUNDRED_PERCENT));
            }
            else
            {
                mUserStorage->readLock();
                const float commission = mUserStorage->getCommission();
                mUserStorage->readUnlock();

                const double maximumBuyPrice = avgPrice() / (1 + (MINIMUM_YIELD_PERCENT + (2 * commission)) / HUNDRED_PERCENT);
                price                        = maximumBuyPrice;

                for (int i = 0; i < tinkoffOrderBook.bids_size(); ++i)
                {
                    if (tinkoffOrderBook.bids(i).quantity() > 0)
                    {
                        const double curPrice = quotationToDouble(tinkoffOrderBook.bids(i).price());

                        if (curPrice <= maximumBuyPrice)
                        {
                            price = curPrice;

                            break;
                        }
                    }
                }
            }
        }
    }

    if (price > 0)
    {
        const qint64 coef = qRound64(price / quotationToDouble(mMinPriceIncrement));
        res               = quotationMultiply(mMinPriceIncrement, coef);

        if (mode == ASAP_MODE_IMMEDIATELY_TRADE && tinkoffOrderBook.asks_size() > 0)
        {
            res = qMax(res, limitPrice);
        }
        else
        {
            res = qMin(res, limitPrice);
        }
    }

    return res;
}
// NOLINTEND(readability-function-cognitive-complexity)

// NOLINTBEGIN(readability-function-cognitive-complexity)
Quotation TradingThread::calculateSellPrice(const tinkoff::GetOrderBookResponse& tinkoffOrderBook, AsapMode mode)
{
    Quotation res;

    Quotation limitPrice;
    double    price = -1;

    if ((mode == ASAP_MODE_IMMEDIATELY_WITH_TINY_YIELD || mode == ASAP_MODE_IMMEDIATELY_TRADE) &&
        tinkoffOrderBook.bids_size() > 0)
    {
        limitPrice               = quotationConvert(tinkoffOrderBook.bids(0).price());
        const double topBidPrice = quotationToDouble(limitPrice);

        if (mode == ASAP_MODE_IMMEDIATELY_TRADE || topBidPrice >= avgPrice() + DOUBLE_EPSILON)
        {
            price = topBidPrice;
        }
    }

    if (tinkoffOrderBook.asks_size() > 0 && price < 0)
    {
        limitPrice = quotationConvert(
            tinkoffOrderBook.asks_size() == 1 || tinkoffOrderBook.asks(0).quantity() > 0 ? tinkoffOrderBook.asks(0).price()
                                                                                         : tinkoffOrderBook.asks(1).price()
        );
        price = quotationToDouble(limitPrice);

        if (mode == ASAP_MODE_NONE || mode == ASAP_MODE_IMMEDIATELY_WITH_TINY_YIELD)
        {
            mUserStorage->readLock();
            const float commission = mUserStorage->getCommission();
            mUserStorage->readUnlock();

            const double minimumSellPrice = avgPrice() * (1 + (MINIMUM_YIELD_PERCENT + (2 * commission)) / HUNDRED_PERCENT);
            price                         = minimumSellPrice;

            for (int i = 0; i < tinkoffOrderBook.asks_size(); ++i)
            {
                if (tinkoffOrderBook.asks(i).quantity() > 0)
                {
                    const double curPrice = quotationToDouble(tinkoffOrderBook.asks(i).price());

                    if (curPrice >= minimumSellPrice)
                    {
                        price = curPrice;

                        break;
                    }
                }
            }
        }
    }

    if (price > 0)
    {
        const qint64 coef = qRound64(price / quotationToDouble(mMinPriceIncrement));
        res               = quotationMultiply(mMinPriceIncrement, coef);

        if (mode == ASAP_MODE_IMMEDIATELY_TRADE && tinkoffOrderBook.bids_size() > 0)
        {
            res = qMin(res, limitPrice);
        }
        else
        {
            res = qMax(res, limitPrice);
        }
    }

    return res;
}
// NOLINTEND(readability-function-cognitive-complexity)

void TradingThread::cancelOrder()
{
    if (mOrderId != "")
    {
        const std::shared_ptr<tinkoff::OrderState> tinkoffOrder =
            mGrpcClient->getOrderState(QThread::currentThread(), mAccountId, mOrderId);
        cancelOrder(tinkoffOrder);
    }
}

void TradingThread::cancelOrder(const std::shared_ptr<tinkoff::OrderState>& tinkoffOrder)
{
    if (tinkoffOrder != nullptr)
    {
        informAboutOrderState(*tinkoffOrder);

        if (tinkoffOrder->execution_report_status() != tinkoff::EXECUTION_REPORT_STATUS_FILL)
        {
            mLogsThread->addLog(LOG_LEVEL_VERBOSE, mInstrumentId, tr("Order cancelled"));

            mGrpcClient->cancelOrder(QThread::currentThread(), mAccountId, mOrderId);
        }
    }

    mOrderId = "";
}

void TradingThread::informAboutOrderState(const tinkoff::OrderState& tinkoffOrder)
{
    QString details;

    if (tinkoffOrder.direction() == tinkoff::ORDER_DIRECTION_BUY)
    {
        details =
            tr("%1/%2 bought with a price %3")
                .arg(
                    QString::number(tinkoffOrder.lots_executed() * mInstrumentLot),
                    QString::number(tinkoffOrder.lots_requested() * mInstrumentLot),
                    QString::number(quotationToFloat(tinkoffOrder.initial_security_price()), 'f', mPricePrecision) + " \u20BD"
                );
    }
    else
    {
        details =
            tr("%1/%2 sold with a price %3")
                .arg(
                    QString::number(tinkoffOrder.lots_executed() * mInstrumentLot),
                    QString::number(tinkoffOrder.lots_requested() * mInstrumentLot),
                    QString::number(quotationToFloat(tinkoffOrder.initial_security_price()), 'f', mPricePrecision) + " \u20BD"
                );
    }

    const tinkoff::OrderExecutionReportStatus status = tinkoffOrder.execution_report_status();

    if (status == tinkoff::EXECUTION_REPORT_STATUS_FILL)
    {
        mLogsThread->addLog(LOG_LEVEL_VERBOSE, mInstrumentId, tr("Order completed") + ". " + details);
    }
    else if (status == tinkoff::EXECUTION_REPORT_STATUS_PARTIALLYFILL)
    {
        mLogsThread->addLog(LOG_LEVEL_VERBOSE, mInstrumentId, tr("Order partially completed") + ". " + details);
    }
    else if (status == tinkoff::EXECUTION_REPORT_STATUS_REJECTED)
    {
        mLogsThread->addLog(LOG_LEVEL_VERBOSE, mInstrumentId, tr("Order rejected") + ". " + details);
    }
    else if (status == tinkoff::EXECUTION_REPORT_STATUS_CANCELLED)
    {
        mLogsThread->addLog(LOG_LEVEL_VERBOSE, mInstrumentId, tr("Order cancelled") + ". " + details);
    }
    else if (status == tinkoff::EXECUTION_REPORT_STATUS_NEW)
    {
        mLogsThread->addLog(LOG_LEVEL_VERBOSE, mInstrumentId, tr("Order is still waiting") + ". " + details);
    }
}
