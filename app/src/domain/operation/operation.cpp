#include "src/domain/operation/operation.h"



Operation::Operation() :
    timestamp()
{
}

void Operation::fromJsonObject(const QJsonObject& jsonObject)
{
    timestamp = jsonObject.value("timestamp").toInt(0);
}

QJsonObject Operation::toJsonObject() const
{
    QJsonObject res;

    res.insert("timestamp", timestamp);

    return res;
}

bool operator==(const Operation& lhs, const Operation& rhs)
{
    return lhs.timestamp == rhs.timestamp;
}
