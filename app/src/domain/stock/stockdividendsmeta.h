#pragma once



#include <QJsonObject>

#include "src/simdjson/simdjson_wrapped.h"



struct StockDividendsMeta
{
    explicit StockDividendsMeta();
    StockDividendsMeta(const StockDividendsMeta& another) = default;
    ~StockDividendsMeta()                                 = default;

    StockDividendsMeta& operator=(const StockDividendsMeta& another) = default;

    void fromJsonObject(simdjson::ondemand::object jsonObject); // clazy:exclude=function-args-by-ref

    [[nodiscard]]
    QJsonObject toJsonObject() const;

    qint64 createTimestamp;
    qint64 paymentTimestamp;
    float  yield;
};



bool operator==(const StockDividendsMeta& lhs, const StockDividendsMeta& rhs);
