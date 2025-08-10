#include "src/decisions/sell/selldecision1/selldecision1.h"

#include <QDebug>



constexpr float HUNDRED_PERCENT = 100.0f;



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
    Stock*                stock,
    bool /*dateRange*/,
    int /*dataIndex*/,
    float price,
    float avgPrice,
    float commission
)
{
    ISellDecision1Config* sellConfig = config->getSellDecision1Config();

    if (sellConfig->isEnabled())
    {
        const float yield      = ((price / avgPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT;
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

    return "";
}
