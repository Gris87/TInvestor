#include "src/decisions/buy/buydecision4/buydecision4.h"

#include <QDebug>



const char* const DATETIME_FORMAT = "yyyy-MM-dd hh:mm:ss";

constexpr int    MINUTES_TO_DOUBLE_CHECK = 5;
constexpr float  HUNDRED_PERCENT         = 100.0f;
constexpr qint64 MS_IN_SECOND            = 1000LL;
constexpr qint64 ONE_MINUTE              = 60LL * MS_IN_SECOND;
constexpr qint64 ONE_HOUR                = 60LL * ONE_MINUTE;
constexpr qint64 ONE_DAY                 = 24LL * ONE_HOUR;



BuyDecision4::BuyDecision4() :
    IActionDecision()
{
    qDebug() << "Create BuyDecision4";
}

BuyDecision4::~BuyDecision4()
{
    qDebug() << "Destroy BuyDecision4";
}

// NOLINTBEGIN(readability-function-cognitive-complexity)
QString BuyDecision4::makeDecision(
    QThread*              parentThread,
    IDecisionMakerConfig* config,
    Stock*                stock,
    bool                  dateRange,
    int                   dataIndex,
    float                 price,
    float /*avgPrice*/,
    float /*commission*/
)
{
    IBuyDecision4Config* buyConfig = config->getBuyDecision4Config();

    if (buyConfig->isEnabled())
    {
        const float priceFall    = -buyConfig->getPriceFall();
        const float loseYield    = buyConfig->getLoseYield();
        const int   duration     = buyConfig->getDuration();
        const float maximumPrice = price / (1 + (priceFall / HUNDRED_PERCENT));

        StockData* stockData = stock->data.data();

        if (dateRange)
        {
            const qint64 limitTimestamp = stockData[dataIndex].timestamp - (duration * ONE_DAY);

            for (int i = dataIndex - 1; i >= 0 && !parentThread->isInterruptionRequested(); --i)
            {
                const qint64 timestamp = stockData[i].timestamp;
                const float  prevPrice = stockData[i].price;

                if (timestamp < limitTimestamp)
                {
                    return "";
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
                        const float minimumPrice = price / (1 + (loseYield / HUNDRED_PERCENT));

                        for (j = dataIndex - 1; j >= 0 && !parentThread->isInterruptionRequested(); --j)
                        {
                            const float prevPrice2 = stockData[j].price;

                            if (prevPrice2 >= price)
                            {
                                return "";
                            }

                            if (prevPrice2 <= minimumPrice)
                            {
                                const float fall      = ((price / prevPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;
                                const float lostYield = ((price / prevPrice2) * HUNDRED_PERCENT) - HUNDRED_PERCENT;

                                return QObject::tr(
                                           "Decided to buy because the price fall to %1 from %2 at %3 and lost "
                                           "yield %4 from the minimum price %5 at %6 within last %7 days and the fall is %8"
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
        else
        {
            const qint64 limitTimestamp = QDateTime::currentMSecsSinceEpoch() - (duration * ONE_DAY);

            StockOperationalData* stockOperationalData = stock->operational.detailedData.data();

            for (int i = stock->operational.detailedData.size() - 2; i >= 0 && !parentThread->isInterruptionRequested(); --i)
            {
                const qint64 timestamp = stockOperationalData[i].timestamp;
                const float  prevPrice = stockOperationalData[i].price;

                if (timestamp < limitTimestamp)
                {
                    return "";
                }

                if (prevPrice >= maximumPrice)
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
                        const float minimumPrice = price / (1 + (loseYield / HUNDRED_PERCENT));

                        for (j = stock->operational.detailedData.size() - 2; j >= 0 && !parentThread->isInterruptionRequested();
                             --j)
                        {
                            const float prevPrice2 = stockOperationalData[j].price;

                            if (prevPrice2 >= price)
                            {
                                return "";
                            }

                            if (prevPrice2 <= minimumPrice)
                            {
                                const float fall      = ((price / prevPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;
                                const float lostYield = ((price / prevPrice2) * HUNDRED_PERCENT) - HUNDRED_PERCENT;

                                return QObject::tr(
                                           "Decided to buy because the price fall to %1 from %2 at %3 and lost "
                                           "yield %4 from the minimum price %5 at %6 within last %7 days and the fall is %8"
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

            for (int i = stock->data.size() - 1; i >= 0 && !parentThread->isInterruptionRequested(); --i)
            {
                const qint64 timestamp = stockData[i].timestamp;
                const float  prevPrice = stockData[i].price;

                if (timestamp < limitTimestamp)
                {
                    return "";
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
                        const float minimumPrice = price / (1 + (loseYield / HUNDRED_PERCENT));

                        for (j = stock->data.size() - 1; j >= 0 && !parentThread->isInterruptionRequested(); --j)
                        {
                            const float prevPrice2 = stockData[j].price;

                            if (prevPrice2 >= price)
                            {
                                return "";
                            }

                            if (prevPrice2 <= minimumPrice)
                            {
                                const float fall      = ((price / prevPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;
                                const float lostYield = ((price / prevPrice2) * HUNDRED_PERCENT) - HUNDRED_PERCENT;

                                return QObject::tr(
                                           "Decided to buy because the price fall to %1 from %2 at %3 and lost "
                                           "yield %4 from the minimum price %5 at %6 within last %7 days and the fall is %8"
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
    }

    return "";
}
// NOLINTEND(readability-function-cognitive-complexity)
