#include "src/decisions/sell/selldecision4/selldecision4.h"

#include <QDebug>



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
