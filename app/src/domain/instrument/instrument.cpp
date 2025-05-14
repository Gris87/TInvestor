#include "src/domain/instrument/instrument.h"



Instrument::Instrument() :
    ticker(),
    name()
{
}

void Instrument::fromJsonObject(const QJsonObject& jsonObject)
{
    ticker = jsonObject.value("ticker").toString();
    name   = jsonObject.value("name").toString();
}

QJsonObject Instrument::toJsonObject() const
{
    QJsonObject res;

    res.insert("ticker", ticker);
    res.insert("name", name);

    return res;
}

bool operator==(const Instrument& lhs, const Instrument& rhs)
{
    return lhs.ticker == rhs.ticker && lhs.name == rhs.name;
}
