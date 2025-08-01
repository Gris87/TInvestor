#include "src/decisions/sell/selldecision3/selldecision3.h"

#include <QDebug>



const char* const DATETIME_FORMAT = "yyyy-MM-dd hh:mm:ss";

constexpr float  HUNDRED_PERCENT = 100.0f;
constexpr qint64 MS_IN_SECOND    = 1000LL;
constexpr qint64 ONE_MINUTE      = 60LL * MS_IN_SECOND;



SellDecision3::SellDecision3() :
    IActionDecision()
{
    qDebug() << "Create SellDecision3";
}

SellDecision3::~SellDecision3()
{
    qDebug() << "Destroy SellDecision3";
}

QString SellDecision3::makeDecision(
    IDecisionMakerConfig* config, Stock* stock, bool dateRange, int dataIndex, float price, float avgPrice, float commission
)
{
    ISellDecision3Config* sellConfig = config->getSellDecision3Config();

    if (sellConfig->isEnabled())
    {
        const float loseYield    = -sellConfig->getLoseYield() + (2 * commission);
        const int   duration     = sellConfig->getDuration();
        const float maximumPrice = price / (1 + (loseYield / HUNDRED_PERCENT));

        if (dateRange)
        {
            const qint64 limitTimestamp = stock->data.at(dataIndex).timestamp - (duration * ONE_MINUTE);

            for (int i = dataIndex - 1; i >= 0; --i)
            {
                const qint64 timestamp = stock->data.at(i).timestamp;
                const float  prevPrice = stock->data.at(i).price;

                if (timestamp < limitTimestamp)
                {
                    return "";
                }

                if (prevPrice >= maximumPrice)
                {
                    const float yield     = ((price / avgPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;
                    const float lostYield = ((price / prevPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;

                    return QObject::tr(
                               "Decided to sell because the price reached %1 with yield %2 from the price %3 and lost "
                               "yield %4 from the price %5 at %6 within last %7 minutes"
                    )
                        .arg(
                            QString::number(price, 'f', stock->meta.pricePrecision) + " \u20BD",
                            QString::number(yield, 'f', 2) + "%",
                            QString::number(avgPrice, 'f', stock->meta.pricePrecision) + " \u20BD",
                            QString::number(lostYield, 'f', 2) + "%",
                            QString::number(prevPrice, 'f', stock->meta.pricePrecision) + " \u20BD",
                            QDateTime::fromMSecsSinceEpoch(timestamp).toString(DATETIME_FORMAT),
                            QString::number(duration)
                        );
                }
            }
        }
        else
        {
            const qint64 limitTimestamp = QDateTime::currentMSecsSinceEpoch() - (duration * ONE_MINUTE);

            for (int i = stock->operational.detailedData.size() - 2; i >= 0; --i)
            {
                const qint64 timestamp = stock->operational.detailedData.at(i).timestamp;
                const float  prevPrice = stock->operational.detailedData.at(i).price;

                if (timestamp < limitTimestamp)
                {
                    return "";
                }

                if (prevPrice >= maximumPrice)
                {
                    const float yield     = ((price / avgPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;
                    const float lostYield = ((price / prevPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;

                    return QObject::tr(
                               "Decided to sell because the price reached %1 with yield %2 from the price %3 and lost "
                               "yield %4 from the price %5 at %6 within last %7 minutes"
                    )
                        .arg(
                            QString::number(price, 'f', stock->meta.pricePrecision) + " \u20BD",
                            QString::number(yield, 'f', 2) + "%",
                            QString::number(avgPrice, 'f', stock->meta.pricePrecision) + " \u20BD",
                            QString::number(lostYield, 'f', 2) + "%",
                            QString::number(prevPrice, 'f', stock->meta.pricePrecision) + " \u20BD",
                            QDateTime::fromMSecsSinceEpoch(timestamp).toString(DATETIME_FORMAT),
                            QString::number(duration)
                        );
                }
            }

            for (int i = stock->data.size() - 1; i >= 0; --i)
            {
                const qint64 timestamp = stock->data.at(i).timestamp;
                const float  prevPrice = stock->data.at(i).price;

                if (timestamp < limitTimestamp)
                {
                    return "";
                }

                if (prevPrice >= maximumPrice)
                {
                    const float yield     = ((price / avgPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;
                    const float lostYield = ((price / prevPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;

                    return QObject::tr(
                               "Decided to sell because the price reached %1 with yield %2 from the price %3 and lost "
                               "yield %4 from the price %5 at %6 within last %7 minutes"
                    )
                        .arg(
                            QString::number(price, 'f', stock->meta.pricePrecision) + " \u20BD",
                            QString::number(yield, 'f', 2) + "%",
                            QString::number(avgPrice, 'f', stock->meta.pricePrecision) + " \u20BD",
                            QString::number(lostYield, 'f', 2) + "%",
                            QString::number(prevPrice, 'f', stock->meta.pricePrecision) + " \u20BD",
                            QDateTime::fromMSecsSinceEpoch(timestamp).toString(DATETIME_FORMAT),
                            QString::number(duration)
                        );
                }
            }
        }
    }

    return "";
}
