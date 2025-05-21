#include "src/domain/operation/operation.h"



Operation::Operation() :
    timestamp(),
    instrumentId(),
    description(),
    price(),
    avgPrice(),
    quantity(),
    remainedQuantity(),
    payment(),
    avgCost(),
    cost(),
    commission(),
    yield(),
    yieldWithCommission(),
    yieldWithCommissionPercent(),
    remainedMoney(),
    totalMoney(),
    paymentPrecision(),
    commissionPrecision()
{
}

void Operation::fromJsonObject(const QJsonObject& jsonObject)
{
    timestamp                  = jsonObject.value("timestamp").toInteger();
    instrumentId               = jsonObject.value("instrumentId").toString();
    description                = jsonObject.value("description").toString();
    price                      = jsonObject.value("price").toDouble();
    avgPrice                   = jsonObject.value("avgPrice").toDouble();
    quantity                   = jsonObject.value("quantity").toInteger();
    remainedQuantity           = jsonObject.value("remainedQuantity").toInteger();
    payment                    = jsonObject.value("payment").toDouble();
    avgCost                    = jsonObject.value("avgCost").toDouble();
    commission                 = jsonObject.value("commission").toDouble();
    yield                      = jsonObject.value("yield").toDouble();
    yieldWithCommission        = jsonObject.value("yieldWithCommission").toDouble();
    yieldWithCommissionPercent = jsonObject.value("yieldWithCommissionPercent").toDouble();
    paymentPrecision           = jsonObject.value("paymentPrecision").toInt();
    commissionPrecision        = jsonObject.value("commissionPrecision").toInt();

    cost.fromJsonObject(jsonObject.value("cost").toObject());
    remainedMoney.fromJsonObject(jsonObject.value("remainedMoney").toObject());
    totalMoney.fromJsonObject(jsonObject.value("totalMoney").toObject());
}

QJsonObject Operation::toJsonObject() const
{
    QJsonObject res;

    // clang-format off
    res.insert("timestamp",                  timestamp);
    res.insert("instrumentId",               instrumentId);
    res.insert("description",                description);
    res.insert("price",                      price);
    res.insert("avgPrice",                   avgPrice);
    res.insert("quantity",                   quantity);
    res.insert("remainedQuantity",           remainedQuantity);
    res.insert("payment",                    payment);
    res.insert("avgCost",                    avgCost);
    res.insert("cost",                       cost.toJsonObject());
    res.insert("commission",                 commission);
    res.insert("yield",                      yield);
    res.insert("yieldWithCommission",        yieldWithCommission);
    res.insert("yieldWithCommissionPercent", yieldWithCommissionPercent);
    res.insert("remainedMoney",              remainedMoney.toJsonObject());
    res.insert("totalMoney",                 totalMoney.toJsonObject());
    res.insert("paymentPrecision",           paymentPrecision);
    res.insert("commissionPrecision",        commissionPrecision);
    // clang-format on

    return res;
}

bool operator==(const Operation& lhs, const Operation& rhs)
{
    return lhs.timestamp == rhs.timestamp && lhs.instrumentId == rhs.instrumentId && lhs.description == rhs.description &&
           lhs.price == rhs.price && lhs.avgPrice == rhs.avgPrice && lhs.quantity == rhs.quantity &&
           lhs.remainedQuantity == rhs.remainedQuantity && lhs.payment == rhs.payment && lhs.avgCost == rhs.avgCost &&
           lhs.cost == rhs.cost && lhs.commission == rhs.commission && lhs.yield == rhs.yield &&
           lhs.yieldWithCommission == rhs.yieldWithCommission &&
           lhs.yieldWithCommissionPercent == rhs.yieldWithCommissionPercent && lhs.remainedMoney == rhs.remainedMoney &&
           lhs.totalMoney == rhs.totalMoney && lhs.paymentPrecision == rhs.paymentPrecision &&
           lhs.commissionPrecision == rhs.commissionPrecision;
}
