#include "src/decisions/buy/buydecision2/buydecision2.h"

#include <QDebug>



const char* const DATETIME_FORMAT = "yyyy-MM-dd hh:mm:ss";

constexpr int    MINUTES_TO_DOUBLE_CHECK = 3;
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
        const float priceFall          = -buyConfig->getPriceFall();
        const float loseYield          = buyConfig->getLoseYield();
        const int   duration           = buyConfig->getDuration();
        const float maximumPrice       = price / (1 + (priceFall / HUNDRED_PERCENT));

        const StockData* stockData = stock->data.constData();

        if (dateRange)
        {
            limitTimestamp = qMax(limitTimestamp, stockData[dataIndex].timestamp - (duration * ONE_MINUTE));

            for (int i = dataIndex - 1; i >= 0 && !parentThread->isInterruptionRequested(); --i)
            {
                const qint64 timestamp = stockData[i].timestamp;
                const float  prevPrice = stockData[i].price;

                if (timestamp < limitTimestamp)
                {
                    break;
                }

                if (prevPrice >= maximumPrice)
                {
                    bool good = true;

                    int j           = i - 1;
                    int minutesLeft = MINUTES_TO_DOUBLE_CHECK;

                    while (j >= 0 && minutesLeft > 0 && !parentThread->isInterruptionRequested())
                    {
                        if (stockData[j].price < maximumPrice)
                        {
                            good = false;

                            break;
                        }

                        --j;
                        --minutesLeft;
                    }

                    if (good)
                    {
                        const float tripleMinimumPrice = prevPrice / (1 - (TRIPLE_MINIMUM_COEF * priceFall / HUNDRED_PERCENT));

                        int j         = i - mStepForTripleCheck;
                        int hoursLeft = HOURS_TO_TRIPLE_CHECK;

                        while (j >= 0 && hoursLeft > 0 && !parentThread->isInterruptionRequested())
                        {
                            if (stockData[j].price < tripleMinimumPrice)
                            {
                                good = false;

                                break;
                            }

                            j -= mStepForTripleCheck;
                            --hoursLeft;
                        }

                        if (good)
                        {
                            const float minimumPrice = price / (1 + (loseYield / HUNDRED_PERCENT));

                            for (j = dataIndex - 1; j >= 0 && !parentThread->isInterruptionRequested(); --j)
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

                                    return QObject::
                                        tr("Decided to buy because the price fall to %1 from %2 at %3 and lost "
                                           "yield %4 from the minimum price %5 at %6 within last %7 minutes and the fall is %8")
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
        }
        else
        {
            const StockOperationalData* stockOperationalData = stock->operational.detailedData.constData();

            limitTimestamp = qMax(limitTimestamp, QDateTime::currentMSecsSinceEpoch() - (duration * ONE_MINUTE));

            for (int i = stock->operational.detailedData.size() - 2; i >= 0 && !parentThread->isInterruptionRequested(); --i)
            {
                const qint64 timestamp = stockOperationalData[i].timestamp;
                const float  prevPrice = stockOperationalData[i].price;

                if (timestamp < limitTimestamp)
                {
                    break;
                }

                if (prevPrice >= maximumPrice)
                {
                    if (i >= MINUTES_TO_DOUBLE_CHECK)
                    {
                        bool good = true;

                        int j           = i - 1;
                        int minutesLeft = MINUTES_TO_DOUBLE_CHECK;

                        while (j >= 0 && minutesLeft > 0 && !parentThread->isInterruptionRequested())
                        {
                            if (stockOperationalData[j].price < maximumPrice)
                            {
                                good = false;

                                break;
                            }

                            --j;
                            --minutesLeft;
                        }

                        if (good)
                        {
                            const float tripleMinimumPrice =
                                prevPrice / (1 - (TRIPLE_MINIMUM_COEF * priceFall / HUNDRED_PERCENT));

                            int j         = stock->data.size() - 1;
                            int hoursLeft = HOURS_TO_TRIPLE_CHECK;

                            while (j >= 0 && hoursLeft > 0 && !parentThread->isInterruptionRequested())
                            {
                                if (stockData[j].price < tripleMinimumPrice)
                                {
                                    good = false;

                                    break;
                                }

                                j -= mStepForTripleCheck;
                                --hoursLeft;
                            }

                            if (good)
                            {
                                const float minimumPrice = price / (1 + (loseYield / HUNDRED_PERCENT));

                                for (j = stock->operational.detailedData.size() - 2;
                                     j >= 0 && !parentThread->isInterruptionRequested();
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
                                                QDateTime::fromMSecsSinceEpoch(stockOperationalData[j].timestamp)
                                                    .toString(DATETIME_FORMAT),
                                                QString::number(duration),
                                                QString::number(fall, 'f', 2) + "%"
                                            );
                                    }
                                }
                            }
                        }
                    }
                }
            }

            for (int i = stock->data.size() - 1; i >= 0 && !parentThread->isInterruptionRequested(); --i)
            {
                const qint64 timestamp = stockData[i].timestamp;
                const float  prevPrice = stockData[i].price;

                if (timestamp < limitTimestamp)
                {
                    break;
                }

                if (prevPrice >= maximumPrice)
                {
                    bool good = true;

                    int j           = i - 1;
                    int minutesLeft = MINUTES_TO_DOUBLE_CHECK;

                    while (j >= 0 && minutesLeft > 0 && !parentThread->isInterruptionRequested())
                    {
                        if (stockData[j].price < maximumPrice)
                        {
                            good = false;

                            break;
                        }

                        --j;
                        --minutesLeft;
                    }

                    if (good)
                    {
                        const float tripleMinimumPrice = prevPrice / (1 - (TRIPLE_MINIMUM_COEF * priceFall / HUNDRED_PERCENT));

                        int j         = i - mStepForTripleCheck;
                        int hoursLeft = HOURS_TO_TRIPLE_CHECK;

                        while (j >= 0 && hoursLeft > 0 && !parentThread->isInterruptionRequested())
                        {
                            if (stockData[j].price < tripleMinimumPrice)
                            {
                                good = false;

                                break;
                            }

                            j -= mStepForTripleCheck;
                            --hoursLeft;
                        }

                        if (good)
                        {
                            const float minimumPrice = price / (1 + (loseYield / HUNDRED_PERCENT));

                            for (j = stock->data.size() - 1; j >= 0 && !parentThread->isInterruptionRequested(); --j)
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

                                    return QObject::
                                        tr("Decided to buy because the price fall to %1 from %2 at %3 and lost "
                                           "yield %4 from the minimum price %5 at %6 within last %7 minutes and the fall is %8")
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
        }
    }

    return "";
}
// NOLINTEND(readability-function-cognitive-complexity)
