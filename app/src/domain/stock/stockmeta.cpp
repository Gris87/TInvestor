#include "src/domain/stock/stockmeta.h"



StockMeta::StockMeta() :
    instrumentId(),
    instrumentLogo(),
    instrumentTicker(),
    instrumentName(),
    forQualInvestorFlag(),
    minPriceIncrement(),
    pricePrecision()
{
}

static void metaInstrumentIdParse(StockMeta* meta, simdjson::ondemand::value value)
{
    const std::string_view valueStr = value.get_string();
    meta->instrumentId              = QString::fromUtf8(valueStr.data(), valueStr.size());
}

static void metaInstrumentTickerParse(StockMeta* meta, simdjson::ondemand::value value)
{
    const std::string_view valueStr = value.get_string();
    meta->instrumentTicker          = QString::fromUtf8(valueStr.data(), valueStr.size());
}

static void metaInstrumentNameParse(StockMeta* meta, simdjson::ondemand::value value)
{
    const std::string_view valueStr = value.get_string();
    meta->instrumentName            = QString::fromUtf8(valueStr.data(), valueStr.size());
}

static void metaForQualInvestorFlagParse(StockMeta* meta, simdjson::ondemand::value value)
{
    meta->forQualInvestorFlag = value.get_bool().value();
}

static void metaMinPriceIncrementParse(StockMeta* meta, simdjson::ondemand::value value)
{
    meta->minPriceIncrement = value.get_double_in_string().value();
}

static void metaPricePrecisionParse(StockMeta* meta, simdjson::ondemand::value value)
{
    meta->pricePrecision = value.get_int64();
}

static void metaThrowParseException(StockMeta* /*meta*/, simdjson::ondemand::value /*value*/)
{
    throw std::runtime_error("Unknown parameter");
}

using ParseHandler = void (*)(StockMeta* meta, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"instrumentId",        metaInstrumentIdParse       },
    {"instrumentTicker",    metaInstrumentTickerParse   },
    {"instrumentName",      metaInstrumentNameParse     },
    {"forQualInvestorFlag", metaForQualInvestorFlagParse},
    {"minPriceIncrement",   metaMinPriceIncrementParse  },
    {"pricePrecision",      metaPricePrecisionParse     }
};
// clang-format on

void StockMeta::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, metaThrowParseException);

        parseHandler(this, field.value());
    }
}

QJsonObject StockMeta::toJsonObject() const
{
    QJsonObject res;

    // clang-format off
    res.insert("instrumentId",        instrumentId);
    res.insert("instrumentTicker",    instrumentTicker);
    res.insert("instrumentName",      instrumentName);
    res.insert("forQualInvestorFlag", forQualInvestorFlag);
    res.insert("minPriceIncrement",   QString::number(minPriceIncrement, 'f', pricePrecision));
    res.insert("pricePrecision",      pricePrecision);
    // clang-format on

    return res;
}

bool operator==(const StockMeta& lhs, const StockMeta& rhs)
{
    return lhs.instrumentId == rhs.instrumentId && lhs.instrumentTicker == rhs.instrumentTicker &&
           lhs.instrumentName == rhs.instrumentName && lhs.forQualInvestorFlag == rhs.forQualInvestorFlag &&
           lhs.minPriceIncrement == rhs.minPriceIncrement && lhs.pricePrecision == rhs.pricePrecision;
}
