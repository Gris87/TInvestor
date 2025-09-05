#include "src/decisions/sell/selldecision1/selldecision1.h"

#include <QDebug>



constexpr double INCREDIBLE_SELL_COEF = 3.0;
constexpr float  HUNDRED_PERCENT      = 100.0f;



SellDecision1::SellDecision1() :
    IActionDecision()
{
    qDebug() << "Create SellDecision1";
}

SellDecision1::~SellDecision1()
{
    qDebug() << "Destroy SellDecision1";
}

QString SellDecision1::makeDecision(
    QThread* /*parentThread*/,
    IDecisionMakerConfig* config,
    qint64 /*limitTimestamp*/,
    Stock* stock,
    bool /*dateRange*/,
    int /*dataIndex*/,
    float price,
    float avgPrice,
    float commission
)
{
    Q_ASSERT_X(config != nullptr, __FUNCTION__, "config is invalid");
    Q_ASSERT_X(stock != nullptr, __FUNCTION__, "stock is invalid");
    Q_ASSERT_X(price > 0, __FUNCTION__, "price is invalid");
    Q_ASSERT_X(avgPrice > 0, __FUNCTION__, "avgPrice is invalid");
    Q_ASSERT_X(commission > 0, __FUNCTION__, "commission is invalid");

    ISellDecision1Config* sellConfig = config->getSellDecision1Config();

    if (sellConfig->isEnabled())
    {
        const float coef = price / avgPrice;

        if (coef < INCREDIBLE_SELL_COEF)
        {
            const float yield      = (coef * HUNDRED_PERCENT) - HUNDRED_PERCENT;
            const float yieldAbove = sellConfig->getYieldAbove() + (2 * commission);

            if (yield >= yieldAbove)
            {
                return QObject::tr("Decided to sell because the price reached %1 with yield %2 from the price %3")
                    .arg(
                        QString::number(price, 'f', stock->meta.pricePrecision) + " \u20BD",
                        "+" + QString::number(yield, 'f', 2) + "%",
                        QString::number(avgPrice, 'f', stock->meta.pricePrecision) + " \u20BD"
                    );
            }
        }
    }

    return "";
}

AsapMode SellDecision1::asapMode() const
{
    return ASAP_MODE_NONE;
}
