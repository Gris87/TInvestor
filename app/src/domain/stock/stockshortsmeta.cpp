#include "src/domain/stock/stockshortsmeta.h"

#include "src/utils/exception/exception.h"



StockShortsMeta::StockShortsMeta() :
    enabled(),
    lastEnabledTimestamp()
{
}

static void metaEnabledParse(StockShortsMeta* meta, simdjson::ondemand::value value)
{
    meta->enabled = value.get_bool();
}

static void metaLastEnabledTimestampParse(StockShortsMeta* meta, simdjson::ondemand::value value)
{
    meta->lastEnabledTimestamp = value.get_int64();
}

static void metaThrowParseException(
    StockShortsMeta* /*meta*/, simdjson::ondemand::value /*value*/ // clazy:exclude=function-args-by-ref
)
{
    throwException("Unknown parameter");
}

using ParseHandler = void (*)(StockShortsMeta* meta, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"enabled",              metaEnabledParse             },
    {"lastEnabledTimestamp", metaLastEnabledTimestampParse}
};
// clang-format on

void StockShortsMeta::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, metaThrowParseException);

        parseHandler(this, field.value());
    }
}

QJsonObject StockShortsMeta::toJsonObject() const
{
    QJsonObject res;

    // clang-format off
    res.insert("enabled",              enabled);
    res.insert("lastEnabledTimestamp", lastEnabledTimestamp);
    // clang-format on

    return res;
}

bool operator==(const StockShortsMeta& lhs, const StockShortsMeta& rhs)
{
    return lhs.enabled == rhs.enabled && lhs.lastEnabledTimestamp == rhs.lastEnabledTimestamp;
}
