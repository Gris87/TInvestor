#pragma once



#include <QJsonObject>

#include "src/domain/logo/logo.h"
#include "src/simdjson/simdjson_wrapped.h"



struct PortfolioItem
{
    explicit PortfolioItem();
    PortfolioItem(const PortfolioItem& another) = default;
    ~PortfolioItem()                            = default;

    PortfolioItem& operator=(const PortfolioItem& another) = default;

    void fromJsonObject(simdjson::ondemand::object jsonObject); // clazy:exclude=function-args-by-ref

    [[nodiscard]]
    QJsonObject toJsonObject() const;

    QString instrumentId;
    Logo*   instrumentLogo;
    QString instrumentTicker;
    QString instrumentName;
    bool    showPrices;
    double  available;
    float   price;
    float   avgPriceFifo;
    float   avgPriceWavg;
    double  cost;
    float   part;
    float   yield;
    float   yieldPercent;
    float   dailyYield;
    float   priceForDailyYield;
    double  costForDailyYield;
    float   dailyYieldPercent;
    qint8   pricePrecision;
};



bool operator==(const PortfolioItem& lhs, const PortfolioItem& rhs);
