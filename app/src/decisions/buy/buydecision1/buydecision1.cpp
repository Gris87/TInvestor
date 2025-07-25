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

QString BuyDecision1::makeDecision(Stock* stock, bool dateRange, int dataIndex, float /*price*/)
{
    // TODO: Remove it [BEGIN]
    const int minute = dateRange ? QDateTime::fromMSecsSinceEpoch(stock->data.at(dataIndex).timestamp).time().minute()
                                 : QTime::currentTime().minute();

    const bool    needToBuy = minute % 4 < 2;
    const QString instrumentId =
        minute % 2 == 0 ? "48bd9002-43be-4528-abf4-dc8135ad4550" : "15dc2120-29d2-48b8-87c0-da1d95255f68";

    if (needToBuy && stock->meta.instrumentId == instrumentId)
    {
        return "I want to buy";
    }
    // TODO: Remove it [END]

    return "";
}
