#include "src/domain/stock/stockdividendsmeta.h"

#include "src/utils/exception/exception.h"



constexpr float FLOAT_EPSILON = 0.0001f;



StockDividendsMeta::StockDividendsMeta() :
    createTimestamp(),
    paymentTimestamp(),
    yield()
{
}

static void metaCreateTimestampParse(StockDividendsMeta* meta, simdjson::ondemand::value value)
{
    meta->createTimestamp = value.get_int64();
}

static void metaPaymentTimestampParse(StockDividendsMeta* meta, simdjson::ondemand::value value)
{
    meta->paymentTimestamp = value.get_int64();
}

static void metaYieldParse(StockDividendsMeta* meta, simdjson::ondemand::value value)
{
    meta->yield = value.get_double_in_string();
}

static void metaThrowParseException(
    StockDividendsMeta* /*meta*/, simdjson::ondemand::value /*value*/ // clazy:exclude=function-args-by-ref
)
{
    throwException("Unknown parameter");
}

using ParseHandler = void (*)(StockDividendsMeta* meta, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"createTimestamp",  metaCreateTimestampParse },
    {"paymentTimestamp", metaPaymentTimestampParse},
    {"yield",            metaYieldParse           }
};
// clang-format on

void StockDividendsMeta::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, metaThrowParseException);

        parseHandler(this, field.value());
    }
}

QJsonObject StockDividendsMeta::toJsonObject() const
{
    QJsonObject res;

    // clang-format off
    res.insert("createTimestamp",  createTimestamp);
    res.insert("paymentTimestamp", paymentTimestamp);
    res.insert("yield",            QString::number(yield, 'f', 2));
    // clang-format on

    return res;
}

bool operator==(const StockDividendsMeta& lhs, const StockDividendsMeta& rhs)
{
    return lhs.createTimestamp == rhs.createTimestamp && lhs.paymentTimestamp == rhs.paymentTimestamp &&
           qAbs(lhs.yield - rhs.yield) < FLOAT_EPSILON;
}
