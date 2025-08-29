#include "src/decisions/sell/selldecision4/selldecision4.h"

#include <QDebug>



constexpr float HUNDRED_PERCENT = 100.0f;



SellDecision4::SellDecision4() :
    IActionDecision()
{
    qDebug() << "Create SellDecision4";
}

SellDecision4::~SellDecision4()
{
    qDebug() << "Destroy SellDecision4";
}

QString SellDecision4::makeDecision(
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

    ISellDecision4Config* sellConfig = config->getSellDecision4Config();

    if (sellConfig->isEnabled())
    {
        const float yield     = ((price / avgPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;
        const float loseYield = -sellConfig->getLoseYield() + (2 * commission);

        if (yield <= loseYield)
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
