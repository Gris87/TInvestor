#include "src/domain/bidirinfo/bidirinfo.h"

#include "src/utils/exception/exception.h"



BidirInfo::BidirInfo() :
    ticker(),
    name(),
    lot(),
    pricePrecision(),
    minPriceIncrement()
{
}

static void bidirInfoTickerParse(BidirInfo* bidirInfo, simdjson::ondemand::value value)
{
    const std::string_view valueStr = value.get_string();
    bidirInfo->ticker               = QString::fromUtf8(valueStr.data(), valueStr.size());
}

static void bidirInfoNameParse(BidirInfo* bidirInfo, simdjson::ondemand::value value)
{
    const std::string_view valueStr = value.get_string();
    bidirInfo->name                 = QString::fromUtf8(valueStr.data(), valueStr.size());
}

static void bidirInfoLotParse(BidirInfo* bidirInfo, simdjson::ondemand::value value)
{
    bidirInfo->lot = value.get_int64();
}

static void bidirInfoPricePrecisionParse(BidirInfo* bidirInfo, simdjson::ondemand::value value)
{
    bidirInfo->pricePrecision = value.get_int64();
}

static void bidirInfoMinPriceIncrementParse(BidirInfo* bidirInfo, simdjson::ondemand::value value)
{
    bidirInfo->minPriceIncrement.fromJsonObject(value.get_object());
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
    {"ticker",            bidirInfoTickerParse           },
    {"name",              bidirInfoNameParse             },
    {"lot",               bidirInfoLotParse              },
    {"pricePrecision",    bidirInfoPricePrecisionParse   },
    {"minPriceIncrement", bidirInfoMinPriceIncrementParse}
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
    res.insert("ticker",            ticker);
    res.insert("name",              name);
    res.insert("lot",               lot);
    res.insert("pricePrecision",    pricePrecision);
    res.insert("minPriceIncrement", minPriceIncrement.toJsonObject());
    // clang-format on

    return res;
}

bool operator==(const BidirInfo& lhs, const BidirInfo& rhs)
{
    return lhs.ticker == rhs.ticker && lhs.name == rhs.name && lhs.lot == rhs.lot && lhs.pricePrecision == rhs.pricePrecision &&
           lhs.minPriceIncrement == rhs.minPriceIncrement;
}
