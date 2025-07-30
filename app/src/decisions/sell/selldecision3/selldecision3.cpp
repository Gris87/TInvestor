#include "src/decisions/sell/selldecision3/selldecision3.h"

#include <QDebug>



SellDecision3::SellDecision3() :
    IActionDecision()
{
    qDebug() << "Create SellDecision3";
}

SellDecision3::~SellDecision3()
{
    qDebug() << "Destroy SellDecision3";
}

QString SellDecision3::makeDecision(
    IDecisionMakerConfig* /*config*/, Stock* /*stock*/, bool /*dateRange*/, int /*dataIndex*/, float /*price*/, float /*avgPrice*/
)
{
    return "";
}
