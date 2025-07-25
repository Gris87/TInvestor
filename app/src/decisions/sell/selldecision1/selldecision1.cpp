#include "src/decisions/sell/selldecision1/selldecision1.h"

#include <QDebug>



SellDecision1::SellDecision1() :
    IActionDecision()
{
    qDebug() << "Create SellDecision1";
}

SellDecision1::~SellDecision1()
{
    qDebug() << "Destroy SellDecision1";
}

QString SellDecision1::makeDecision(Stock* /*stock*/, bool /*dateRange*/, int /*dataIndex*/, float /*price*/)
{
    return "";
}
