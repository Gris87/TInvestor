#include "src/decisions/buy/buydecision5/buydecision5.h"

#include <QDebug>



const char* const DATETIME_FORMAT = "yyyy-MM-dd hh:mm:ss";

constexpr int    MINUTES_TO_DOUBLE_CHECK = 5;
constexpr int    MINUTES_TO_TRIPLE_CHECK = 3;
constexpr float  HUNDRED_PERCENT         = 100.0f;
constexpr qint64 MS_IN_SECOND            = 1000LL;
constexpr qint64 ONE_MINUTE              = 60LL * MS_IN_SECOND;



BuyDecision5::BuyDecision5() :
    IActionDecision()
{
    qDebug() << "Create BuyDecision5";
}

BuyDecision5::~BuyDecision5()
{
    qDebug() << "Destroy BuyDecision5";
}

// NOLINTBEGIN(readability-function-cognitive-complexity)
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
        const float priceRaise   = buyConfig->getPriceRaise();
        const int   duration     = buyConfig->getDuration();
        const float minimumPrice = price / (1 + (priceRaise / HUNDRED_PERCENT));

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

                if (prevPrice <= minimumPrice)
                {
                    bool good = true;

                    int j           = i - 1;
                    int minutesLeft = MINUTES_TO_DOUBLE_CHECK;

                    while (j >= 0 && minutesLeft > 0 && !parentThread->isInterruptionRequested())
                    {
                        if (stockData[j].price > minimumPrice)
                        {
                            good = false;

                            break;
                        }

                        --j;
                        --minutesLeft;
                    }

                    if (good)
                    {
                        const float tripleMaximumPrice = prevPrice / (1 - (priceRaise / HUNDRED_PERCENT));

                        int j           = dataIndex;
                        int minutesLeft = MINUTES_TO_TRIPLE_CHECK;

                        while (j >= 0 && minutesLeft > 0 && !parentThread->isInterruptionRequested())
                        {
                            if (stockData[j].price < tripleMaximumPrice)
                            {
                                good = false;

                                break;
                            }

                            --j;
                            --minutesLeft;
                        }

                        if (good)
                        {
                            int passPositions = (price - prevPrice) / stock->meta.minPriceIncrement;

                            if (passPositions > buyConfig->getOrderBookPositions())
                            {
                                const float raise = ((price / prevPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;

                                return QObject::
                                    tr("Decided to buy because the price raise to %1 from %2 at %3 within last %4 minutes and "
                                       "pass %5 positions of order book and the raise is %6")
                                        .arg(
                                            QString::number(price, 'f', stock->meta.pricePrecision) + " \u20BD",
                                            QString::number(prevPrice, 'f', stock->meta.pricePrecision) + " \u20BD",
                                            QDateTime::fromMSecsSinceEpoch(timestamp).toString(DATETIME_FORMAT),
                                            QString::number(duration),
                                            QString::number(passPositions),
                                            QString::number(raise, 'f', 2) + "%"
                                        );
                            }
                        }
                    }
                }
            }
        }
    }

    return "";
}
// NOLINTEND(readability-function-cognitive-complexity)

AsapMode BuyDecision5::asapMode() const
{
    return ASAP_MODE_IMMEDIATELY_TRADE;
}
