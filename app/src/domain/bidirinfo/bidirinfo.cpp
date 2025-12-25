#include "src/domain/bidirinfo/bidirinfo.h"

#include "src/utils/exception/exception.h"



constexpr float FLOAT_EPSILON = 0.0001f;



BidirInfo::BidirInfo() :
    spread(),
    minYield(),
    totalYield()
{
}

static void bidirInfoSpreadParse(BidirInfo* bidirInfo, simdjson::ondemand::value value)
{
    bidirInfo->spread = value.get_double();
}

static void bidirInfoMinYieldParse(BidirInfo* bidirInfo, simdjson::ondemand::value value)
{
    bidirInfo->minYield = value.get_double();
}

static void bidirInfoTotalYieldParse(BidirInfo* bidirInfo, simdjson::ondemand::value value)
{
    bidirInfo->totalYield = value.get_double();
}

static void bidirInfoThrowParseException(
    BidirInfo* /*bidirInfo*/, simdjson::ondemand::value /*value*/ // clazy:exclude=function-args-by-ref
)
{
    throwException("Unknown parameter");
}

using ParseHandler = void (*)(BidirInfo* bidirInfo, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"spread",     bidirInfoSpreadParse    },
    {"minYield",   bidirInfoMinYieldParse  },
    {"totalYield", bidirInfoTotalYieldParse}
};
// clang-format on

void BidirInfo::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, bidirInfoThrowParseException);

        parseHandler(this, field.value());
    }
}

QJsonObject BidirInfo::toJsonObject() const
{
    QJsonObject res;

    // clang-format off
    res.insert("spread",     spread);
    res.insert("minYield",   minYield);
    res.insert("totalYield", totalYield);
    // clang-format on

    return res;
}

bool operator==(const BidirInfo& lhs, const BidirInfo& rhs)
{
    return qAbs(lhs.spread - rhs.spread) < FLOAT_EPSILON && qAbs(lhs.minYield - rhs.minYield) < FLOAT_EPSILON &&
           qAbs(lhs.totalYield - rhs.totalYield) < FLOAT_EPSILON;
}
