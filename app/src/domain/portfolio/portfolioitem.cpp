#include "src/domain/portfolio/portfolioitem.h"

#include "src/utils/exception/exception.h"



constexpr float  FLOAT_EPSILON  = 0.0001f;
constexpr double DOUBLE_EPSILON = 0.0001;



PortfolioItem::PortfolioItem() :
    instrumentId(),
    instrumentLogo(),
    instrumentTicker(),
    instrumentName(),
    showPrices(),
    available(),
    price(),
    avgPriceFifo(),
    avgPriceWavg(),
    cost(),
    part(),
    yield(),
    yieldPercent(),
    dailyYield(),
    priceForDailyYield(),
    costForDailyYield(),
    dailyYieldPercent(),
    pricePrecision()
{
}

static void itemInstrumentIdParse(PortfolioItem* item, simdjson::ondemand::value value)
{
    const std::string_view valueStr = value.get_string();
    item->instrumentId              = QString::fromUtf8(valueStr.data(), valueStr.size());
}

static void itemInstrumentTickerParse(PortfolioItem* item, simdjson::ondemand::value value)
{
    const std::string_view valueStr = value.get_string();
    item->instrumentTicker          = QString::fromUtf8(valueStr.data(), valueStr.size());
}

static void itemInstrumentNameParse(PortfolioItem* item, simdjson::ondemand::value value)
{
    const std::string_view valueStr = value.get_string();
    item->instrumentName            = QString::fromUtf8(valueStr.data(), valueStr.size());
}

static void itemShowPricesParse(PortfolioItem* item, simdjson::ondemand::value value)
{
    item->showPrices = value.get_bool().value();
}

static void itemAvailableParse(PortfolioItem* item, simdjson::ondemand::value value)
{
    item->available = value.get_double_in_string().value();
}

static void itemPriceParse(PortfolioItem* item, simdjson::ondemand::value value)
{
    item->price = value.get_double_in_string().value();
}

static void itemAvgPriceFifoParse(PortfolioItem* item, simdjson::ondemand::value value)
{
    item->avgPriceFifo = value.get_double_in_string().value();
}

static void itemAvgPriceWavgParse(PortfolioItem* item, simdjson::ondemand::value value)
{
    item->avgPriceWavg = value.get_double_in_string().value();
}

static void itemCostParse(PortfolioItem* item, simdjson::ondemand::value value)
{
    item->cost = value.get_double_in_string().value();
}

static void itemPartParse(PortfolioItem* item, simdjson::ondemand::value value)
{
    item->part = value.get_double_in_string().value();
}

static void itemYieldParse(PortfolioItem* item, simdjson::ondemand::value value)
{
    item->yield = value.get_double_in_string().value();
}

static void itemYieldPercentParse(PortfolioItem* item, simdjson::ondemand::value value)
{
    item->yieldPercent = value.get_double_in_string().value();
}

static void itemDailyYieldParse(PortfolioItem* item, simdjson::ondemand::value value)
{
    item->dailyYield = value.get_double_in_string().value();
}

static void itemPriceForDailyYieldParse(PortfolioItem* item, simdjson::ondemand::value value)
{
    item->priceForDailyYield = value.get_double_in_string().value();
}

static void itemCostForDailyYieldParse(PortfolioItem* item, simdjson::ondemand::value value)
{
    item->costForDailyYield = value.get_double_in_string().value();
}

static void itemDailyYieldPercentParse(PortfolioItem* item, simdjson::ondemand::value value)
{
    item->dailyYieldPercent = value.get_double_in_string().value();
}

static void itemPricePrecisionParse(PortfolioItem* item, simdjson::ondemand::value value)
{
    item->pricePrecision = value.get_int64();
}

static void itemThrowParseException(
    PortfolioItem* /*item*/, simdjson::ondemand::value /*value*/ // clazy:exclude=function-args-by-ref
)
{
    throwException("Unknown parameter");
}

using ParseHandler = void (*)(PortfolioItem* item, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"instrumentId",       itemInstrumentIdParse      },
    {"instrumentTicker",   itemInstrumentTickerParse  },
    {"instrumentName",     itemInstrumentNameParse    },
    {"showPrices",         itemShowPricesParse        },
    {"available",          itemAvailableParse         },
    {"price",              itemPriceParse             },
    {"avgPriceFifo",       itemAvgPriceFifoParse      },
    {"avgPriceWavg",       itemAvgPriceWavgParse      },
    {"cost",               itemCostParse              },
    {"part",               itemPartParse              },
    {"yield",              itemYieldParse             },
    {"yieldPercent",       itemYieldPercentParse      },
    {"dailyYield",         itemDailyYieldParse        },
    {"priceForDailyYield", itemPriceForDailyYieldParse},
    {"costForDailyYield",  itemCostForDailyYieldParse },
    {"dailyYieldPercent",  itemDailyYieldPercentParse },
    {"pricePrecision",     itemPricePrecisionParse    }
};
// clang-format on

void PortfolioItem::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, itemThrowParseException);

        parseHandler(this, field.value());
    }
}

QJsonObject PortfolioItem::toJsonObject() const
{
    QJsonObject res;

    // clang-format off
    res.insert("instrumentId",       instrumentId);
    res.insert("instrumentTicker",   instrumentTicker);
    res.insert("instrumentName",     instrumentName);
    res.insert("showPrices",         showPrices);
    res.insert("available",          QString::number(available, 'f', 2));
    res.insert("price",              QString::number(price, 'f', pricePrecision));
    res.insert("avgPriceFifo",       QString::number(avgPriceFifo, 'f', pricePrecision));
    res.insert("avgPriceWavg",       QString::number(avgPriceWavg, 'f', pricePrecision));
    res.insert("cost",               QString::number(cost, 'f', 2));
    res.insert("part",               QString::number(part, 'f', 2));
    res.insert("yield",              QString::number(yield, 'f', 2));
    res.insert("yieldPercent",       QString::number(yieldPercent, 'f', 2));
    res.insert("dailyYield",         QString::number(dailyYield, 'f', 2));
    res.insert("priceForDailyYield", QString::number(priceForDailyYield, 'f', 2));
    res.insert("costForDailyYield",  QString::number(costForDailyYield, 'f', 2));
    res.insert("dailyYieldPercent",  QString::number(dailyYieldPercent, 'f', 2));
    res.insert("pricePrecision",     pricePrecision);
    // clang-format on

    return res;
}

bool operator==(const PortfolioItem& lhs, const PortfolioItem& rhs)
{
    return lhs.instrumentId == rhs.instrumentId && lhs.instrumentTicker == rhs.instrumentTicker &&
           lhs.instrumentName == rhs.instrumentName && lhs.showPrices == rhs.showPrices &&
           qAbs(lhs.available - rhs.available) < DOUBLE_EPSILON && qAbs(lhs.price - rhs.price) < FLOAT_EPSILON &&
           qAbs(lhs.avgPriceFifo - rhs.avgPriceFifo) < FLOAT_EPSILON &&
           qAbs(lhs.avgPriceWavg - rhs.avgPriceWavg) < FLOAT_EPSILON && qAbs(lhs.cost - rhs.cost) < DOUBLE_EPSILON &&
           qAbs(lhs.part - rhs.part) < FLOAT_EPSILON && qAbs(lhs.yield - rhs.yield) < FLOAT_EPSILON &&
           qAbs(lhs.yieldPercent - rhs.yieldPercent) < FLOAT_EPSILON && qAbs(lhs.dailyYield - rhs.dailyYield) < FLOAT_EPSILON &&
           qAbs(lhs.priceForDailyYield - rhs.priceForDailyYield) < FLOAT_EPSILON &&
           qAbs(lhs.costForDailyYield - rhs.costForDailyYield) < DOUBLE_EPSILON &&
           qAbs(lhs.dailyYieldPercent - rhs.dailyYieldPercent) < FLOAT_EPSILON && lhs.pricePrecision == rhs.pricePrecision;
}
