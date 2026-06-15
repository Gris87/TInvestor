#pragma once



#include <QJsonObject>

#include "src/simdjson/simdjson_wrapped.h"



enum BiDirPriority : quint8
{
    BIDIR_PRIORITY_LOW,
    BIDIR_PRIORITY_NORMAL,
    BIDIR_PRIORITY_HIGH
};



struct BiDirInfo
{
    explicit BiDirInfo();
    BiDirInfo(const BiDirInfo& another) = default;
    ~BiDirInfo()                        = default;

    BiDirInfo& operator=(const BiDirInfo& another) = default;

    void fromJsonObject(simdjson::ondemand::object jsonObject); // clazy:exclude=function-args-by-ref

    [[nodiscard]]
    QJsonObject toJsonObject() const;

    float         spread;
    float         maxSpread;
    float         minYield;
    float         totalYield;
    BiDirPriority priority;
};

using BiDirInfos = QMap<QString, BiDirInfo>; // UID => BiDirInfo



bool operator==(const BiDirInfo& lhs, const BiDirInfo& rhs);
