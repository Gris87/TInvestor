#include "src/decisions/sell/selldecision3/selldecision3.h"

#include <QDebug>



constexpr int   MINUTES_TO_DOUBLE_CHECK = 5;
constexpr float HUNDRED_PERCENT         = 100.0f;



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
    qint64 /*limitTimestamp*/,
    Stock* stock,
    bool   dateRange,
    int    dataIndex,
    float  price,
    float  avgPrice,
    float  commission
)
{
    Q_ASSERT_X(parentThread != nullptr, __FUNCTION__, "parentThread is invalid");
    Q_ASSERT_X(config != nullptr, __FUNCTION__, "config is invalid");
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
        const float yield     = ((price / avgPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;
        const float loseYield = -sellConfig->getLoseYield() + (2 * commission);

        if (yield <= loseYield)
        {
            if (dateRange)
            {
                const StockData* stockData = stock->data.constData();

                bool good = true;

                int j           = dataIndex;
                int minutesLeft = MINUTES_TO_DOUBLE_CHECK;

                while (j >= 0 && minutesLeft > 0 && !parentThread->isInterruptionRequested())
                {
                    const float prevPrice = stockData[j].price;
                    const float prevYield = ((prevPrice / avgPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;

                    if (prevYield > loseYield)
                    {
                        good = false;

                        break;
                    }

                    --j;
                    --minutesLeft;
                }

                if (good)
                {
                    return QObject::tr("Decided to sell because the price fall to %1 with yield %2 from the price %3")
                        .arg(
                            QString::number(price, 'f', stock->meta.pricePrecision) + " \u20BD",
                            QString::number(yield, 'f', 2) + "%",
                            QString::number(avgPrice, 'f', stock->meta.pricePrecision) + " \u20BD"
                        );
                }
            }
            else
            {
                if (stock->operational.detailedData.size() > MINUTES_TO_DOUBLE_CHECK)
                {
                    const StockOperationalData* stockOperationalData = stock->operational.detailedData.constData();

                    bool good = true;

                    int j           = stock->operational.detailedData.size() - 1;
                    int minutesLeft = MINUTES_TO_DOUBLE_CHECK;

                    while (j >= 0 && minutesLeft > 0 && !parentThread->isInterruptionRequested())
                    {
                        const float prevPrice = stockOperationalData[j].price;
                        const float prevYield = ((prevPrice / avgPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;

                        if (prevYield > loseYield)
                        {
                            good = false;

                            break;
                        }

                        --j;
                        --minutesLeft;
                    }

                    if (good)
                    {
                        return QObject::tr("Decided to sell because the price fall to %1 with yield %2 from the price %3")
                            .arg(
                                QString::number(price, 'f', stock->meta.pricePrecision) + " \u20BD",
                                QString::number(yield, 'f', 2) + "%",
                                QString::number(avgPrice, 'f', stock->meta.pricePrecision) + " \u20BD"
                            );
                    }
                }
            }
        }
    }

    return "";
}
// NOLINTEND(readability-function-cognitive-complexity)

AsapMode SellDecision3::asapMode() const
{
    return ASAP_MODE_FOLLOW_PRICE;
}
