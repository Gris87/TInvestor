#include "src/domain/quotation/quotation.h"



Quotation::Quotation() :
    units(),
    nano()
{
}

static void quotationUnitsParse(Quotation* quotation, simdjson::ondemand::value value)
{
    quotation->units = value.get_int64();
}

static void quotationNanoParse(Quotation* quotation, simdjson::ondemand::value value)
{
    quotation->nano = value.get_int64();
}

static void quotationThrowParseException(Quotation* /*quotation*/, simdjson::ondemand::value /*value*/)
{
    throw std::exception("Unknown parameter");
}

using ParseHandler = void (*)(Quotation* quotation, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"units", quotationUnitsParse},
    {"nano",  quotationNanoParse }
};
// clang-format on

void Quotation::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, quotationThrowParseException);

        parseHandler(this, field.value());
    }
}

QJsonObject Quotation::toJsonObject() const
{
    QJsonObject res;

    // clang-format off
    res.insert("units", units);
    res.insert("nano",  nano);
    // clang-format on

    return res;
}

bool operator<(const Quotation& lhs, const Quotation& rhs)
{
    return lhs.units < rhs.units || (lhs.units == rhs.units && lhs.nano < rhs.nano);
}

bool operator==(const Quotation& lhs, const Quotation& rhs)
{
    return lhs.units == rhs.units && lhs.nano == rhs.nano;
}

bool operator>(const Quotation& lhs, const Quotation& rhs)
{
    return lhs.units > rhs.units || (lhs.units == rhs.units && lhs.nano > rhs.nano);
}
