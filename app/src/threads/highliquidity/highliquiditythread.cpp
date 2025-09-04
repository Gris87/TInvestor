#include "src/threads/highliquidity/highliquiditythread.h"

#include <QDateTime>
#include <QDebug>

#include "src/grpc/utils.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";
const char* const TMON_UID  = "498ec3ff-ef27-4729-9703-a5aac48d5789";

constexpr int    NORMAL_SESSION_START_HOUR   = 10;
constexpr int    NORMAL_SESSION_START_MINUTE = 0;
constexpr int    NORMAL_SESSION_END_HOUR     = 18;
constexpr int    NORMAL_SESSION_END_MINUTE   = 30;
constexpr int    EXTRA_SESSION_END_HOUR      = 23;
constexpr int    EXTRA_SESSION_END_MINUTE    = 40;
constexpr qint64 MS_IN_SECOND                = 1000LL;
constexpr qint64 SLEEP_BEFORE_REQUEST        = 1LL * MS_IN_SECOND; // 1 second



HighLiquidityThread::HighLiquidityThread(IConfig* config, ITimeUtils* timeUtils, IGrpcClient* grpcClient, QObject* parent) :
    IHighLiquidityThread(parent),
    mConfig(config),
    mTimeUtils(timeUtils),
    mGrpcClient(grpcClient),
    mMoscowTimezone("Europe/Moscow")
{
    qDebug() << "Create HighLiquidityThread";
}

HighLiquidityThread::~HighLiquidityThread()
{
    qDebug() << "Destroy HighLiquidityThread";
}

void HighLiquidityThread::run()
{
    qDebug() << "Running HighLiquidityThread";

    blockSignals(false);
    makeDecisionBaseOnTimestamp(QDateTime::currentMSecsSinceEpoch());

    qDebug() << "Finish HighLiquidityThread";
}

void HighLiquidityThread::makeDecisionBaseOnTimestamp(qint64 timestamp)
{
    const QDateTime dateTime  = QDateTime::fromMSecsSinceEpoch(timestamp, mMoscowTimezone);
    const int       dayOfWeek = dateTime.date().dayOfWeek();

    if (dayOfWeek == Qt::Saturday || dayOfWeek == Qt::Sunday)
    {
        return;
    }

    const QTime time      = dateTime.time();
    const QTime startTime = QTime(NORMAL_SESSION_START_HOUR, NORMAL_SESSION_START_MINUTE);

    if (time < startTime)
    {
        return;
    }

    const QTime endTime = mConfig->isTradeInNonWorkingHours() ? QTime(EXTRA_SESSION_END_HOUR, EXTRA_SESSION_END_MINUTE)
                                                              : QTime(NORMAL_SESSION_END_HOUR, NORMAL_SESSION_END_MINUTE);

    if (time < endTime)
    {
        sellEtf();

        return;
    }

    buyEtf();
}

void HighLiquidityThread::buyEtf()
{
    bool success = false;

    while (!QThread::currentThread()->isInterruptionRequested() && !success)
    {
        const std::shared_ptr<tinkoff::PortfolioResponse> tinkoffPortfolio =
            mGrpcClient->getPortfolio(QThread::currentThread(), mAccountId);

        if (!QThread::currentThread()->isInterruptionRequested() && tinkoffPortfolio != nullptr)
        {
            if (validatePortfolioResponse(*tinkoffPortfolio))
            {
                double money     = 0.0;
                double totalCost = 0.0;
                bool   etfFound  = false;

                calculateMoneyAndTotalCost(*tinkoffPortfolio, money, totalCost, etfFound);
                money -= keepMoney();

                if (!QThread::currentThread()->isInterruptionRequested() && !etfFound && money > 0)
                {
                    // TODO: Buy
                }

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
}

void HighLiquidityThread::sellEtf()
{
    const std::shared_ptr<tinkoff::PortfolioResponse> tinkoffPortfolio =
        mGrpcClient->getPortfolio(QThread::currentThread(), mAccountId);

    if (!QThread::currentThread()->isInterruptionRequested() && tinkoffPortfolio != nullptr)
    {
        for (int i = 0; i < tinkoffPortfolio->positions_size(); ++i)
        {
            const tinkoff::PortfolioPosition& position = tinkoffPortfolio->positions(i);

            const QString instrumentId = QString::fromStdString(position.instrument_uid());

            if (instrumentId == TMON_UID)
            {
                // TODO: Sell
            }
        }
    }
}

void HighLiquidityThread::setAccountId(const QString& accountId)
{
    mAccountId = accountId;
}

void HighLiquidityThread::setKeepMoney(int value)
{
    const QWriteLocker lock(mRwMutex);

    mKeepMoney = value;
}

int HighLiquidityThread::keepMoney() const
{
    const QReadLocker lock(mRwMutex);

    return mKeepMoney;
}

void HighLiquidityThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}

bool HighLiquidityThread::validatePortfolioResponse(const tinkoff::PortfolioResponse& tinkoffPortfolio)
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

void HighLiquidityThread::calculateMoneyAndTotalCost(
    const tinkoff::PortfolioResponse& tinkoffPortfolio, double& money, double& totalCost, bool& etfFound
)
{
    money     = 0.0;
    totalCost = 0.0;
    etfFound  = false;

    for (int i = 0; i < tinkoffPortfolio.positions_size() && !QThread::currentThread()-->isInterruptionRequested(); ++i)
    {
        const tinkoff::PortfolioPosition& position = tinkoffPortfolio.positions(i);

        const QString instrumentId = QString::fromStdString(position.instrument_uid());

        if (instrumentId == RUBLE_UID)
        {
            money      = quotationToDouble(position.quantity());
            totalCost += money;
        }
        else
        {
            if (instrumentId == TMON_UID)
            {
                etfFound = true;
            }

            totalCost += quotationToDouble(position.quantity()) * quotationToFloat(position.average_position_price_fifo());
        }
    }
}
