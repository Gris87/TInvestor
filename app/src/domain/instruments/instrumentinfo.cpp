#include "src/domain/instruments/instrumentinfo.h"



void InstrumentInfo::fromJsonObject(const QJsonObject& jsonObject)
{
    ticker = jsonObject.value("ticker").toString("");
    name   = jsonObject.value("name").toString("");
}

QJsonObject InstrumentInfo::toJsonObject() const
{
    QJsonObject res;

    res.insert("ticker", ticker);
    res.insert("name", name);

    return res;
}

bool operator==(const InstrumentInfo& lhs, const InstrumentInfo& rhs)
{
    return lhs.ticker == rhs.ticker && lhs.name == rhs.name;
}
