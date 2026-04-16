#include "src/decisions/buy/buydecision8/buydecision8.h"

#include <QDebug>



constexpr int    MINUTES_TO_DOUBLE_CHECK = 10;
constexpr qint64 MS_IN_SECOND            = 1000LL;
constexpr qint64 ONE_MINUTE              = 60LL * MS_IN_SECOND;
constexpr qint64 ONE_HOUR                = 60LL * ONE_MINUTE;
constexpr qint64 ONE_DAY                 = 24LL * ONE_HOUR;



// ====================================================================
// Buy stock if price limit found during time period
// ====================================================================
//
//   -----------------------------------------------------------------X
//  /
// /
//

BuyDecision8::BuyDecision8() :
    IActionDecision()
{
    qDebug() << "Create BuyDecision8";
}

BuyDecision8::~BuyDecision8()
{
    qDebug() << "Destroy BuyDecision8";
}

// NOLINTBEGIN(readability-function-cognitive-complexity)
QString BuyDecision8::makeDecision(
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

    IBuyDecision8Config* buyConfig = config->getBuyDecision8Config();

    if (buyConfig->isEnabled())
    {
        if (dateRange)
        {
            return makeDecisionBasedOnStockData(parentThread, buyConfig, limitTimestamp, stock, dataIndex, price);
        }

        return makeDecisionBasedOnStockData(parentThread, buyConfig, limitTimestamp, stock, stock->data.size() - 1, price);
    }

    return "";
}
// NOLINTEND(readability-function-cognitive-complexity)

AsapMode BuyDecision8::asapMode() const
{
    return ASAP_MODE_NONE;
}

QString BuyDecision8::makeDecisionBasedOnStockData(
    QThread* parentThread, IBuyDecision8Config* buyConfig, qint64 limitTimestamp, Stock* stock, int dataIndex, float price
) const
{
    const StockData* stockData = stock->data.constData();

    const int duration = buyConfig->getDuration();
    limitTimestamp     = stockData[dataIndex].timestamp - (duration * ONE_MINUTE);

    for (int i = dataIndex; i >= 0 && !parentThread->isInterruptionRequested(); --i)
    {
        const qint64 timestamp = stockData[i].timestamp;
        const float  prevPrice = stockData[i].price;

        if (timestamp < limitTimestamp)
        {
            if (dataIndex - i > MINUTES_TO_DOUBLE_CHECK)
            {
                limitTimestamp = stockData[dataIndex].timestamp - ONE_DAY;

                if (doubleCheck(parentThread, stockData, i - 1, limitTimestamp, price))
                {
                    return QObject::tr("Decided to buy because the price reach market limit at %1 and hold it for %2 minutes")
                        .arg(QString::number(price, 'f', stock->meta.pricePrecision) + " \u20BD", QString::number(duration));
                }
            }

            break;
        }

        if (prevPrice != price)
        {
            break;
        }
    }

    return "";
}

bool
BuyDecision8::doubleCheck(QThread* parentThread, const StockData* stockData, int index, qint64 limitTimestamp, float price) const
{
    bool res = true;

    for (int i = index; i >= 0 && !parentThread->isInterruptionRequested(); --i)
    {
        const qint64 anotherTimestamp = stockData[i].timestamp;
        const float  anotherPrevPrice = stockData[i].price;

        if (anotherTimestamp < limitTimestamp)
        {
            break;
        }

        if (anotherPrevPrice > price)
        {
            res = false;

            break;
        }
    }

    return res;
}
