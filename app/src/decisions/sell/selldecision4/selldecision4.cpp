#include "src/decisions/sell/selldecision4/selldecision4.h"

#include <QDebug>



const char* const DATETIME_FORMAT = "yyyy-MM-dd hh:mm:ss";

constexpr double INCREDIBLE_SELL_COEF = 3.0;
constexpr float  HUNDRED_PERCENT      = 100.0f;
constexpr qint64 MS_IN_SECOND         = 1000LL;
constexpr qint64 ONE_MINUTE           = 60LL * MS_IN_SECOND;



// ====================================================================
// Sell stock if price exceeds top Bollindger edge with positive yield
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
    bool                  isShort,
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
                parentThread, sellConfig, limitTimestamp, stock, dataIndex, isShort, price, avgPrice, commission
            );
        }

        return makeDecisionBasedOnStockOperationalData(
            parentThread, sellConfig, limitTimestamp, stock, isShort, price, avgPrice, commission
        );
    }

    return "";
}

AsapMode SellDecision4::asapMode() const
{
    return ASAP_MODE_NONE;
}

// NOLINTBEGIN(readability-function-cognitive-complexity)
QString SellDecision4::makeDecisionBasedOnStockData(
    QThread*              parentThread,
    ISellDecision4Config* sellConfig,
    qint64                limitTimestamp,
    Stock*                stock,
    int                   dataIndex,
    bool                  isShort,
    float                 price,
    float                 avgPrice,
    float                 commission
) const
{
    const float coef = !isShort ? price / avgPrice : avgPrice / price;

    if (coef < INCREDIBLE_SELL_COEF)
    {
        const float yield      = (coef * HUNDRED_PERCENT) - HUNDRED_PERCENT;
        const float yieldAbove = sellConfig->getYieldAbove() + (2 * commission);

        if (yield >= yieldAbove)
        {
            const StockData* stockData = stock->data.constData();

            const int duration = sellConfig->getDuration();
            limitTimestamp     = stockData[dataIndex].timestamp - (duration * ONE_MINUTE);

            for (int i = dataIndex; i >= 2 && !parentThread->isInterruptionRequested(); --i)
            {
                const qint64 timestamp = stockData[i].timestamp;

                if (timestamp < limitTimestamp)
                {
                    const int startIndex = i - 2;

                    const double currentTopEdge  = !isShort ? mBollindger->getTopEdge(stock, startIndex + 2, dataIndex + 1)
                                                            : mBollindger->getBottomEdge(stock, startIndex + 2, dataIndex + 1);
                    const double previousTopEdge = !isShort ? mBollindger->getTopEdge(stock, startIndex + 1, dataIndex)
                                                            : mBollindger->getBottomEdge(stock, startIndex + 1, dataIndex);
                    const double beforePreviousTopEdge = !isShort ? mBollindger->getTopEdge(stock, startIndex, dataIndex - 1)
                                                                  : mBollindger->getBottomEdge(stock, startIndex, dataIndex - 1);

                    if (!isShort
                            ? stockData[dataIndex].price < currentTopEdge && stockData[dataIndex - 1].price < previousTopEdge &&
                                  stockData[dataIndex - 2].price > beforePreviousTopEdge
                            : stockData[dataIndex].price > currentTopEdge && stockData[dataIndex - 1].price > previousTopEdge &&
                                  stockData[dataIndex - 2].price < beforePreviousTopEdge)
                    {
                        return QObject::tr("Decided to %1 because the price %2 exceeds %3 Bollindger edge price %4 at %5")
                            .arg(
                                !isShort ? QObject::tr("sell") : QObject::tr("buy"),
                                QString::number(stockData[dataIndex - 2].price, 'f', stock->meta.pricePrecision) + " \u20BD",
                                !isShort ? QObject::tr("top") : QObject::tr("bottom"),
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

// NOLINTBEGIN(readability-function-cognitive-complexity)
QString SellDecision4::makeDecisionBasedOnStockOperationalData(
    QThread*              parentThread,
    ISellDecision4Config* sellConfig,
    qint64                limitTimestamp,
    Stock*                stock,
    bool                  isShort,
    float                 price,
    float                 avgPrice,
    float                 commission
) const
{
    const float coef = !isShort ? price / avgPrice : avgPrice / price;

    if (coef < INCREDIBLE_SELL_COEF)
    {
        const float yield      = (coef * HUNDRED_PERCENT) - HUNDRED_PERCENT;
        const float yieldAbove = sellConfig->getYieldAbove() + (2 * commission);

        if (yield >= yieldAbove)
        {
            const StockOperationalData* stockOperationalData = stock->operational.detailedData.constData();

            const int duration = sellConfig->getDuration();
            limitTimestamp     = QDateTime::currentMSecsSinceEpoch() - (duration * ONE_MINUTE);

            for (int i = stock->operational.detailedData.size() - 1; i >= 2 && !parentThread->isInterruptionRequested(); --i)
            {
                const qint64 timestamp = stockOperationalData[i].timestamp;

                if (timestamp < limitTimestamp)
                {
                    const int startIndex = i - 2;

                    const double currentTopEdge =
                        !isShort
                            ? mBollindger->getTopEdgeOperational(stock, startIndex + 2, stock->operational.detailedData.size())
                            : mBollindger->getBottomEdgeOperational(
                                  stock, startIndex + 2, stock->operational.detailedData.size()
                              );
                    const double previousTopEdge = !isShort
                                                       ? mBollindger->getTopEdgeOperational(
                                                             stock, startIndex + 1, stock->operational.detailedData.size() - 1
                                                         )
                                                       : mBollindger->getBottomEdgeOperational(
                                                             stock, startIndex + 1, stock->operational.detailedData.size() - 1
                                                         );
                    const double beforePreviousTopEdge =
                        !isShort
                            ? mBollindger->getTopEdgeOperational(stock, startIndex, stock->operational.detailedData.size() - 2)
                            : mBollindger->getBottomEdgeOperational(
                                  stock, startIndex, stock->operational.detailedData.size() - 2
                              );

                    if (!isShort
                            ? stockOperationalData[stock->operational.detailedData.size() - 1].price < currentTopEdge &&
                                  stockOperationalData[stock->operational.detailedData.size() - 2].price < previousTopEdge &&
                                  stockOperationalData[stock->operational.detailedData.size() - 3].price > beforePreviousTopEdge
                            : stockOperationalData[stock->operational.detailedData.size() - 1].price > currentTopEdge &&
                                  stockOperationalData[stock->operational.detailedData.size() - 2].price > previousTopEdge &&
                                  stockOperationalData[stock->operational.detailedData.size() - 3].price < beforePreviousTopEdge)
                    {
                        return QObject::tr("Decided to %1 because the price %2 exceeds %3 Bollindger edge price %4 at %5")
                            .arg(
                                !isShort ? QObject::tr("sell") : QObject::tr("buy"),
                                QString::number(
                                    stockOperationalData[stock->operational.detailedData.size() - 3].price,
                                    'f',
                                    stock->meta.pricePrecision
                                ) + " \u20BD",
                                !isShort ? QObject::tr("top") : QObject::tr("bottom"),
                                QString::number(beforePreviousTopEdge, 'f', stock->meta.pricePrecision) + " \u20BD",
                                QDateTime::fromMSecsSinceEpoch(
                                    stockOperationalData[stock->operational.detailedData.size() - 3].timestamp
                                )
                                    .toString(DATETIME_FORMAT)
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
