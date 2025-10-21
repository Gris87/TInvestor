#include "src/decisions/buy/buydecision6/buydecision6.h"

#include <QDebug>



constexpr float  HUNDRED_PERCENT = 100.0f;
constexpr qint64 MS_IN_SECOND    = 1000LL;
constexpr qint64 ONE_MINUTE      = 60LL * MS_IN_SECOND;
constexpr qint64 ONE_HOUR        = 60LL * ONE_MINUTE;
constexpr qint64 ONE_DAY         = 24LL * ONE_HOUR;
constexpr qint64 NIGHT_DELAY     = 2LL * ONE_HOUR; // 2 hours



BuyDecision6::BuyDecision6() :
    IActionDecision()
{
    qDebug() << "Create BuyDecision6";
}

BuyDecision6::~BuyDecision6()
{
    qDebug() << "Destroy BuyDecision6";
}

// NOLINTBEGIN(readability-function-cognitive-complexity)
QString BuyDecision6::makeDecision(
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

    IBuyDecision6Config* buyConfig = config->getBuyDecision6Config();

    if (buyConfig->isEnabled())
    {
        if (dateRange)
        {
            return makeDecisionBasedOnStockData(parentThread, buyConfig, limitTimestamp, stock, dataIndex, price);
        }
    }

    return "";
}
// NOLINTEND(readability-function-cognitive-complexity)

AsapMode BuyDecision6::asapMode() const
{
    return ASAP_MODE_FOLLOW_PRICE;
}

QString BuyDecision6::makeDecisionBasedOnStockData(
    QThread* parentThread, IBuyDecision6Config* buyConfig, qint64 /*limitTimestamp*/, Stock* stock, int dataIndex, float price
)
{
    const float rsiLimit = buyConfig->getRsi();
    const int   duration = buyConfig->getDuration();

    const QList<float> prices = getDayPrices(parentThread, stock, dataIndex, duration);

    if (prices.length() < 3)
    {
        return "";
    }

    const float sma = calculateSma(prices);
    const float ema = calculateEma(prices);
    const float rsi = calculateRsi(prices);

    if (price >= sma && price >= ema && rsi <= rsiLimit)
    {
        return QObject::tr(
                   "Decided to buy because the price %1 is above SMA %2 and EMA %3 and RSI %4 is less than %5 for last %6 minutes"
        )
            .arg(
                QString::number(price, 'f', stock->meta.pricePrecision) + " \u20BD",
                QString::number(sma, 'f', stock->meta.pricePrecision) + " \u20BD",
                QString::number(ema, 'f', stock->meta.pricePrecision) + " \u20BD",
                QString::number(rsi, 'f', 2) + "%",
                QString::number(rsiLimit, 'f', 2) + "%",
                QString::number(duration)
            );
    }

    return "";
}

QList<float> BuyDecision6::getDayPrices(QThread* parentThread, Stock* stock, int dataIndex, int duration)
{
    const StockData* stockData = stock->data.constData();

    const qint64 currentTimestamp = stockData[dataIndex].timestamp;
    const qint64 limitTimestamp   = currentTimestamp - (duration * ONE_DAY);

    QList<float> prices;

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
            prices.prepend(stockData[i].price);
        }
    }

    return prices;
}

float BuyDecision6::calculateSma(const QList<float>& prices)
{
    return std::accumulate(prices.begin(), prices.end(), 0.0) / prices.size();
}

float BuyDecision6::calculateEma(const QList<float>& prices)
{
    const double span = prices.size();

    double sum     = 0.0;
    double weights = 0.0;

    for (int i = 0; i < prices.size(); ++i)
    {
        const double weight = 2.0 / (span + 1) * (span - i) / span;

        sum     += prices.at(i) * weight;
        weights += weight;
    }

    return sum / weights;
}

float BuyDecision6::calculateRsi(const QList<float>& prices)
{
    double gain = 0.0;
    double loss = 0.0;

    for (int i = 0; i < prices.size() - 1; ++i)
    {
        const double delta = prices.at(i + 1) - prices.at(i);

        if (delta > 0)
        {
            gain += delta;
        }
        else
        {
            loss -= delta;
        }
    }

    return loss > 0.0 ? (HUNDRED_PERCENT - (HUNDRED_PERCENT / (1 + (gain / loss)))) : HUNDRED_PERCENT;
}
