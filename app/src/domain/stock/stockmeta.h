#pragma once



#include <QJsonObject>

#include "src/domain/logo/logo.h"
#include "src/simdjson/simdjson_wrapped.h"



struct StockMeta
{
    StockMeta();
    StockMeta(const StockMeta& another) = default;
    ~StockMeta()                        = default;

    StockMeta& operator=(const StockMeta& another) = default;

    void fromJsonObject(simdjson::ondemand::object jsonObject); // clazy:exclude=function-args-by-ref

    [[nodiscard]]
    QJsonObject toJsonObject() const;

    QString  instrumentId;
    Logo*    instrumentLogo;
    QString  instrumentTicker;
    QString  instrumentName;
    bool     forQualInvestorFlag;
    float    minPriceIncrement;
    qint8    pricePrecision;
};



bool operator==(const StockMeta& lhs, const StockMeta& rhs);
