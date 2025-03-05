#include "src/domain/stocks/stocks.h"



StockMeta::StockMeta() :
    uid(),
    ticker(),
    name(),
    forQualInvestorFlag(),
    lot(),
    minPriceIncrement()
{
}

StockMeta::StockMeta(const StockMeta& another) :
    uid(another.uid),
    ticker(another.ticker),
    name(another.name),
    forQualInvestorFlag(another.forQualInvestorFlag),
    lot(another.lot),
    minPriceIncrement(another.minPriceIncrement)
{
}

StockMeta::~StockMeta()
{
}

StockMeta& StockMeta::operator=(const StockMeta& another)
{
    uid                 = another.uid;
    ticker              = another.ticker;
    name                = another.name;
    forQualInvestorFlag = another.forQualInvestorFlag;
    lot                 = another.lot;
    minPriceIncrement   = another.minPriceIncrement;

    return *this;
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

StockOperational::StockOperational() :
    lastStoredTimestamp(),
    detailedData()
{
}

StockOperational::StockOperational(const StockOperational& another) :
    lastStoredTimestamp(another.lastStoredTimestamp),
    detailedData(another.detailedData)
{
}

StockOperational::~StockOperational()
{
}

StockOperational& StockOperational::operator=(const StockOperational& another)
{
    lastStoredTimestamp = another.lastStoredTimestamp;
    detailedData        = another.detailedData;

    return *this;
}

Stock::Stock() :
    mutex(new QMutex()),
    meta(),
    operational(),
    data()
{
}

Stock::Stock(const Stock& another) :
    mutex(new QMutex()),
    meta(another.meta),
    operational(another.operational),
    data(another.data)
{
}

Stock::~Stock()
{
    delete mutex;
}

Stock& Stock::operator=(const Stock& another)
{
    meta        = another.meta;
    operational = another.operational;
    data        = another.data;

    return *this;
}

bool operator==(const StockMeta& lhs, const StockMeta& rhs)
{
    return lhs.uid == rhs.uid && lhs.ticker == rhs.ticker && lhs.name == rhs.name &&
           lhs.forQualInvestorFlag == rhs.forQualInvestorFlag && lhs.lot == rhs.lot &&
           lhs.minPriceIncrement == rhs.minPriceIncrement;
}
