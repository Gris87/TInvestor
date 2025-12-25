#pragma once



#include <QJsonObject>

#include "src/simdjson/simdjson_wrapped.h"



struct BidirInfo
{
    explicit BidirInfo();
    BidirInfo(const BidirInfo& another) = default;
    ~BidirInfo()                        = default;

    BidirInfo& operator=(const BidirInfo& another) = default;

    void fromJsonObject(simdjson::ondemand::object jsonObject); // clazy:exclude=function-args-by-ref

    [[nodiscard]]
    QJsonObject toJsonObject() const;

    float spread;
    float minYield;
    float totalYield;
};

using BidirInfos = QMap<QString, BidirInfo>; // UID => BidirInfo



bool operator==(const BidirInfo& lhs, const BidirInfo& rhs);
