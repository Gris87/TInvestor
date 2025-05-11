#include "src/domain/stock/stockmeta.h"



StockMeta::StockMeta() :
    uid(),
    ticker(),
    name(),
    forQualInvestorFlag(),
    lot(),
    minPriceIncrement()
{
}

void StockMeta::fromJsonObject(const QJsonObject& jsonObject)
{
    uid                 = jsonObject.value("uid").toString("");
    ticker              = jsonObject.value("ticker").toString("");
    name                = jsonObject.value("name").toString("");
    forQualInvestorFlag = jsonObject.value("forQualInvestorFlag").toBool(false);
    lot                 = jsonObject.value("lot").toInt(0);
    minPriceIncrement.fromJsonObject(jsonObject.value("minPriceIncrement").toObject());
}

QJsonObject StockMeta::toJsonObject() const
{
    QJsonObject res;

    res.insert("uid", uid);
    res.insert("ticker", ticker);
    res.insert("name", name);
    res.insert("forQualInvestorFlag", forQualInvestorFlag);
    res.insert("lot", lot);
    res.insert("minPriceIncrement", minPriceIncrement.toJsonObject());

    return res;
}

bool operator==(const StockMeta& lhs, const StockMeta& rhs)
{
    return lhs.uid == rhs.uid && lhs.ticker == rhs.ticker && lhs.name == rhs.name &&
           lhs.forQualInvestorFlag == rhs.forQualInvestorFlag && lhs.lot == rhs.lot &&
           lhs.minPriceIncrement == rhs.minPriceIncrement;
}
