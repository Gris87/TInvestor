#include "src/domain/operation/operation.h"



Operation::Operation() :
    timestamp(),
    instrumentId()
{
}

void Operation::fromJsonObject(const QJsonObject& jsonObject)
{
    timestamp    = jsonObject.value("timestamp").toInteger(0);
    instrumentId = jsonObject.value("instrumentId").toString();
}

QJsonObject Operation::toJsonObject() const
{
    QJsonObject res;

    // clang-format off
    res.insert("timestamp",    timestamp);
    res.insert("instrumentId", instrumentId);
    // clang-format on

    return res;
}

bool operator==(const Operation& lhs, const Operation& rhs)
{
    return lhs.timestamp == rhs.timestamp && lhs.instrumentId == rhs.instrumentId;
}
