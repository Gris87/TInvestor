#include "src/domain/operation/operationfifoitem.h"



OperationFifoItem::OperationFifoItem() :
    quantity(),
    cost()
{
}

void OperationFifoItem::fromJsonObject(const QJsonObject& jsonObject)
{
    quantity = jsonObject.value("quantity").toInteger();

    cost.fromJsonObject(jsonObject.value("cost").toObject());
}

QJsonObject OperationFifoItem::toJsonObject() const
{
    QJsonObject res;

    // clang-format off
    res.insert("quantity", quantity);
    res.insert("cost",     cost.toJsonObject());
    // clang-format on

    return res;
}

bool operator==(const OperationFifoItem& lhs, const OperationFifoItem& rhs)
{
    return lhs.quantity == rhs.quantity && lhs.cost == rhs.cost;
}
