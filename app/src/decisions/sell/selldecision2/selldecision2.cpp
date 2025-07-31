#include "src/decisions/sell/selldecision2/selldecision2.h"

#include <QDebug>



SellDecision2::SellDecision2() :
    IActionDecision()
{
    qDebug() << "Create SellDecision2";
}

SellDecision2::~SellDecision2()
{
    qDebug() << "Destroy SellDecision2";
}

QString SellDecision2::makeDecision(
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
