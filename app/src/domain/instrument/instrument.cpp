#include "src/domain/instrument/instrument.h"



Instrument::Instrument() :
    ticker(),
    name(),
    lot(),
    pricePrecision()
{
}

void Instrument::fromJsonObject(const QJsonObject& jsonObject)
{
    ticker         = jsonObject.value("ticker").toString();
    name           = jsonObject.value("name").toString();
    lot            = jsonObject.value("lot").toInt();
    pricePrecision = jsonObject.value("pricePrecision").toInt();
}

QJsonObject Instrument::toJsonObject() const
{
    QJsonObject res;

    res.insert("ticker", ticker);
    res.insert("name", name);
    res.insert("lot", lot);
    res.insert("pricePrecision", pricePrecision);

    return res;
}

bool operator==(const Instrument& lhs, const Instrument& rhs)
{
    return lhs.ticker == rhs.ticker && lhs.name == rhs.name && lhs.lot == rhs.lot && lhs.pricePrecision == rhs.pricePrecision;
}
