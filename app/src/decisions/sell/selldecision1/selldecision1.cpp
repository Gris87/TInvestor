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

void SellDecision1::makeDecision()
{
}
