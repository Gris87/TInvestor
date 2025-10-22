#include "src/decisions/buy/buydecision2/buydecision2.h"

#include <QDebug>



const char* const DATETIME_FORMAT = "yyyy-MM-dd hh:mm:ss";

constexpr int    MINUTES_TO_DOUBLE_CHECK = 5;
constexpr int    HOURS_TO_TRIPLE_CHECK   = 3;
constexpr int    STEP_FOR_TRIPLE_CHECK   = 60;
constexpr float  TRIPLE_MINIMUM_COEF     = 3.0f;
constexpr float  HUNDRED_PERCENT         = 100.0f;
constexpr qint64 MS_IN_SECOND            = 1000LL;
constexpr qint64 ONE_MINUTE              = 60LL * MS_IN_SECOND;



BuyDecision2::BuyDecision2() :
    IActionDecision(),
    mStepForTripleCheck(STEP_FOR_TRIPLE_CHECK)
{
    qDebug() << "Create BuyDecision2";
}

BuyDecision2::~BuyDecision2()
{
    qDebug() << "Destroy BuyDecision2";
}

// NOLINTBEGIN(readability-function-cognitive-complexity)
QString BuyDecision2::makeDecision(
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

    IBuyDecision2Config* buyConfig = config->getBuyDecision2Config();

    if (buyConfig->isEnabled())
    {
        if (dateRange)
        {
            return makeDecisionBasedOnStockData(parentThread, buyConfig, limitTimestamp, stock, dataIndex, price);
        }

        return makeDecisionBasedOnStockOperationalData(parentThread, buyConfig, limitTimestamp, stock, price);
    }

    return "";
}
// NOLINTEND(readability-function-cognitive-complexity)

AsapMode BuyDecision2::asapMode() const
{
    return ASAP_MODE_NONE;
}

QString BuyDecision2::makeDecisionBasedOnStockData(
    QThread* parentThread, IBuyDecision2Config* buyConfig, qint64 limitTimestamp, Stock* stock, int dataIndex, float price
)
{
    const float priceFall    = -buyConfig->getPriceFall();
    const float loseYield    = buyConfig->getLoseYield();
    const int   duration     = buyConfig->getDuration();
    const float maximumPrice = price / (1 + (priceFall / HUNDRED_PERCENT));

    const StockData* stockData = stock->data.constData();

    limitTimestamp = qMax(limitTimestamp, stockData[dataIndex].timestamp - (duration * ONE_MINUTE));

    for (int i = dataIndex; i >= 0 && !parentThread->isInterruptionRequested(); --i)
    {
        const qint64 timestamp = stockData[i].timestamp;
        const float  prevPrice = stockData[i].price;

        if (timestamp < limitTimestamp)
        {
            break;
        }

        if (prevPrice >= maximumPrice)
        {
            if (doubleCheckBasedOnStockData(parentThread, stockData, i - 1, maximumPrice))
            {
                const float tripleMinimumPrice = prevPrice / (1 - (TRIPLE_MINIMUM_COEF * priceFall / HUNDRED_PERCENT));

                if (tripleCheck(parentThread, stockData, i - mStepForTripleCheck, tripleMinimumPrice))
                {
                    const float minimumPrice = price / (1 + (loseYield / HUNDRED_PERCENT));

                    for (int j = dataIndex; j >= 0 && !parentThread->isInterruptionRequested(); --j)
                    {
                        const float prevPrice2 = stockData[j].price;

                        if (prevPrice2 >= maximumPrice)
                        {
                            break;
                        }

                        if (prevPrice2 <= minimumPrice)
                        {
                            const float fall      = ((price / prevPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;
                            const float lostYield = ((price / prevPrice2) * HUNDRED_PERCENT) - HUNDRED_PERCENT;

                            return QObject::tr(
                                       "Decided to buy because the price fall to %1 from %2 at %3 and lost "
                                       "yield %4 from the minimum price %5 at %6 within last %7 minutes and the fall is %8"
                            )
                                .arg(
                                    QString::number(price, 'f', stock->meta.pricePrecision) + " \u20BD",
                                    QString::number(prevPrice, 'f', stock->meta.pricePrecision) + " \u20BD",
                                    QDateTime::fromMSecsSinceEpoch(timestamp).toString(DATETIME_FORMAT),
                                    QString::number(lostYield, 'f', 2) + "%",
                                    QString::number(prevPrice2, 'f', stock->meta.pricePrecision) + " \u20BD",
                                    QDateTime::fromMSecsSinceEpoch(stockData[j].timestamp).toString(DATETIME_FORMAT),
                                    QString::number(duration),
                                    QString::number(fall, 'f', 2) + "%"
                                );
                        }
                    }
                }
            }
        }
    }

    return "";
}

QString BuyDecision2::makeDecisionBasedOnStockOperationalData(
    QThread* parentThread, IBuyDecision2Config* buyConfig, qint64 limitTimestamp, Stock* stock, float price
)
{
    const float priceFall    = -buyConfig->getPriceFall();
    const float loseYield    = buyConfig->getLoseYield();
    const int   duration     = buyConfig->getDuration();
    const float maximumPrice = price / (1 + (priceFall / HUNDRED_PERCENT));

    const StockData*            stockData            = stock->data.constData();
    const StockOperationalData* stockOperationalData = stock->operational.detailedData.constData();

    limitTimestamp = qMax(limitTimestamp, QDateTime::currentMSecsSinceEpoch() - (duration * ONE_MINUTE));

    for (int i = stock->operational.detailedData.size() - 1; i >= 0 && !parentThread->isInterruptionRequested(); --i)
    {
        const qint64 timestamp = stockOperationalData[i].timestamp;
        const float  prevPrice = stockOperationalData[i].price;

        if (timestamp < limitTimestamp)
        {
            break;
        }

        if (prevPrice >= maximumPrice)
        {
            if (doubleCheckBasedOnStockOperationalData(parentThread, stockOperationalData, i - 1, maximumPrice))
            {
                const float tripleMinimumPrice = prevPrice / (1 - (TRIPLE_MINIMUM_COEF * priceFall / HUNDRED_PERCENT));

                if (tripleCheck(parentThread, stockData, stock->data.size() - 1, tripleMinimumPrice))
                {
                    const float minimumPrice = price / (1 + (loseYield / HUNDRED_PERCENT));

                    for (int j = stock->operational.detailedData.size() - 1; j >= 0 && !parentThread->isInterruptionRequested();
                         --j)
                    {
                        const float prevPrice2 = stockOperationalData[j].price;

                        if (prevPrice2 >= maximumPrice)
                        {
                            break;
                        }

                        if (prevPrice2 <= minimumPrice)
                        {
                            const float fall      = ((price / prevPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;
                            const float lostYield = ((price / prevPrice2) * HUNDRED_PERCENT) - HUNDRED_PERCENT;

                            return QObject::tr(
                                       "Decided to buy because the price fall to %1 from %2 at %3 and lost "
                                       "yield %4 from the minimum price %5 at %6 within last %7 minutes and the fall "
                                       "is %8"
                            )
                                .arg(
                                    QString::number(price, 'f', stock->meta.pricePrecision) + " \u20BD",
                                    QString::number(prevPrice, 'f', stock->meta.pricePrecision) + " \u20BD",
                                    QDateTime::fromMSecsSinceEpoch(timestamp).toString(DATETIME_FORMAT),
                                    QString::number(lostYield, 'f', 2) + "%",
                                    QString::number(prevPrice2, 'f', stock->meta.pricePrecision) + " \u20BD",
                                    QDateTime::fromMSecsSinceEpoch(stockOperationalData[j].timestamp).toString(DATETIME_FORMAT),
                                    QString::number(duration),
                                    QString::number(fall, 'f', 2) + "%"
                                );
                        }
                    }
                }
            }
        }
    }

    return "";
}

bool BuyDecision2::doubleCheckBasedOnStockData(QThread* parentThread, const StockData* stockData, int index, float maximumPrice)
{
    bool res = false;

    if (index >= MINUTES_TO_DOUBLE_CHECK - 1)
    {
        res = true;

        int j           = index;
        int minutesLeft = MINUTES_TO_DOUBLE_CHECK;

        while (j >= 0 && minutesLeft > 0 && !parentThread->isInterruptionRequested())
        {
            if (stockData[j].price < maximumPrice)
            {
                res = false;

                break;
            }

            --j;
            --minutesLeft;
        }
    }

    return res;
}

bool BuyDecision2::doubleCheckBasedOnStockOperationalData(
    QThread* parentThread, const StockOperationalData* stockOperationalData, int index, float maximumPrice
)
{
    bool res = false;

    if (index >= MINUTES_TO_DOUBLE_CHECK - 1)
    {
        res = true;

        int j           = index;
        int minutesLeft = MINUTES_TO_DOUBLE_CHECK;

        while (j >= 0 && minutesLeft > 0 && !parentThread->isInterruptionRequested())
        {
            if (stockOperationalData[j].price < maximumPrice)
            {
                res = false;

                break;
            }

            --j;
            --minutesLeft;
        }
    }

    return res;
}

bool BuyDecision2::tripleCheck(QThread* parentThread, const StockData* stockData, int index, float tripleMinimumPrice)
{
    bool res = true;

    int j         = index;
    int hoursLeft = HOURS_TO_TRIPLE_CHECK;

    while (j >= 0 && hoursLeft > 0 && !parentThread->isInterruptionRequested())
    {
        if (stockData[j].price < tripleMinimumPrice)
        {
            res = false;

            break;
        }

        j -= mStepForTripleCheck;
        --hoursLeft;
    }

    return res;
}
