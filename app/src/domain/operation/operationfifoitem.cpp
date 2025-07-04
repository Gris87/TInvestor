#include "src/domain/operation/operationfifoitem.h"

#include "src/utils/exception/exception.h"



OperationFifoItem::OperationFifoItem() :
    quantity(),
    cost()
{
}

static void itemQuantityParse(OperationFifoItem* item, simdjson::ondemand::value value)
{
    item->quantity = value.get_int64();
}

static void itemCostParse(OperationFifoItem* item, simdjson::ondemand::value value)
{
    item->cost.fromJsonObject(value.get_object());
}

static void itemThrowParseException(
    OperationFifoItem* /*item*/, simdjson::ondemand::value /*value*/ // clazy:exclude=function-args-by-ref
)
{
    throwException("Unknown parameter");
}

using ParseHandler = void (*)(OperationFifoItem* item, simdjson::ondemand::value value);

// clang-format off
static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{ // clazy:exclude=non-pod-global-static
    {"quantity", itemQuantityParse},
    {"cost",     itemCostParse    }
};
// clang-format on

void OperationFifoItem::fromJsonObject(simdjson::ondemand::object jsonObject) // clazy:exclude=function-args-by-ref
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        const std::string_view key          = field.escaped_key();
        ParseHandler           parseHandler = PARSE_HANDLER.value(key, itemThrowParseException);

        parseHandler(this, field.value());
    }
}

QJsonObject OperationFifoItem::toJsonObject() const
{
    QJsonObject res;

    // clang-format off
    res.insert("quantity", quantity);
    res.insert("cost",     cost.toJsonObject());
    // clang-format on

    return res;
}

bool operator==(const OperationFifoItem& lhs, const OperationFifoItem& rhs)
{
    return lhs.quantity == rhs.quantity && lhs.cost == rhs.cost;
}
