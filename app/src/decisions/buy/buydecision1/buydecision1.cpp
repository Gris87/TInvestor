#include "src/decisions/buy/buydecision1/buydecision1.h"

#include <QDebug>



BuyDecision1::BuyDecision1() :
    IActionDecision()
{
    qDebug() << "Create BuyDecision1";
}

BuyDecision1::~BuyDecision1()
{
    qDebug() << "Destroy BuyDecision1";
}

QString BuyDecision1::makeDecision(Stock* /*stock*/, bool /*dateRange*/, int /*dataIndex*/, float /*price*/)
{
    return "";
}
