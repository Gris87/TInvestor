#include "src/domain/quotation/quotation.h"



Quotation::Quotation() :
    units(),
    nano()
{
}

Quotation::Quotation(const Quotation& another) :
    units(another.units),
    nano(another.nano)
{
}

Quotation::~Quotation()
{
}

Quotation& Quotation::operator=(const Quotation& another)
{
    units = another.units;
    nano  = another.nano;

    return *this;
}

void Quotation::fromJsonObject(const QJsonObject& jsonObject)
{
    units = jsonObject.value("units").toInt(0);
    nano  = jsonObject.value("nano").toInt(0);
}

QJsonObject Quotation::toJsonObject() const
{
    QJsonObject res;

    res.insert("units", units);
    res.insert("nano", nano);

    return res;
}

bool operator==(const Quotation& lhs, const Quotation& rhs)
{
    return lhs.units == rhs.units && lhs.nano == rhs.nano;
}
