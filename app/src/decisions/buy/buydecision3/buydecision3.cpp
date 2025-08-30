#include "src/decisions/buy/buydecision3/buydecision3.h"

#include <QDebug>



const char* const DATETIME_FORMAT = "yyyy-MM-dd hh:mm:ss";

constexpr int    STEP                    = 60;
constexpr int    MINUTES_TO_DOUBLE_CHECK = 3;
constexpr float  HUNDRED_PERCENT         = 100.0f;
constexpr qint64 MS_IN_SECOND            = 1000LL;
constexpr qint64 ONE_MINUTE              = 60LL * MS_IN_SECOND;
constexpr qint64 ONE_HOUR                = 60LL * ONE_MINUTE;
constexpr qint64 ONE_DAY                 = 24LL * ONE_HOUR;



BuyDecision3::BuyDecision3() :
    IActionDecision(),
    mStep(STEP)
{
    qDebug() << "Create BuyDecision3";
}

BuyDecision3::~BuyDecision3()
{
    qDebug() << "Destroy BuyDecision3";
}

// NOLINTBEGIN(readability-function-cognitive-complexity)
QString BuyDecision3::makeDecision(
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

    IBuyDecision3Config* buyConfig = config->getBuyDecision3Config();

    if (buyConfig->isEnabled())
    {
        const float priceFall    = -buyConfig->getPriceFall();
        const int   duration     = buyConfig->getDuration();
        const float maximumPrice = price / (1 + (priceFall / HUNDRED_PERCENT));

        const StockData* stockData = stock->data.constData();

        if (dateRange)
        {
            limitTimestamp = qMax(limitTimestamp, stockData[dataIndex].timestamp - (duration * ONE_DAY));

            for (int i = dataIndex - 1; i >= 0 && !parentThread->isInterruptionRequested(); i -= mStep)
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
                        const float fall = ((price / prevPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;

                        return QObject::tr(
                                   "Decided to buy because the price fall to %1 from %2 at %3 within last %4 days and the "
                                   "fall is %5"
                        )
                            .arg(
                                QString::number(price, 'f', stock->meta.pricePrecision) + " \u20BD",
                                QString::number(prevPrice, 'f', stock->meta.pricePrecision) + " \u20BD",
                                QDateTime::fromMSecsSinceEpoch(timestamp).toString(DATETIME_FORMAT),
                                QString::number(duration),
                                QString::number(fall, 'f', 2) + "%"
                            );
                    }
                }
            }
        }
        else
        {
            limitTimestamp = qMax(limitTimestamp, QDateTime::currentMSecsSinceEpoch() - (duration * ONE_DAY));

            for (int i = stock->data.size() - 1; i >= 0 && !parentThread->isInterruptionRequested(); i -= mStep)
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
                        const float fall = ((price / prevPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;

                        return QObject::tr(
                                   "Decided to buy because the price fall to %1 from %2 at %3 within last %4 days and the "
                                   "fall is %5"
                        )
                            .arg(
                                QString::number(price, 'f', stock->meta.pricePrecision) + " \u20BD",
                                QString::number(prevPrice, 'f', stock->meta.pricePrecision) + " \u20BD",
                                QDateTime::fromMSecsSinceEpoch(timestamp).toString(DATETIME_FORMAT),
                                QString::number(duration),
                                QString::number(fall, 'f', 2) + "%"
                            );
                    }
                }
            }
        }
    }

    return "";
}
// NOLINTEND(readability-function-cognitive-complexity)
