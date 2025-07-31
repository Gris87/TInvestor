#include "src/decisions/buy/buydecision2/buydecision2.h"

#include <QDebug>



BuyDecision2::BuyDecision2() :
    IActionDecision()
{
    qDebug() << "Create BuyDecision2";
}

BuyDecision2::~BuyDecision2()
{
    qDebug() << "Destroy BuyDecision2";
}

QString BuyDecision2::makeDecision(
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
