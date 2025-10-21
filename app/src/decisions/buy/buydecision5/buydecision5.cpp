#include "src/decisions/buy/buydecision5/buydecision5.h"

#include <QDebug>



const char* const DATETIME_FORMAT = "yyyy-MM-dd hh:mm:ss";

constexpr qint64 MS_IN_SECOND = 1000LL;
constexpr qint64 ONE_MINUTE   = 60LL * MS_IN_SECOND;



BuyDecision5::BuyDecision5(IBollindger* bollindger) :
    IActionDecision(),
    mBollindger(bollindger)
{
    qDebug() << "Create BuyDecision5";
}

BuyDecision5::~BuyDecision5()
{
    qDebug() << "Destroy BuyDecision5";
}

QString BuyDecision5::makeDecision(
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

    IBuyDecision5Config* buyConfig = config->getBuyDecision5Config();

    if (buyConfig->isEnabled())
    {
        if (dateRange)
        {
            return makeDecisionBasedOnStockData(parentThread, buyConfig, limitTimestamp, stock, dataIndex);
        }
    }

    return "";
}

AsapMode BuyDecision5::asapMode() const
{
    return ASAP_MODE_NONE;
}

// NOLINTBEGIN(readability-function-cognitive-complexity)
QString BuyDecision5::makeDecisionBasedOnStockData(
    QThread* parentThread, IBuyDecision5Config* buyConfig, qint64 limitTimestamp, Stock* stock, int dataIndex
)
{
    const int duration = buyConfig->getDuration();

    const StockData* stockData = stock->data.constData();

    const qint64 currentTimestamp = stockData[dataIndex].timestamp;
    limitTimestamp                = currentTimestamp - (duration * ONE_MINUTE);

    for (int i = dataIndex; i >= 2 && !parentThread->isInterruptionRequested(); --i)
    {
        const qint64 timestamp = stockData[i].timestamp;

        if (timestamp < limitTimestamp)
        {
            const int startIndex = i - 2;

            const double currentBottomEdge        = mBollindger->getBottomEdge(stock, startIndex + 2, dataIndex + 1);
            const double previousBottomEdge       = mBollindger->getBottomEdge(stock, startIndex + 1, dataIndex);
            const double beforePreviousBottomEdge = mBollindger->getBottomEdge(stock, startIndex, dataIndex - 1);

            if (stockData[dataIndex].price > currentBottomEdge && stockData[dataIndex - 1].price > previousBottomEdge &&
                stockData[dataIndex - 2].price < beforePreviousBottomEdge)
            {
                return QObject::tr("Decided to buy because the price %1 exceeds bottom Bollindger edge price %2 at %3")
                    .arg(
                        QString::number(stockData[dataIndex - 2].price, 'f', stock->meta.pricePrecision) + " \u20BD",
                        QString::number(beforePreviousBottomEdge, 'f', stock->meta.pricePrecision) + " \u20BD",
                        QDateTime::fromMSecsSinceEpoch(stockData[dataIndex - 2].timestamp).toString(DATETIME_FORMAT)
                    );
            }

            break;
        }
    }

    return "";
}
// NOLINTEND(readability-function-cognitive-complexity)
