#include "src/decisions/sell/selldecision2/selldecision2.h"

#include <QDebug>



const char* const DATETIME_FORMAT = "yyyy-MM-dd hh:mm:ss";

constexpr double INCREDIBLE_SELL_COEF = 3.0;
constexpr float  HUNDRED_PERCENT      = 100.0f;



SellDecision2::SellDecision2() :
    IActionDecision()
{
    qDebug() << "Create SellDecision2";
}

SellDecision2::~SellDecision2()
{
    qDebug() << "Destroy SellDecision2";
}

// NOLINTBEGIN(readability-function-cognitive-complexity)
QString SellDecision2::makeDecision(
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

    ISellDecision2Config* sellConfig = config->getSellDecision2Config();

    if (sellConfig->isEnabled())
    {
        if (dateRange)
        {
            return makeDecisionBasedOnStockData(parentThread, sellConfig, stock, dataIndex, price, avgPrice, commission);
        }

        return makeDecisionBasedOnStockOperationalData(parentThread, sellConfig, stock, price, avgPrice, commission);
    }

    return "";
}
// NOLINTEND(readability-function-cognitive-complexity)

AsapMode SellDecision2::asapMode() const
{
    return ASAP_MODE_NONE;
}

QString SellDecision2::makeDecisionBasedOnStockData(
    QThread*              parentThread,
    ISellDecision2Config* sellConfig,
    Stock*                stock,
    int                   dataIndex,
    float                 price,
    float                 avgPrice,
    float                 commission
)
{
    const float coef = price / avgPrice;

    if (coef < INCREDIBLE_SELL_COEF)
    {
        const float yield      = (coef * HUNDRED_PERCENT) - HUNDRED_PERCENT;
        const float yieldAbove = sellConfig->getYieldAbove() + (2 * commission);

        if (yield >= yieldAbove)
        {
            const float loseYield    = -sellConfig->getLoseYield();
            const float minimumPrice = avgPrice * (1 + (yieldAbove / HUNDRED_PERCENT));
            const float maximumPrice = price / (1 + (loseYield / HUNDRED_PERCENT));

            const StockData* stockData = stock->data.constData();

            for (int i = dataIndex; i >= 0 && !parentThread->isInterruptionRequested(); --i)
            {
                const float prevPrice = stockData[i].price;

                if (prevPrice < minimumPrice)
                {
                    break;
                }

                if (prevPrice >= maximumPrice)
                {
                    const float lostYield = ((price / prevPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;

                    return QObject::tr(
                               "Decided to sell because the price reached %1 with yield %2 from the price %3 and lost "
                               "yield %4 from the maximum price %5 at %6"
                    )
                        .arg(
                            QString::number(price, 'f', stock->meta.pricePrecision) + " \u20BD",
                            "+" + QString::number(yield, 'f', 2) + "%",
                            QString::number(avgPrice, 'f', stock->meta.pricePrecision) + " \u20BD",
                            QString::number(lostYield, 'f', 2) + "%",
                            QString::number(prevPrice, 'f', stock->meta.pricePrecision) + " \u20BD",
                            QDateTime::fromMSecsSinceEpoch(stockData[i].timestamp).toString(DATETIME_FORMAT)
                        );
                }
            }
        }
    }

    return "";
}

QString SellDecision2::makeDecisionBasedOnStockOperationalData(
    QThread* parentThread, ISellDecision2Config* sellConfig, Stock* stock, float price, float avgPrice, float commission
)
{
    const float coef = price / avgPrice;

    if (coef < INCREDIBLE_SELL_COEF)
    {
        const float yield      = (coef * HUNDRED_PERCENT) - HUNDRED_PERCENT;
        const float yieldAbove = sellConfig->getYieldAbove() + (2 * commission);

        if (yield >= yieldAbove)
        {
            const float loseYield    = -sellConfig->getLoseYield();
            const float minimumPrice = avgPrice * (1 + (yieldAbove / HUNDRED_PERCENT));
            const float maximumPrice = price / (1 + (loseYield / HUNDRED_PERCENT));

            const StockOperationalData* stockOperationalData = stock->operational.detailedData.constData();

            for (int i = stock->operational.detailedData.size() - 1; i >= 0 && !parentThread->isInterruptionRequested(); --i)
            {
                const float prevPrice = stockOperationalData[i].price;

                if (prevPrice < minimumPrice)
                {
                    break;
                }

                if (prevPrice >= maximumPrice)
                {
                    const float lostYield = ((price / prevPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;

                    return QObject::tr(
                               "Decided to sell because the price reached %1 with yield %2 from the price %3 and lost "
                               "yield %4 from the maximum price %5 at %6"
                    )
                        .arg(
                            QString::number(price, 'f', stock->meta.pricePrecision) + " \u20BD",
                            "+" + QString::number(yield, 'f', 2) + "%",
                            QString::number(avgPrice, 'f', stock->meta.pricePrecision) + " \u20BD",
                            QString::number(lostYield, 'f', 2) + "%",
                            QString::number(prevPrice, 'f', stock->meta.pricePrecision) + " \u20BD",
                            QDateTime::fromMSecsSinceEpoch(stockOperationalData[i].timestamp).toString(DATETIME_FORMAT)
                        );
                }
            }
        }
    }

    return "";
}
