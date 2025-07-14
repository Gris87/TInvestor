#include "src/domain/portfolio/portfoliocategoryitem.h"

#include <QJsonArray>

#include "src/utils/exception/exception.h"



constexpr float  FLOAT_EPSILON  = 0.0001f;
constexpr double DOUBLE_EPSILON = 0.0001;



PortfolioCategoryItem::PortfolioCategoryItem() :
    id(),
    name(),
    cost(),
    part(),
    items()
{
}

static void categoryIdParse(PortfolioCategoryItem* category, simdjson::ondemand::value value)
{
    category->id = value.get_int64().value();
}

static void categoryNameParse(PortfolioCategoryItem* category, simdjson::ondemand::value value)
{
    const std::string_view valueStr = value.get_string();
    category->name                  = QString::fromUtf8(valueStr.data(), valueStr.size());
}

static void categoryCostParse(PortfolioCategoryItem* category, simdjson::ondemand::value value)
{
    category->cost = value.get_double_in_string().value();
}

static void categoryPartParse(PortfolioCategoryItem* category, simdjson::ondemand::value value)
{
    category->part = value.get_double_in_string().value();
}

static void categoryItemsParse(PortfolioCategoryItem* category, simdjson::ondemand::value value)
{
    simdjson::ondemand::array jsonArray = value.get_array();

    category->items.resizeForOverwrite(jsonArray.count_elements());
    int i = 0;

    for (const simdjson::ondemand::object jsonObject : jsonArray)
    {
        category->items[i].fromJsonObject(jsonObject);
        ++i;
    }
}

static void categoryThrowParseException(
    PortfolioCategoryItem* /*category*/, simdjson::ondemand::value /*value*/ // clazy:exclude=function-args-by-ref
)
{
    throwException("Unknown parameter");
}

using ParseHandler = void (*)(PortfolioCategoryItem* category, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"id",    categoryIdParse   },
    {"name",  categoryNameParse },
    {"cost",  categoryCostParse },
    {"part",  categoryPartParse },
    {"items", categoryItemsParse}
};
// clang-format on

void PortfolioCategoryItem::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, categoryThrowParseException);

        parseHandler(this, field.value());
    }
}

QJsonObject PortfolioCategoryItem::toJsonObject() const
{
    QJsonObject res;

    QJsonArray jsonItems;

    for (const PortfolioItem& item : items)
    {
        jsonItems.append(item.toJsonObject());
    }

    // clang-format off
    res.insert("id",    id);
    res.insert("name",  name);
    res.insert("cost",  QString::number(cost, 'f', 2));
    res.insert("part",  QString::number(part, 'f', 2));
    res.insert("items", jsonItems);
    // clang-format on

    return res;
}

bool operator==(const PortfolioCategoryItem& lhs, const PortfolioCategoryItem& rhs)
{
    return lhs.id == rhs.id && lhs.name == rhs.name && qAbs(lhs.cost - rhs.cost) < DOUBLE_EPSILON &&
           qAbs(lhs.part - rhs.part) < FLOAT_EPSILON && lhs.items == rhs.items;
}
