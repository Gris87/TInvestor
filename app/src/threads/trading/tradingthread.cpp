#include "src/threads/trading/tradingthread.h"

#include <QDebug>

#include "src/grpc/utils.h"



constexpr float  HUNDRED_PERCENT             = 100.0f;
constexpr float  MAXIMUM_PRICE_RAISE_PERCENT = 0.50f;
constexpr float  MINIMUM_YIELD_PERCENT       = 0.30f;
constexpr qint64 MS_IN_SECOND                = 1000LL;
constexpr qint64 SLEEP_DELAY                 = 30LL * MS_IN_SECOND; // 30 seconds
constexpr qint64 ORDER_CANCEL_DELAY          = 3LL * MS_IN_SECOND;  // 3 seconds
constexpr qint64 ORDER_RETRY_DELAY           = 1LL * MS_IN_SECOND;  // 1 second
constexpr double DOUBLE_EPSILON              = 0.0001;



TradingThread::TradingThread(
    IInstrumentsStorage* instrumentsStorage,
    IUserStorage*        userStorage,
    ITimeUtils*          timeUtils,
    IGrpcClient*         grpcClient,
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
    mTimeUtils(timeUtils),
    mGrpcClient(grpcClient),
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
    mLastExpectedCost()
{
    qDebug() << "Create TradingThread";

    mLogsThread->addLog(LOG_LEVEL_DEBUG, mInstrumentId, cause);
}

TradingThread::~TradingThread()
{
    qDebug() << "Destroy TradingThread";

    delete mRwMutex;
}

void TradingThread::run()
{
    qDebug() << "Running TradingThread";

    blockSignals(false);

    if (trade())
    {
        mLogsThread->addLog(LOG_LEVEL_VERBOSE, mInstrumentId, tr("Trade completed successfully"));

        emit tradingCompleted(mInstrumentId);
    }

    qDebug() << "Finish TradingThread";
}

void TradingThread::setAsapMode(AsapMode asapMode)
{
    const QWriteLocker lock(mRwMutex);

    if (asapMode > mAsapMode)
    {
        mAsapMode = asapMode;
    }
}

void TradingThread::setAvgPrice(float avgPrice)
{
    const QWriteLocker lock(mRwMutex);

    if (mAvgPrice != avgPrice)
    {
        mAvgPrice = avgPrice;
    }
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
            mGrpcClient->getPortfolio(QThread::currentThread(), mAccountId);

        if (QThread::currentThread()->isInterruptionRequested() || tinkoffPortfolio == nullptr)
        {
            return false;
        }

        // TODO: Validate portfolio

        const double cost     = handlePortfolioResponse(*tinkoffPortfolio);
        const double expected = expectedCost();

        const double delta = expected - cost;

        bool completed = false;

        if (delta <= 0)
        {
            completed = sell(expected, -delta);
        }
        else
        {
            completed = buy(expected, delta);
        }

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

bool TradingThread::sell(double expected, double delta)
{
    const std::shared_ptr<tinkoff::GetOrderBookResponse> tinkoffOrderBook =
        mGrpcClient->getOrderBook(QThread::currentThread(), mInstrumentId, 1);

    if (QThread::currentThread()->isInterruptionRequested() || tinkoffOrderBook == nullptr)
    {
        return false;
    }

    const AsapMode mode  = asapMode();
    double         price = -1;

    if (mode == ASAP_MODE_IMMEDIATELY_TRADE && tinkoffOrderBook->bids_size() > 0)
    {
        price = quotationToDouble(tinkoffOrderBook->bids(0).price());
    }

    if (tinkoffOrderBook->asks_size() > 0 && price < 0)
    {
        price = quotationToDouble(tinkoffOrderBook->asks(0).price());

        if (mode == ASAP_MODE_NONE)
        {
            mUserStorage->readLock();
            const float commission = mUserStorage->getCommission();
            mUserStorage->readUnlock();

            price = qMax(price, avgPrice() * (HUNDRED_PERCENT + MINIMUM_YIELD_PERCENT + (2 * commission)) / HUNDRED_PERCENT);
        }
    }

    if (price > 0)
    {
        const qint64 coef = static_cast<qint64>(std::ceil(price / quotationToDouble(mMinPriceIncrement)));

        const float marketPrice = tinkoffOrderBook->bids_size() > 0 ? quotationToDouble(tinkoffOrderBook->bids(0).price()) : 0;

        return sellWithPrice(expected, delta, quotationMultiply(mMinPriceIncrement, coef), marketPrice);
    }

    return false;
}

bool TradingThread::sellWithPrice(double expected, double delta, const Quotation& price, float marketPrice)
{
    if (mOrderId == "" || mLastOrderPrice != price || qAbs(mLastExpectedCost - expected) >= DOUBLE_EPSILON)
    {
        if (mOrderId != "")
        {
            cancelOrder();

            if (mTimeUtils->interruptibleSleep(ORDER_CANCEL_DELAY, QThread::currentThread()))
            {
                return false;
            }
        }

        return sellWithPriceOptimalAmount(expected, delta, price, marketPrice);
    }

    const std::shared_ptr<tinkoff::OrderState> tinkoffOrder =
        mGrpcClient->getOrderState(QThread::currentThread(), mAccountId, mOrderId);

    if (!QThread::currentThread()->isInterruptionRequested() && tinkoffOrder != nullptr)
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
bool TradingThread::sellWithPriceOptimalAmount(double expected, double delta, const Quotation& price, float marketPrice)
{
    while (true)
    {
        const std::shared_ptr<tinkoff::GetMaxLotsResponse> tinkoffMaxLots =
            mGrpcClient->getMaxLots(QThread::currentThread(), mAccountId, mInstrumentId, price);

        if (QThread::currentThread()->isInterruptionRequested() || tinkoffMaxLots == nullptr)
        {
            return false;
        }

        qint64 amountToSell = tinkoffMaxLots->sell_limits().sell_max_lots();

        if (expected > 0)
        {
            const double lotPrice      = mInstrumentLot * quotationToDouble(price);
            const qint64 deltaQuantity = qRound64(delta / lotPrice);

            amountToSell = qMin(deltaQuantity, amountToSell);
        }

        if (amountToSell > 0)
        {
            const std::shared_ptr<tinkoff::PostOrderResponse> tinkoffOrder = mGrpcClient->postOrder(
                QThread::currentThread(), mAccountId, mInstrumentId, tinkoff::ORDER_DIRECTION_SELL, amountToSell, price
            );

            if (QThread::currentThread()->isInterruptionRequested() || tinkoffOrder == nullptr)
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

bool TradingThread::buy(double expected, double delta)
{
    const std::shared_ptr<tinkoff::GetOrderBookResponse> tinkoffOrderBook =
        mGrpcClient->getOrderBook(QThread::currentThread(), mInstrumentId, 1);

    if (QThread::currentThread()->isInterruptionRequested() || tinkoffOrderBook == nullptr)
    {
        return false;
    }

    const AsapMode mode  = asapMode();
    double         price = -1;

    if (mode == ASAP_MODE_IMMEDIATELY_TRADE && tinkoffOrderBook->asks_size() > 0)
    {
        price = quotationToDouble(tinkoffOrderBook->asks(0).price());
    }

    if (tinkoffOrderBook->bids_size() > 0 && price < 0)
    {
        price = quotationToDouble(tinkoffOrderBook->bids(0).price());

        if (mode == ASAP_MODE_NONE)
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

                return true;
            }
        }
    }

    if (price > 0)
    {
        const qint64 coef = static_cast<qint64>(std::floor(price / quotationToDouble(mMinPriceIncrement)));

        const float marketPrice = tinkoffOrderBook->asks_size() > 0 ? quotationToDouble(tinkoffOrderBook->asks(0).price()) : 0;

        return buyWithPrice(expected, delta, quotationMultiply(mMinPriceIncrement, coef), marketPrice);
    }

    return false;
}

bool TradingThread::buyWithPrice(double expected, double delta, const Quotation& price, float marketPrice)
{
    if (mOrderId == "" || mLastOrderPrice != price || qAbs(mLastExpectedCost - expected) >= DOUBLE_EPSILON)
    {
        if (mOrderId != "")
        {
            cancelOrder();

            if (mTimeUtils->interruptibleSleep(ORDER_CANCEL_DELAY, QThread::currentThread()))
            {
                return false;
            }
        }

        return buyWithPriceOptimalAmount(expected, delta, price, marketPrice);
    }

    const std::shared_ptr<tinkoff::OrderState> tinkoffOrder =
        mGrpcClient->getOrderState(QThread::currentThread(), mAccountId, mOrderId);

    if (!QThread::currentThread()->isInterruptionRequested() && tinkoffOrder != nullptr)
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

bool TradingThread::buyWithPriceOptimalAmount(double expected, double delta, const Quotation& price, float marketPrice)
{
    while (true)
    {
        const std::shared_ptr<tinkoff::GetMaxLotsResponse> tinkoffMaxLots =
            mGrpcClient->getMaxLots(QThread::currentThread(), mAccountId, mInstrumentId, price);

        if (QThread::currentThread()->isInterruptionRequested() || tinkoffMaxLots == nullptr)
        {
            return false;
        }

        const double lotPrice      = mInstrumentLot * quotationToDouble(price);
        const qint64 deltaQuantity = qRound64(delta / lotPrice);

        const qint64 amountToBuy = qMin(deltaQuantity, tinkoffMaxLots->buy_limits().buy_max_lots());

        if (amountToBuy > 0)
        {
            const std::shared_ptr<tinkoff::PostOrderResponse> tinkoffOrder = mGrpcClient->postOrder(
                QThread::currentThread(), mAccountId, mInstrumentId, tinkoff::ORDER_DIRECTION_BUY, amountToBuy, price
            );

            if (QThread::currentThread()->isInterruptionRequested() || tinkoffOrder == nullptr)
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

void TradingThread::cancelOrder()
{
    if (mOrderId != "")
    {
        const std::shared_ptr<tinkoff::OrderState> tinkoffOrder =
            mGrpcClient->getOrderState(QThread::currentThread(), mAccountId, mOrderId);

        if (!QThread::currentThread()->isInterruptionRequested() && tinkoffOrder != nullptr)
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
