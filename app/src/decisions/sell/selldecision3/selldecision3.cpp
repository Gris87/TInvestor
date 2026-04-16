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
    bool   isShort,
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
            return makeDecisionBasedOnStockData(parentThread, sellConfig, stock, dataIndex, isShort, price, avgPrice, commission);
        }

        return makeDecisionBasedOnStockOperationalData(parentThread, sellConfig, stock, isShort, price, avgPrice, commission);
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
    bool                  isShort,
    float                 price,
    float                 avgPrice,
    float                 commission
) const
{
    const float yield     = ((!isShort ? price / avgPrice : avgPrice / price) * HUNDRED_PERCENT) - HUNDRED_PERCENT;
    const float loseYield = -sellConfig->getLoseYield() + (2 * commission);

    if (yield <= loseYield)
    {
        const StockData* stockData = stock->data.constData();

        const float minimumPrice =
            !isShort ? avgPrice * (1 + (loseYield / HUNDRED_PERCENT)) : avgPrice / (1 + (loseYield / HUNDRED_PERCENT));

        if (doubleCheckBasedOnStockData(parentThread, stockData, dataIndex, isShort, minimumPrice))
        {
            return QObject::tr("Decided to %1 because the price %2 to %3 with yield %4 from the price %5")
                .arg(
                    !isShort ? QObject::tr("sell") : QObject::tr("buy"),
                    !isShort ? QObject::tr("fall") : QObject::tr("raise"),
                    QString::number(price, 'f', stock->meta.pricePrecision) + " \u20BD",
                    QString::number(yield, 'f', 2) + "%",
                    QString::number(avgPrice, 'f', stock->meta.pricePrecision) + " \u20BD"
                );
        }
    }

    return "";
}

QString SellDecision3::makeDecisionBasedOnStockOperationalData(
    QThread*              parentThread,
    ISellDecision3Config* sellConfig,
    Stock*                stock,
    bool                  isShort,
    float                 price,
    float                 avgPrice,
    float                 commission
) const
{
    const float yield     = ((!isShort ? price / avgPrice : avgPrice / price) * HUNDRED_PERCENT) - HUNDRED_PERCENT;
    const float loseYield = -sellConfig->getLoseYield() + (2 * commission);

    if (yield <= loseYield)
    {
        if (stock->operational.detailedData.size() > MINUTES_TO_DOUBLE_CHECK)
        {
            const StockOperationalData* stockOperationalData = stock->operational.detailedData.constData();

            const float minimumPrice =
                !isShort ? avgPrice * (1 + (loseYield / HUNDRED_PERCENT)) : avgPrice / (1 + (loseYield / HUNDRED_PERCENT));

            if (doubleCheckBasedOnStockOperationalData(
                    parentThread, stockOperationalData, stock->operational.detailedData.size() - 1, isShort, minimumPrice
                ))
            {
                return QObject::tr("Decided to %1 because the price %2 to %3 with yield %4 from the price %5")
                    .arg(
                        !isShort ? QObject::tr("sell") : QObject::tr("buy"),
                        !isShort ? QObject::tr("fall") : QObject::tr("raise"),
                        QString::number(price, 'f', stock->meta.pricePrecision) + " \u20BD",
                        QString::number(yield, 'f', 2) + "%",
                        QString::number(avgPrice, 'f', stock->meta.pricePrecision) + " \u20BD"
                    );
            }
        }
    }

    return "";
}

bool SellDecision3::doubleCheckBasedOnStockData(
    QThread* parentThread, const StockData* stockData, int index, bool isShort, float minimumPrice
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
            if (!isShort ? stockData[j].price > minimumPrice : stockData[j].price < minimumPrice)
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
    QThread* parentThread, const StockOperationalData* stockOperationalData, int index, bool isShort, float minimumPrice
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
            if (!isShort ? stockOperationalData[j].price > minimumPrice : stockOperationalData[j].price < minimumPrice)
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
