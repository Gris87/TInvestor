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

void BuyDecision1::makeDecision()
{
}
