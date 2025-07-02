#include "src/threads/trading/tradingthread.h"

#include <QDebug>

#include "src/grpc/utils.h"



constexpr qint64 MS_IN_SECOND      = 1000LL;
constexpr qint64 SLEEP_DELAY       = 30LL * MS_IN_SECOND; // 30 seconds
constexpr qint64 ORDER_RETRY_DELAY = 1LL * MS_IN_SECOND;  // 1 second



TradingThread::TradingThread(
    IInstrumentsStorage* instrumentsStorage,
    IGrpcClient*         grpcClient,
    ILogsThread*         logsThread,
    ITimeUtils*          timeUtils,
    const QString&       accountId,
    const QString&       instrumentId,
    double               expectedCost,
    const QString&       cause,
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
    mExpectedCost(expectedCost),
    mInstrumentLot(),
    mPricePrecision(),
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

    delete mMutex;
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

void TradingThread::setExpectedCost(double expectedCost, const QString& cause)
{
    const QMutexLocker lock(mMutex);

    if (mExpectedCost != expectedCost)
    {
        mExpectedCost = expectedCost;

        mLogsThread->addLog(LOG_LEVEL_DEBUG, mInstrumentId, cause);
    }
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
    getInstrumentData();

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

    const Instruments& instrumentsData = mInstrumentsStorage->getInstruments();
    Q_ASSERT_X(instrumentsData.contains(mInstrumentId), __FUNCTION__, "Data about instrument not found");
    const Instrument& instrument = instrumentsData.value(mInstrumentId);

    mInstrumentLot  = instrument.lot;
    mPricePrecision = instrument.pricePrecision;

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

    if (tinkoffOrderBook->asks_size() > 0)
    {
        return sellWithPrice(expected, delta, quotationConvert(tinkoffOrderBook->asks(0).price()));
    }

    return false;
}

bool TradingThread::sellWithPrice(double expected, double delta, const Quotation& price)
{
    if (mOrderId == "" || mLastOrderPrice != price || mLastExpectedCost != expected)
    {
        cancelOrder();

        return sellWithPriceOptimalAmount(expected, delta, price);
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

bool TradingThread::sellWithPriceOptimalAmount(double expected, double delta, const Quotation& price)
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
                return false;
            }

            if (tinkoffOrder->execution_report_status() != tinkoff::EXECUTION_REPORT_STATUS_REJECTED)
            {
                mLogsThread->addLog(
                    LOG_LEVEL_VERBOSE,
                    mInstrumentId,
                    tr("Order to sell %1 created with a price %2 %3")
                        .arg(
                            QString::number(amountToSell * mInstrumentLot),
                            QString::number(quotationToFloat(price), 'f', mPricePrecision),
                            "\u20BD"
                        )
                );

                mOrderId          = QString::fromStdString(tinkoffOrder->order_id());
                mLastOrderPrice   = price;
                mLastExpectedCost = expected;

                break;
            }

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

bool TradingThread::buy(double expected, double delta)
{
    const std::shared_ptr<tinkoff::GetOrderBookResponse> tinkoffOrderBook =
        mGrpcClient->getOrderBook(QThread::currentThread(), mInstrumentId, 1);

    if (QThread::currentThread()->isInterruptionRequested() || tinkoffOrderBook == nullptr)
    {
        return false;
    }

    if (tinkoffOrderBook->bids_size() > 0)
    {
        return buyWithPrice(expected, delta, quotationConvert(tinkoffOrderBook->bids(0).price()));
    }

    return false;
}

bool TradingThread::buyWithPrice(double expected, double delta, const Quotation& price)
{
    if (mOrderId == "" || mLastOrderPrice != price || mLastExpectedCost != expected)
    {
        cancelOrder();

        return buyWithPriceOptimalAmount(expected, delta, price);
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

bool TradingThread::buyWithPriceOptimalAmount(double expected, double delta, const Quotation& price)
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
                return false;
            }

            if (tinkoffOrder->execution_report_status() != tinkoff::EXECUTION_REPORT_STATUS_REJECTED)
            {
                mLogsThread->addLog(
                    LOG_LEVEL_VERBOSE,
                    mInstrumentId,
                    tr("Order to buy %1 created with a price %2 %3")
                        .arg(
                            QString::number(amountToBuy * mInstrumentLot),
                            QString::number(quotationToFloat(price), 'f', mPricePrecision),
                            "\u20BD"
                        )
                );

                mOrderId          = QString::fromStdString(tinkoffOrder->order_id());
                mLastOrderPrice   = price;
                mLastExpectedCost = expected;

                break;
            }

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

            if (tinkoffOrder->execution_report_status() == tinkoff::EXECUTION_REPORT_STATUS_FILL)
            {
                mOrderId = "";
            }
            else
            {
                mLogsThread->addLog(LOG_LEVEL_VERBOSE, mInstrumentId, tr("Order cancelled"));

                const std::shared_ptr<tinkoff::CancelOrderResponse> tinkoffCancel =
                    mGrpcClient->cancelOrder(QThread::currentThread(), mAccountId, mOrderId);

                if (!QThread::currentThread()->isInterruptionRequested() && tinkoffCancel != nullptr)
                {
                    mOrderId = "";
                }
            }
        }
    }
}

void TradingThread::informAboutOrderState(const tinkoff::OrderState& tinkoffOrder)
{
    QString details;

    if (tinkoffOrder.direction() == tinkoff::ORDER_DIRECTION_BUY)
    {
        details = tr("%1 bought with a price %2 %3")
                      .arg(
                          QString::number(tinkoffOrder.lots_executed() * mInstrumentLot),
                          QString::number(quotationToFloat(tinkoffOrder.initial_security_price()), 'f', mPricePrecision),
                          "\u20BD"
                      );
    }
    else
    {
        details = tr("%1 sold with a price %2 %3")
                      .arg(
                          QString::number(tinkoffOrder.lots_executed() * mInstrumentLot),
                          QString::number(quotationToFloat(tinkoffOrder.initial_security_price()), 'f', mPricePrecision),
                          "\u20BD"
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
