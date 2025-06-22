#include "src/domain/stock/stockmeta.h"



StockMeta::StockMeta() :
    uid(),
    ticker(),
    name(),
    forQualInvestorFlag(),
    lot(),
    minPriceIncrement()
{
}

static void metaUidParse(StockMeta* meta, simdjson::ondemand::value value)
{
    const std::string_view valueStr = value.get_string();
    meta->uid                       = QString::fromUtf8(valueStr.data(), valueStr.size());
}

static void metaTickerParse(StockMeta* meta, simdjson::ondemand::value value)
{
    const std::string_view valueStr = value.get_string();
    meta->ticker                    = QString::fromUtf8(valueStr.data(), valueStr.size());
}

static void metaNameParse(StockMeta* meta, simdjson::ondemand::value value)
{
    const std::string_view valueStr = value.get_string();
    meta->name                      = QString::fromUtf8(valueStr.data(), valueStr.size());
}

static void metaForQualInvestorFlagParse(StockMeta* meta, simdjson::ondemand::value value)
{
    meta->forQualInvestorFlag = value.get_bool().value();
}

static void metaLotParse(StockMeta* meta, simdjson::ondemand::value value)
{
    meta->lot = value.get_int64();
}

static void metaMinPriceIncrementParse(StockMeta* meta, simdjson::ondemand::value value)
{
    meta->minPriceIncrement.fromJsonObject(value.get_object());
}

using ParseHandler = void (*)(StockMeta* meta, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"uid",                 metaUidParse                },
    {"ticker",              metaTickerParse             },
    {"name",                metaNameParse               },
    {"forQualInvestorFlag", metaForQualInvestorFlagParse},
    {"lot",                 metaLotParse                },
    {"minPriceIncrement",   metaMinPriceIncrementParse  }
};
// clang-format on

void StockMeta::fromJsonObject(simdjson::ondemand::object jsonObject)
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key);

        parseHandler(this, field.value());
    }
}

QJsonObject StockMeta::toJsonObject() const
{
    QJsonObject res;

    res.insert("uid", uid);
    res.insert("ticker", ticker);
    res.insert("name", name);
    res.insert("forQualInvestorFlag", forQualInvestorFlag);
    res.insert("lot", lot);
    res.insert("minPriceIncrement", minPriceIncrement.toJsonObject());

    return res;
}

bool operator==(const StockMeta& lhs, const StockMeta& rhs)
{
    return lhs.uid == rhs.uid && lhs.ticker == rhs.ticker && lhs.name == rhs.name &&
           lhs.forQualInvestorFlag == rhs.forQualInvestorFlag && lhs.lot == rhs.lot &&
           lhs.minPriceIncrement == rhs.minPriceIncrement;
}
