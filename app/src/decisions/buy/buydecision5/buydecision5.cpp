#include "src/decisions/buy/buydecision5/buydecision5.h"

#include <QDebug>



const char* const DATETIME_FORMAT = "yyyy-MM-dd hh:mm:ss";

constexpr qint64 MS_IN_SECOND = 1000LL;
constexpr qint64 ONE_MINUTE   = 60LL * MS_IN_SECOND;



// ====================================================================
// Buy stock if price exceeds bottom Bollindger edge
// ====================================================================
//
// ----------------------------------------------\
//                                               |
//                                               \-------\
//                                                        \
//                                                         \
//                                                          \-----\
//                                                                 \
//                                                                  \
//                                                                   \X
//

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
    bool /*isShort*/,
    float price,
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

        return makeDecisionBasedOnStockOperationalData(parentThread, buyConfig, limitTimestamp, stock);
    }

    return "";
}

AsapMode BuyDecision5::asapMode() const
{
    return ASAP_MODE_NONE;
}

QString BuyDecision5::makeDecisionBasedOnStockData(
    QThread* parentThread, IBuyDecision5Config* buyConfig, qint64 limitTimestamp, Stock* stock, int dataIndex
) const
{
    const StockData* stockData = stock->data.constData();

    const int duration = buyConfig->getDuration();
    limitTimestamp     = stockData[dataIndex].timestamp - (duration * ONE_MINUTE);

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

QString BuyDecision5::makeDecisionBasedOnStockOperationalData(
    QThread* parentThread, IBuyDecision5Config* buyConfig, qint64 limitTimestamp, Stock* stock
) const
{
    const int duration = buyConfig->getDuration();

    const StockOperationalData* stockOperationalData = stock->operational.detailedData.constData();

    limitTimestamp = QDateTime::currentMSecsSinceEpoch() - (duration * ONE_MINUTE);

    for (int i = stock->operational.detailedData.size() - 1; i >= 2 && !parentThread->isInterruptionRequested(); --i)
    {
        const qint64 timestamp = stockOperationalData[i].timestamp;

        if (timestamp < limitTimestamp)
        {
            const int startIndex = i - 2;

            const double currentBottomEdge =
                mBollindger->getBottomEdgeOperational(stock, startIndex + 2, stock->operational.detailedData.size());
            const double previousBottomEdge =
                mBollindger->getBottomEdgeOperational(stock, startIndex + 1, stock->operational.detailedData.size() - 1);
            const double beforePreviousBottomEdge =
                mBollindger->getBottomEdgeOperational(stock, startIndex, stock->operational.detailedData.size() - 2);

            if (stockOperationalData[stock->operational.detailedData.size() - 1].price > currentBottomEdge &&
                stockOperationalData[stock->operational.detailedData.size() - 2].price > previousBottomEdge &&
                stockOperationalData[stock->operational.detailedData.size() - 3].price < beforePreviousBottomEdge)
            {
                return QObject::tr("Decided to buy because the price %1 exceeds bottom Bollindger edge price %2 at %3")
                    .arg(
                        QString::number(
                            stockOperationalData[stock->operational.detailedData.size() - 3].price,
                            'f',
                            stock->meta.pricePrecision
                        ) + " \u20BD",
                        QString::number(beforePreviousBottomEdge, 'f', stock->meta.pricePrecision) + " \u20BD",
                        QDateTime::fromMSecsSinceEpoch(stockOperationalData[stock->operational.detailedData.size() - 3].timestamp)
                            .toString(DATETIME_FORMAT)
                    );
            }

            break;
        }
    }

    return "";
}
