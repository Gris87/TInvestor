#include "src/decisions/buy/buydecision7/buydecision7.h"

#include <QDebug>



constexpr float  HUNDRED_PERCENT         = 100.0f;
constexpr qint64 MS_IN_SECOND            = 1000LL;
constexpr qint64 ONE_MINUTE              = 60LL * MS_IN_SECOND;
constexpr qint64 ONE_HOUR                = 60LL * ONE_MINUTE;
constexpr qint64 ONE_DAY                 = 24LL * ONE_HOUR;
constexpr qint64 NIGHT_DELAY             = 2LL * ONE_HOUR; // 2 hours
constexpr int    MINUTES_TO_CHECK        = 15;
constexpr int    MINUTES_TO_DOUBLE_CHECK = 5;
constexpr int    MINUTES_BEFORE_DAY_END  = 10;



// ====================================================================
// Buy stock if price raises multiple times during night
// ====================================================================
//
//           /\                  /\                  /\
//          /  \                /  \                /  \
// -------|/    \-------------|/    \-------------|/    \-------------X
//

BuyDecision7::BuyDecision7(ITimeUtils* timeUtils) :
    IActionDecision(),
    mTimeUtils(timeUtils)
{
    qDebug() << "Create BuyDecision7";
}

BuyDecision7::~BuyDecision7()
{
    qDebug() << "Destroy BuyDecision7";
}

QString BuyDecision7::makeDecision(
    QThread*              parentThread,
    IDecisionMakerConfig* config,
    qint64                limitTimestamp,
    Stock*                stock,
    bool                  dateRange,
    int                   dataIndex,
    float                 price,
    float /*avgPrice*/,
    float /*commission*/
)
{
    Q_ASSERT_X(parentThread != nullptr, __FUNCTION__, "parentThread is invalid");
    Q_ASSERT_X(config != nullptr, __FUNCTION__, "config is invalid");
    Q_ASSERT_X(limitTimestamp >= 0, __FUNCTION__, "limitTimestamp is invalid");
    Q_ASSERT_X(stock != nullptr, __FUNCTION__, "stock is invalid");
    Q_ASSERT_X(
        (dateRange && dataIndex >= 0 && dataIndex < stock->data.size()) || (!dateRange && dataIndex == -1),
        __FUNCTION__,
        "dateRange and dataIndex are invalid"
    );
    Q_ASSERT_X(price > 0, __FUNCTION__, "price is invalid");

    IBuyDecision7Config* buyConfig = config->getBuyDecision7Config();

    if (buyConfig->isEnabled())
    {
        if (dateRange)
        {
            return makeDecisionBasedOnStockData(parentThread, buyConfig, limitTimestamp, stock, dataIndex);
        }

        return makeDecisionBasedOnStockData(parentThread, buyConfig, limitTimestamp, stock, stock->data.size() - 1);
    }

    return "";
}

AsapMode BuyDecision7::asapMode() const
{
    return ASAP_MODE_IMMEDIATELY_TRADE;
}

// NOLINTBEGIN(readability-function-cognitive-complexity)
QString BuyDecision7::makeDecisionBasedOnStockData(
    QThread* parentThread, IBuyDecision7Config* buyConfig, qint64 limitTimestamp, Stock* stock, int dataIndex
) const
{
    const StockData* stockData = stock->data.constData();

    int successNights = 0;
    int failedNights  = 0;

    const float priceRaise = buyConfig->getPriceRaise();
    const int   duration   = buyConfig->getDuration();

    const qint64 currentTimestamp = stockData[dataIndex].timestamp;

    const QTime startTime   = stock->meta.lastTradeTime.addMSecs(-(MINUTES_BEFORE_DAY_END * ONE_MINUTE));
    const QTime endTime     = stock->meta.lastTradeTime;
    const QTime currentTime = QDateTime::fromMSecsSinceEpoch(currentTimestamp).time();

    if (!mTimeUtils->isTimeBetween(currentTime, startTime, endTime))
    {
        return "";
    }

    limitTimestamp = currentTimestamp - (duration * ONE_DAY) - ONE_HOUR;

    for (int i = dataIndex; i >= 0 && !parentThread->isInterruptionRequested(); --i)
    {
        const qint64 timestamp = stockData[i].timestamp;

        if (timestamp < limitTimestamp)
        {
            break;
        }

        const qint64 nextTimestamp = stockData[i + 1].timestamp;

        if (nextTimestamp - timestamp >= NIGHT_DELAY)
        {
            const float prevPrice    = stockData[i].price;
            const float maximumPrice = prevPrice * (1 + (priceRaise / HUNDRED_PERCENT));

            if (doubleCheck(parentThread, stock, stockData, i + 1, nextTimestamp, maximumPrice))
            {
                ++successNights;
            }
            else
            {
                ++failedNights;

                break;
            }
        }
    }

    if (successNights > 0 && failedNights == 0)
    {
        return QObject::tr("Decided to buy because the price raises more than %1 during last %2 nights")
            .arg(QString::number(priceRaise, 'f', 2) + "%", QString::number(duration));
    }

    return "";
}
// NOLINTEND(readability-function-cognitive-complexity)

bool BuyDecision7::doubleCheck(
    QThread* parentThread, Stock* stock, const StockData* stockData, int index, qint64 nextTimestamp, float maximumPrice
) const
{
    const qint64 anotherLimitTimestamp = nextTimestamp + (MINUTES_TO_CHECK * ONE_MINUTE);
    int          minutesLeft           = MINUTES_TO_DOUBLE_CHECK;

    for (int j = index; j < stock->data.size() && !parentThread->isInterruptionRequested(); ++j)
    {
        const qint64 anotherTimestamp = stockData[j].timestamp;
        const float  anotherPrice     = stockData[j].price;

        if (anotherTimestamp > anotherLimitTimestamp)
        {
            break;
        }

        if (anotherPrice >= maximumPrice)
        {
            --minutesLeft;

            if (minutesLeft <= 0)
            {
                break;
            }
        }
    }

    return minutesLeft <= 0;
}
