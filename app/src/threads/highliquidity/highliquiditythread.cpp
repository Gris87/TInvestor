#include "src/threads/highliquidity/highliquiditythread.h"

#include <QDateTime>
#include <QDebug>

#include "src/grpc/utils.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";
const char* const TMON_UID  = "498ec3ff-ef27-4729-9703-a5aac48d5789";

constexpr float HUNDRED_PERCENT             = 100.0f;
constexpr int   NORMAL_SESSION_START_HOUR   = 10;
constexpr int   NORMAL_SESSION_START_MINUTE = 0;
constexpr int   NORMAL_SESSION_END_HOUR     = 18;
constexpr int   NORMAL_SESSION_END_MINUTE   = 30;
constexpr int   EXTRA_SESSION_END_HOUR      = 23;
constexpr int   EXTRA_SESSION_END_MINUTE    = 40;



HighLiquidityThread::HighLiquidityThread(
    IConfig* config, ITimeUtils* timeUtils, IGrpcClient* grpcClient, IGrpcRetryClient* grpcRetryClient, QObject* parent
) :
    IHighLiquidityThread(parent),
    mConfig(config),
    mTimeUtils(timeUtils),
    mGrpcClient(grpcClient),
    mGrpcRetryClient(grpcRetryClient),
    mMoscowTimezone("Europe/Moscow"),
    mAccountId()
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
    if (!mConfig->isTradeInNonWorkingHours() && !mTimeUtils->isWorkingHours(timestamp))
    {
        return;
    }

    const QDateTime dateTime  = QDateTime::fromMSecsSinceEpoch(timestamp, mMoscowTimezone);
    const QTime     time      = dateTime.time();
    const QTime     startTime = QTime(NORMAL_SESSION_START_HOUR, NORMAL_SESSION_START_MINUTE);

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
    const std::shared_ptr<tinkoff::PortfolioResponse> tinkoffPortfolio =
        mGrpcRetryClient->getValidPortfolio(QThread::currentThread(), mAccountId);

    if (!QThread::currentThread()->isInterruptionRequested() && tinkoffPortfolio != nullptr)
    {
        double money     = 0.0;
        double totalCost = 0.0;
        double etfCost   = 0.0;
        float  etfPrice  = 0.0f;

        calculateMoneyAndTotalCost(*tinkoffPortfolio, money, totalCost, etfCost, etfPrice);

        const double expectedCost =
            money + etfCost - (totalCost * mConfig->getLiquidityEtfRemainedPartNightly() / HUNDRED_PERCENT);

        if (!QThread::currentThread()->isInterruptionRequested() && expectedCost - etfCost > etfPrice)
        {
            InstrumentsForTrading instrumentsForTrading;

            instrumentsForTrading[TMON_UID] = TradingInfo(
                ASAP_MODE_IMMEDIATELY_TRADE, -1.0f, -1.0f, expectedCost, tr("Decided to buy because trading day is over")
            );
            emit tradeInstruments(instrumentsForTrading);
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
                InstrumentsForTrading instrumentsForTrading;

                instrumentsForTrading[instrumentId] = TradingInfo(
                    ASAP_MODE_IMMEDIATELY_TRADE, -1.0f, -1.0f, 0.0, tr("Decided to sell because it had been a night since buying")
                );
                emit tradeInstruments(instrumentsForTrading);

                break;
            }
        }
    }
}

void HighLiquidityThread::setAccountId(const QString& accountId)
{
    mAccountId = accountId;
}

void HighLiquidityThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}

void HighLiquidityThread::calculateMoneyAndTotalCost(
    const tinkoff::PortfolioResponse& tinkoffPortfolio, double& money, double& totalCost, double& etfCost, float& etfPrice
)
{
    money     = 0.0;
    totalCost = 0.0;
    etfCost   = 0.0;
    etfPrice  = 0.0f;

    for (int i = 0; i < tinkoffPortfolio.positions_size() && !QThread::currentThread()->isInterruptionRequested(); ++i)
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
            const double cost = quotationToDouble(position.quantity()) * quotationToFloat(position.average_position_price_fifo());

            if (instrumentId == TMON_UID)
            {
                etfCost  = cost;
                etfPrice = quotationToFloat(position.current_price());
            }

            totalCost += cost;
        }
    }
}
