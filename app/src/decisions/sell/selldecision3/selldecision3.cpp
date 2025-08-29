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

// NOLINTBEGIN(readability-function-cognitive-complexity)
QString SellDecision3::makeDecision(
    QThread*              parentThread,
    IDecisionMakerConfig* config,
    qint64                limitTimestamp,
    Stock*                stock,
    bool                  dateRange,
    int                   dataIndex,
    float                 price,
    float                 avgPrice,
    float                 commission
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
    Q_ASSERT_X(avgPrice > 0, __FUNCTION__, "avgPrice is invalid");
    Q_ASSERT_X(commission > 0, __FUNCTION__, "commission is invalid");

    ISellDecision3Config* sellConfig = config->getSellDecision3Config();

    if (sellConfig->isEnabled())
    {
        const float loseYield    = -sellConfig->getLoseYield() + (2 * commission);
        const int   duration     = sellConfig->getDuration();
        const float maximumPrice = price / (1 + (loseYield / HUNDRED_PERCENT));

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
                    const float yield     = ((price / avgPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;
                    const float lostYield = ((price / prevPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;

                    return QObject::tr(
                               "Decided to sell because the price reached %1 with yield %2 from the price %3 and lost "
                               "yield %4 from the price %5 at %6 within last %7 minutes"
                    )
                        .arg(
                            QString::number(price, 'f', stock->meta.pricePrecision) + " \u20BD",
                            (yield > 0 ? "+" : "") + QString::number(yield, 'f', 2) + "%",
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
            limitTimestamp = qMax(limitTimestamp, QDateTime::currentMSecsSinceEpoch() - (duration * ONE_MINUTE));

            const StockOperationalData* stockOperationalData = stock->operational.detailedData.constData();

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
                    const float yield     = ((price / avgPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;
                    const float lostYield = ((price / prevPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;

                    return QObject::tr(
                               "Decided to sell because the price reached %1 with yield %2 from the price %3 and lost "
                               "yield %4 from the price %5 at %6 within last %7 minutes"
                    )
                        .arg(
                            QString::number(price, 'f', stock->meta.pricePrecision) + " \u20BD",
                            (yield > 0 ? "+" : "") + QString::number(yield, 'f', 2) + "%",
                            QString::number(avgPrice, 'f', stock->meta.pricePrecision) + " \u20BD",
                            QString::number(lostYield, 'f', 2) + "%",
                            QString::number(prevPrice, 'f', stock->meta.pricePrecision) + " \u20BD",
                            QDateTime::fromMSecsSinceEpoch(timestamp).toString(DATETIME_FORMAT),
                            QString::number(duration)
                        );
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
                    const float yield     = ((price / avgPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;
                    const float lostYield = ((price / prevPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;

                    return QObject::tr(
                               "Decided to sell because the price reached %1 with yield %2 from the price %3 and lost "
                               "yield %4 from the price %5 at %6 within last %7 minutes"
                    )
                        .arg(
                            QString::number(price, 'f', stock->meta.pricePrecision) + " \u20BD",
                            (yield > 0 ? "+" : "") + QString::number(yield, 'f', 2) + "%",
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
// NOLINTEND(readability-function-cognitive-complexity)
