#include "src/domain/quotation/quotation.h"



Quotation::Quotation() :
    units(),
    nano()
{
}

void Quotation::fromJsonObject(const QJsonObject& jsonObject)
{
    units = jsonObject.value("units").toInteger();
    nano  = jsonObject.value("nano").toInt();
}

QJsonObject Quotation::toJsonObject() const
{
    QJsonObject res;

    // clang-format off
    res.insert("units", units);
    res.insert("nano",  nano);
    // clang-format on

    return res;
}

bool operator==(const Quotation& lhs, const Quotation& rhs)
{
    return lhs.units == rhs.units && lhs.nano == rhs.nano;
}

bool operator>(const Quotation& lhs, const Quotation& rhs)
{
    return lhs.units > rhs.units || (lhs.units == rhs.units && lhs.nano > rhs.nano);
}
