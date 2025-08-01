#include "src/decisions/buy/buydecision4/buydecision4.h"

#include <QDebug>



BuyDecision4::BuyDecision4() :
    IActionDecision()
{
    qDebug() << "Create BuyDecision4";
}

BuyDecision4::~BuyDecision4()
{
    qDebug() << "Destroy BuyDecision4";
}

QString BuyDecision4::makeDecision(
    QThread* /*parentThread*/,
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
