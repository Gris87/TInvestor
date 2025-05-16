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
    commission(),
    yield(),
    yieldWithCommission(),
    yieldWithCommissionPercent(),
    remainedMoney(),
    totalMoney(),
    pricePrecision(),
    paymentPrecision(),
    commissionPrecision(),
    yieldPrecision(),
    yieldWithCommissionPrecision(),
    remainedMoneyPrecision(),
    totalMoneyPrecision()
{
}

void Operation::fromJsonObject(const QJsonObject& jsonObject)
{
    timestamp                    = jsonObject.value("timestamp").toInteger();
    instrumentId                 = jsonObject.value("instrumentId").toString();
    description                  = jsonObject.value("description").toString();
    price                        = jsonObject.value("price").toDouble();
    quantity                     = jsonObject.value("quantity").toInteger();
    remainedQuantity             = jsonObject.value("remainedQuantity").toInteger();
    payment                      = jsonObject.value("payment").toDouble();
    commission                   = jsonObject.value("commission").toDouble();
    yield                        = jsonObject.value("yield").toDouble();
    yieldWithCommission          = jsonObject.value("yieldWithCommission").toDouble();
    yieldWithCommissionPercent   = jsonObject.value("yieldWithCommissionPercent").toDouble();
    pricePrecision               = jsonObject.value("pricePrecision").toInt();
    paymentPrecision             = jsonObject.value("paymentPrecision").toInt();
    commissionPrecision          = jsonObject.value("commissionPrecision").toInt();
    yieldPrecision               = jsonObject.value("yieldPrecision").toInt();
    yieldWithCommissionPrecision = jsonObject.value("yieldWithCommissionPrecision").toInt();
    remainedMoneyPrecision       = jsonObject.value("remainedMoneyPrecision").toInt();
    totalMoneyPrecision          = jsonObject.value("totalMoneyPrecision").toInt();

    avgPrice.fromJsonObject(jsonObject.value("avgPrice").toObject());
    remainedMoney.fromJsonObject(jsonObject.value("remainedMoney").toObject());
    totalMoney.fromJsonObject(jsonObject.value("totalMoney").toObject());
}

QJsonObject Operation::toJsonObject() const
{
    QJsonObject res;

    // clang-format off
    res.insert("timestamp",                    timestamp);
    res.insert("instrumentId",                 instrumentId);
    res.insert("description",                  description);
    res.insert("price",                        price);
    res.insert("avgPrice",                     avgPrice.toJsonObject());
    res.insert("quantity",                     quantity);
    res.insert("remainedQuantity",             remainedQuantity);
    res.insert("payment",                      payment);
    res.insert("commission",                   commission);
    res.insert("yield",                        yield);
    res.insert("yieldWithCommission",          yieldWithCommission);
    res.insert("yieldWithCommissionPercent",   yieldWithCommissionPercent);
    res.insert("remainedMoney",                remainedMoney.toJsonObject());
    res.insert("totalMoney",                   totalMoney.toJsonObject());
    res.insert("pricePrecision",               pricePrecision);
    res.insert("paymentPrecision",             paymentPrecision);
    res.insert("commissionPrecision",          commissionPrecision);
    res.insert("yieldPrecision",               yieldPrecision);
    res.insert("yieldWithCommissionPrecision", yieldWithCommissionPrecision);
    res.insert("remainedMoneyPrecision",       remainedMoneyPrecision);
    res.insert("totalMoneyPrecision",          totalMoneyPrecision);
    // clang-format on

    return res;
}

bool operator==(const Operation& lhs, const Operation& rhs)
{
    return lhs.timestamp == rhs.timestamp && lhs.instrumentId == rhs.instrumentId && lhs.description == rhs.description &&
           lhs.price == rhs.price && lhs.avgPrice == rhs.avgPrice && lhs.quantity == rhs.quantity &&
           lhs.remainedQuantity == rhs.remainedQuantity && lhs.payment == rhs.payment && lhs.commission == rhs.commission &&
           lhs.yield == rhs.yield && lhs.yieldWithCommission == rhs.yieldWithCommission &&
           lhs.yieldWithCommissionPercent == rhs.yieldWithCommissionPercent && lhs.remainedMoney == rhs.remainedMoney &&
           lhs.totalMoney == rhs.totalMoney && lhs.pricePrecision == rhs.pricePrecision &&
           lhs.paymentPrecision == rhs.paymentPrecision && lhs.commissionPrecision == rhs.commissionPrecision &&
           lhs.yieldPrecision == rhs.yieldPrecision && lhs.yieldWithCommissionPrecision == rhs.yieldWithCommissionPrecision &&
           lhs.remainedMoneyPrecision == rhs.remainedMoneyPrecision && lhs.totalMoneyPrecision == rhs.totalMoneyPrecision;
}
