#include "src/decisions/sell/selldecision3/selldecision3.h"

#include <QDebug>



constexpr int   MINUTES_TO_DOUBLE_CHECK = 5;
constexpr float HUNDRED_PERCENT         = 100.0f;



// ====================================================================
// Sell stock if price fall too much
// ====================================================================
//
// --------------------------------------------\
//                                             |
//                                             \-------\
//                                                      \
//                                                       \
//                                                        \-----\
//                                                               \
//                                                                \
//                                                                 \--X
//

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
        if (dateRange)
        {
            return makeDecisionBasedOnStockData(parentThread, sellConfig, stock, dataIndex, price, avgPrice, commission);
        }

        return makeDecisionBasedOnStockOperationalData(parentThread, sellConfig, stock, price, avgPrice, commission);
    }

    return "";
}
// NOLINTEND(readability-function-cognitive-complexity)

AsapMode SellDecision3::asapMode() const
{
    return ASAP_MODE_FOLLOW_PRICE;
}

QString SellDecision3::makeDecisionBasedOnStockData(
    QThread*              parentThread,
    ISellDecision3Config* sellConfig,
    Stock*                stock,
    int                   dataIndex,
    float                 price,
    float                 avgPrice,
    float                 commission
) const
{
    const float yield     = ((price / avgPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;
    const float loseYield = -sellConfig->getLoseYield() + (2 * commission);

    if (yield <= loseYield)
    {
        const StockData* stockData    = stock->data.constData();
        const float      minimumPrice = avgPrice * (1 + (loseYield / HUNDRED_PERCENT));

        if (doubleCheckBasedOnStockData(parentThread, stockData, dataIndex, minimumPrice))
        {
            return QObject::tr("Decided to sell because the price fall to %1 with yield %2 from the price %3")
                .arg(
                    QString::number(price, 'f', stock->meta.pricePrecision) + " \u20BD",
                    QString::number(yield, 'f', 2) + "%",
                    QString::number(avgPrice, 'f', stock->meta.pricePrecision) + " \u20BD"
                );
        }
    }

    return "";
}

QString SellDecision3::makeDecisionBasedOnStockOperationalData(
    QThread* parentThread, ISellDecision3Config* sellConfig, Stock* stock, float price, float avgPrice, float commission
) const
{
    const float yield     = ((price / avgPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;
    const float loseYield = -sellConfig->getLoseYield() + (2 * commission);

    if (yield <= loseYield)
    {
        if (stock->operational.detailedData.size() > MINUTES_TO_DOUBLE_CHECK)
        {
            const StockOperationalData* stockOperationalData = stock->operational.detailedData.constData();
            const float                 minimumPrice         = avgPrice * (1 + (loseYield / HUNDRED_PERCENT));

            if (doubleCheckBasedOnStockOperationalData(
                    parentThread, stockOperationalData, stock->operational.detailedData.size() - 1, minimumPrice
                ))
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

    return "";
}

bool
SellDecision3::doubleCheckBasedOnStockData(QThread* parentThread, const StockData* stockData, int index, float minimumPrice) const
{
    bool res = false;

    if (index >= MINUTES_TO_DOUBLE_CHECK - 1)
    {
        res = true;

        int j           = index;
        int minutesLeft = MINUTES_TO_DOUBLE_CHECK;

        while (j >= 0 && minutesLeft > 0 && !parentThread->isInterruptionRequested())
        {
            if (stockData[j].price > minimumPrice)
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

bool SellDecision3::doubleCheckBasedOnStockOperationalData(
    QThread* parentThread, const StockOperationalData* stockOperationalData, int index, float minimumPrice
) const
{
    bool res = false;

    if (index >= MINUTES_TO_DOUBLE_CHECK - 1)
    {
        res = true;

        int j           = index;
        int minutesLeft = MINUTES_TO_DOUBLE_CHECK;

        while (j >= 0 && minutesLeft > 0 && !parentThread->isInterruptionRequested())
        {
            if (stockOperationalData[j].price > minimumPrice)
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
