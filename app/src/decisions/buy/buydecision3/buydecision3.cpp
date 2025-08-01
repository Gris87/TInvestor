#include "src/decisions/buy/buydecision3/buydecision3.h"

#include <QDebug>



const char* const DATETIME_FORMAT = "yyyy-MM-dd hh:mm:ss";

constexpr int    MINUTES_TO_DOUBLE_CHECK = 5;
constexpr float  HUNDRED_PERCENT         = 100.0f;
constexpr qint64 MS_IN_SECOND            = 1000LL;
constexpr qint64 ONE_MINUTE              = 60LL * MS_IN_SECOND;
constexpr qint64 ONE_HOUR                = 60LL * ONE_MINUTE;
constexpr qint64 ONE_DAY                 = 24LL * ONE_HOUR;



BuyDecision3::BuyDecision3() :
    IActionDecision()
{
    qDebug() << "Create BuyDecision3";
}

BuyDecision3::~BuyDecision3()
{
    qDebug() << "Destroy BuyDecision3";
}

QString BuyDecision3::makeDecision(
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
    IBuyDecision3Config* buyConfig = config->getBuyDecision3Config();

    if (buyConfig->isEnabled())
    {
        const float priceFall    = -buyConfig->getPriceFall();
        const int   duration     = buyConfig->getDuration();
        const float maximumPrice = price / (1 + (priceFall / HUNDRED_PERCENT));

        if (dateRange)
        {
            const qint64 limitTimestamp = stock->data.at(dataIndex).timestamp - (duration * ONE_DAY);

            for (int i = dataIndex - 1; i >= 0 && !parentThread->isInterruptionRequested(); --i)
            {
                const qint64 timestamp = stock->data.at(i).timestamp;
                const float  prevPrice = stock->data.at(i).price;

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
                        if (stock->data.at(j).price < maximumPrice)
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
                                   "Decided to buy because the price fall to %1 from %2 at %3 within last %4 minutes and the "
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
            const qint64 limitTimestamp = QDateTime::currentMSecsSinceEpoch() - (duration * ONE_DAY);

            for (int i = stock->operational.detailedData.size() - 2; i >= 0 && !parentThread->isInterruptionRequested(); --i)
            {
                const qint64 timestamp = stock->operational.detailedData.at(i).timestamp;
                const float  prevPrice = stock->operational.detailedData.at(i).price;

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
                        if (stock->operational.detailedData.at(j).price < maximumPrice)
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
                                   "Decided to buy because the price fall to %1 from %2 at %3 within last %4 minutes and the "
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

            for (int i = stock->data.size() - 1; i >= 0 && !parentThread->isInterruptionRequested(); --i)
            {
                const qint64 timestamp = stock->data.at(i).timestamp;
                const float  prevPrice = stock->data.at(i).price;

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
                        if (stock->data.at(j).price < maximumPrice)
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
                                   "Decided to buy because the price fall to %1 from %2 at %3 within last %4 minutes and the "
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
