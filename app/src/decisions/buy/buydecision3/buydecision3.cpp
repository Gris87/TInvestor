#include "src/decisions/buy/buydecision3/buydecision3.h"

#include <QDebug>



BuyDecision3::BuyDecision3() :
    IActionDecision()
{
    qDebug() << "Create BuyDecision3";
}

BuyDecision3::~BuyDecision3()
{
    qDebug() << "Destroy BuyDecision3";
}

QString BuyDecision3::makeDecision(
    IDecisionMakerConfig* /*config*/,
    Stock* /*stock*/,
    bool /*dateRange*/,
    int /*dataIndex*/,
    float /*price*/,
    float /*avgPrice*/,
    float /*commission*/
)
{
    return "";
}
