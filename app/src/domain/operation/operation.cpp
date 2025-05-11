#include "src/domain/operation/operation.h"



Operation::Operation() :
    units(),
    nano()
{
}

void Operation::fromJsonObject(const QJsonObject& jsonObject)
{
    units = jsonObject.value("units").toInt(0);
    nano  = jsonObject.value("nano").toInt(0);
}

QJsonObject Operation::toJsonObject() const
{
    QJsonObject res;

    res.insert("units", units);
    res.insert("nano", nano);

    return res;
}

bool operator==(const Operation& lhs, const Operation& rhs)
{
    return lhs.units == rhs.units && lhs.nano == rhs.nano;
}
