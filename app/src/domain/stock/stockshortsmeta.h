#pragma once



#include <QJsonObject>

#include "src/simdjson/simdjson_wrapped.h"



struct StockShortsMeta
{
    explicit StockShortsMeta();
    StockShortsMeta(const StockShortsMeta& another) = default;
    ~StockShortsMeta()                              = default;

    StockShortsMeta& operator=(const StockShortsMeta& another) = default;

    void fromJsonObject(simdjson::ondemand::object jsonObject); // clazy:exclude=function-args-by-ref

    [[nodiscard]]
    QJsonObject toJsonObject() const;

    bool   enabled;
    qint64 lastEnabledTimestamp;
};



bool operator==(const StockShortsMeta& lhs, const StockShortsMeta& rhs);
