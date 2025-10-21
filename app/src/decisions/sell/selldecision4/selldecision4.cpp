#include "src/decisions/sell/selldecision4/selldecision4.h"

#include <QDebug>



const char* const DATETIME_FORMAT = "yyyy-MM-dd hh:mm:ss";

constexpr double INCREDIBLE_SELL_COEF = 3.0;
constexpr float  HUNDRED_PERCENT      = 100.0f;
constexpr qint64 MS_IN_SECOND         = 1000LL;
constexpr qint64 ONE_MINUTE           = 60LL * MS_IN_SECOND;



SellDecision4::SellDecision4(IBollindger* bollindger) :
    IActionDecision(),
    mBollindger(bollindger)
{
    qDebug() << "Create SellDecision4";
}

SellDecision4::~SellDecision4()
{
    qDebug() << "Destroy SellDecision4";
}

QString SellDecision4::makeDecision(
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

    ISellDecision4Config* sellConfig = config->getSellDecision4Config();

    if (sellConfig->isEnabled())
    {
        if (dateRange)
        {
            return makeDecisionBasedOnStockData(
                parentThread, sellConfig, limitTimestamp, stock, dataIndex, price, avgPrice, commission
            );
        }
    }

    return "";
}

// NOLINTBEGIN(readability-function-cognitive-complexity)
QString SellDecision4::makeDecisionBasedOnStockData(
    QThread*              parentThread,
    ISellDecision4Config* sellConfig,
    qint64                limitTimestamp,
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
            const int duration = sellConfig->getDuration();

            const StockData* stockData = stock->data.constData();

            const qint64 currentTimestamp = stockData[dataIndex].timestamp;
            limitTimestamp                = currentTimestamp - (duration * ONE_MINUTE);

            for (int i = dataIndex; i >= 2 && !parentThread->isInterruptionRequested(); --i)
            {
                const qint64 timestamp = stockData[i].timestamp;

                if (timestamp < limitTimestamp)
                {
                    const int startIndex = i - 2;

                    const double currentTopEdge        = mBollindger->getTopEdge(stock, startIndex + 2, dataIndex + 1);
                    const double previousTopEdge       = mBollindger->getTopEdge(stock, startIndex + 1, dataIndex);
                    const double beforePreviousTopEdge = mBollindger->getTopEdge(stock, startIndex, dataIndex - 1);

                    if (stockData[dataIndex].price < currentTopEdge && stockData[dataIndex - 1].price < previousTopEdge &&
                        stockData[dataIndex - 2].price > beforePreviousTopEdge)
                    {
                        return QObject::tr("Decided to sell because the price %1 exceeds top Bollindger edge price %2 at %3")
                            .arg(
                                QString::number(stockData[dataIndex - 2].price, 'f', stock->meta.pricePrecision) + " \u20BD",
                                QString::number(beforePreviousTopEdge, 'f', stock->meta.pricePrecision) + " \u20BD",
                                QDateTime::fromMSecsSinceEpoch(stockData[dataIndex - 2].timestamp).toString(DATETIME_FORMAT)
                            );
                    }

                    break;
                }
            }
        }
    }

    return "";
}
// NOLINTEND(readability-function-cognitive-complexity)

AsapMode SellDecision4::asapMode() const
{
    return ASAP_MODE_NONE;
}
