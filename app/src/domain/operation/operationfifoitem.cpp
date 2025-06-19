#include "src/domain/operation/operationfifoitem.h"



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

using ParseHandler = void (*)(OperationFifoItem* item, simdjson::ondemand::value value);

static const QMap<std::string_view, ParseHandler> PARSE_HANDLER{
    {"quantity", itemQuantityParse},
    {"cost",     itemCostParse    }
};

void OperationFifoItem::fromJsonObject(simdjson::ondemand::object jsonObject)
{
    for (simdjson::ondemand::field field : jsonObject)
    {
        std::string_view key          = field.escaped_key();
        ParseHandler     parseHandler = PARSE_HANDLER.value(key);

        if (parseHandler != nullptr)
        {
            parseHandler(this, field.value());
        }
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
