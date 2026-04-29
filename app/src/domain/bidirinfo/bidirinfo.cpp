#include "src/domain/bidirinfo/bidirinfo.h"

#include "src/utils/exception/exception.h"



// clang-format off
static const QMap<QString, BiDirPriority> STRING_TO_BIDIR_PRIORITY{ // clazy:exclude=non-pod-global-static
    {"high",   BIDIR_PRIORITY_HIGH},
    {"normal", BIDIR_PRIORITY_NORMAL},
    {"low",    BIDIR_PRIORITY_LOW}
};

static const QMap<BiDirPriority, QString> BIDIR_PRIORITY_TO_STRING{ // clazy:exclude=non-pod-global-static
    {BIDIR_PRIORITY_HIGH,   "high"},
    {BIDIR_PRIORITY_NORMAL, "normal"},
    {BIDIR_PRIORITY_LOW,    "low"}
};
// clang-format on



constexpr float FLOAT_EPSILON = 0.0001f;



BiDirInfo::BiDirInfo() :
    spread(),
    minYield(),
    totalYield(),
    priority()
{
}

static void biDirInfoSpreadParse(BiDirInfo* biDirInfo, simdjson::ondemand::value value)
{
    biDirInfo->spread = value.get_double();
}

static void biDirInfoMinYieldParse(BiDirInfo* biDirInfo, simdjson::ondemand::value value)
{
    biDirInfo->minYield = value.get_double();
}

static void biDirInfoTotalYieldParse(BiDirInfo* biDirInfo, simdjson::ondemand::value value)
{
    biDirInfo->totalYield = value.get_double();
}

static void biDirInfoPriorityParse(BiDirInfo* biDirInfo, simdjson::ondemand::value value)
{
    const std::string_view valueStr    = value.get_string();
    const QString          priorityStr = QString::fromUtf8(valueStr.data(), valueStr.size());

    biDirInfo->priority = STRING_TO_BIDIR_PRIORITY.value(priorityStr, BIDIR_PRIORITY_LOW);
}

static void biDirInfoThrowParseException(
    BiDirInfo* /*biDirInfo*/, simdjson::ondemand::value /*value*/ // clazy:exclude=function-args-by-ref
)
{
    throwException("Unknown parameter");
}

using ParseHandler = void (*)(BiDirInfo* biDirInfo, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"spread",     biDirInfoSpreadParse    },
    {"minYield",   biDirInfoMinYieldParse  },
    {"totalYield", biDirInfoTotalYieldParse},
    {"priority",   biDirInfoPriorityParse  }
};
// clang-format on

void BiDirInfo::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, biDirInfoThrowParseException);

        parseHandler(this, field.value());
    }
}

QJsonObject BiDirInfo::toJsonObject() const
{
    QJsonObject res;

    // clang-format off
    res.insert("spread",     spread);
    res.insert("minYield",   minYield);
    res.insert("totalYield", totalYield);
    res.insert("priority",   BIDIR_PRIORITY_TO_STRING.value(priority, "low"));
    // clang-format on

    return res;
}

bool operator==(const BiDirInfo& lhs, const BiDirInfo& rhs)
{
    return qAbs(lhs.spread - rhs.spread) < FLOAT_EPSILON && qAbs(lhs.minYield - rhs.minYield) < FLOAT_EPSILON &&
           qAbs(lhs.totalYield - rhs.totalYield) < FLOAT_EPSILON && lhs.priority == rhs.priority;
}
