#include "src/domain/instrument/instrument.h"

#include "src/utils/exception/exception.h"



Instrument::Instrument() :
    ticker(),
    name(),
    lot(),
    pricePrecision()
{
}

static void instrumentTickerParse(Instrument* instrument, simdjson::ondemand::value value)
{
    const std::string_view valueStr = value.get_string();
    instrument->ticker              = QString::fromUtf8(valueStr.data(), valueStr.size());
}

static void instrumentNameParse(Instrument* instrument, simdjson::ondemand::value value)
{
    const std::string_view valueStr = value.get_string();
    instrument->name                = QString::fromUtf8(valueStr.data(), valueStr.size());
}

static void instrumentLotParse(Instrument* instrument, simdjson::ondemand::value value)
{
    instrument->lot = value.get_int64();
}

static void instrumentPricePrecisionParse(Instrument* instrument, simdjson::ondemand::value value)
{
    instrument->pricePrecision = value.get_int64();
}

static void instrumentThrowParseException(
    Instrument* /*instrument*/, simdjson::ondemand::value /*value*/ // clazy:exclude=function-args-by-ref
)
{
    throwException("Unknown parameter");
}

using ParseHandler = void (*)(Instrument* instrument, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"ticker",         instrumentTickerParse        },
    {"name",           instrumentNameParse          },
    {"lot",            instrumentLotParse           },
    {"pricePrecision", instrumentPricePrecisionParse}
};
// clang-format on

void Instrument::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, instrumentThrowParseException);

        parseHandler(this, field.value());
    }
}

QJsonObject Instrument::toJsonObject() const
{
    QJsonObject res;

    res.insert("ticker", ticker);
    res.insert("name", name);
    res.insert("lot", lot);
    res.insert("pricePrecision", pricePrecision);

    return res;
}

bool operator==(const Instrument& lhs, const Instrument& rhs)
{
    return lhs.ticker == rhs.ticker && lhs.name == rhs.name && lhs.lot == rhs.lot && lhs.pricePrecision == rhs.pricePrecision;
}
