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

QString SellDecision1::makeDecision(Stock* stock, bool dateRange, int dataIndex, float /*price*/)
{
    // TODO: Remove it [BEGIN]
    const int minute = dateRange ? QDateTime::fromMSecsSinceEpoch(stock->data.at(dataIndex).timestamp).time().minute()
                                 : QTime::currentTime().minute();

    const bool    needToSell = minute % 4 >= 2;
    const QString instrumentId =
        minute % 2 == 0 ? "48bd9002-43be-4528-abf4-dc8135ad4550" : "15dc2120-29d2-48b8-87c0-da1d95255f68";

    if (needToSell && stock->meta.instrumentId == instrumentId)
    {
        return "I want to sell";
    }
    // TODO: Remove it [END]

    return "";
}
