#include "src/domain/operation/operation.h"



Operation::Operation() :
    timestamp(),
    instrumentId(),
    description(),
    price(),
    quantity(),
    payment(),
    commission(),
    yield()
{
}

void Operation::fromJsonObject(const QJsonObject& jsonObject)
{
    timestamp    = jsonObject.value("timestamp").toInteger();
    instrumentId = jsonObject.value("instrumentId").toString();
    description  = jsonObject.value("description").toString();
    price        = jsonObject.value("price").toDouble();
    quantity     = jsonObject.value("quantity").toInt();
    payment      = jsonObject.value("payment").toDouble();
    commission   = jsonObject.value("commission").toDouble();
    yield        = jsonObject.value("yield").toDouble();
}

QJsonObject Operation::toJsonObject() const
{
    QJsonObject res;

    // clang-format off
    res.insert("timestamp",    timestamp);
    res.insert("instrumentId", instrumentId);
    res.insert("description",  description);
    res.insert("price",        price);
    res.insert("quantity",     quantity);
    res.insert("payment",      payment);
    res.insert("commission",   commission);
    res.insert("yield",        yield);
    // clang-format on

    return res;
}

bool operator==(const Operation& lhs, const Operation& rhs)
{
    return lhs.timestamp == rhs.timestamp && lhs.instrumentId == rhs.instrumentId && lhs.description == rhs.description &&
           lhs.price == rhs.price && lhs.quantity == rhs.quantity && lhs.payment == rhs.payment &&
           lhs.commission == rhs.commission && lhs.yield == rhs.yield;
}
