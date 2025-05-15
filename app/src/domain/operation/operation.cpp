#include "src/domain/operation/operation.h"



Operation::Operation() :
    timestamp(),
    instrumentId(),
    description(),
    price(),
    quantity(),
    payment(),
    commission(),
    yield(),
    moneyOnAccountRemained(),
    moneyOnAccountTotal(),
    pricePrecision(),
    paymentPrecision(),
    commissionPrecision(),
    yieldPrecision(),
    moneyOnAccountRemainedPrecision(),
    moneyOnAccountTotalPrecision()
{
}

void Operation::fromJsonObject(const QJsonObject& jsonObject)
{
    timestamp                       = jsonObject.value("timestamp").toInteger();
    instrumentId                    = jsonObject.value("instrumentId").toString();
    description                     = jsonObject.value("description").toString();
    price                           = jsonObject.value("price").toDouble();
    quantity                        = jsonObject.value("quantity").toInteger();
    payment                         = jsonObject.value("payment").toDouble();
    commission                      = jsonObject.value("commission").toDouble();
    yield                           = jsonObject.value("yield").toDouble();
    pricePrecision                  = jsonObject.value("pricePrecision").toInt();
    paymentPrecision                = jsonObject.value("paymentPrecision").toInt();
    commissionPrecision             = jsonObject.value("commissionPrecision").toInt();
    yieldPrecision                  = jsonObject.value("yieldPrecision").toInt();
    moneyOnAccountRemainedPrecision = jsonObject.value("moneyOnAccountRemainedPrecision").toInt();
    moneyOnAccountTotalPrecision    = jsonObject.value("moneyOnAccountTotalPrecision").toInt();

    moneyOnAccountRemained.fromJsonObject(jsonObject.value("moneyOnAccountRemained").toObject());
    moneyOnAccountTotal.fromJsonObject(jsonObject.value("moneyOnAccountTotal").toObject());
}

QJsonObject Operation::toJsonObject() const
{
    QJsonObject res;

    // clang-format off
    res.insert("timestamp",                       timestamp);
    res.insert("instrumentId",                    instrumentId);
    res.insert("description",                     description);
    res.insert("price",                           price);
    res.insert("quantity",                        quantity);
    res.insert("payment",                         payment);
    res.insert("commission",                      commission);
    res.insert("yield",                           yield);
    res.insert("moneyOnAccountRemained",          moneyOnAccountRemained.toJsonObject());
    res.insert("moneyOnAccountTotal",             moneyOnAccountTotal.toJsonObject());
    res.insert("pricePrecision",                  pricePrecision);
    res.insert("paymentPrecision",                paymentPrecision);
    res.insert("commissionPrecision",             commissionPrecision);
    res.insert("yieldPrecision",                  yieldPrecision);
    res.insert("moneyOnAccountRemainedPrecision", moneyOnAccountRemainedPrecision);
    res.insert("moneyOnAccountTotalPrecision",    moneyOnAccountTotalPrecision);
    // clang-format on

    return res;
}

bool operator==(const Operation& lhs, const Operation& rhs)
{
    return lhs.timestamp == rhs.timestamp && lhs.instrumentId == rhs.instrumentId && lhs.description == rhs.description &&
           lhs.price == rhs.price && lhs.quantity == rhs.quantity && lhs.payment == rhs.payment &&
           lhs.commission == rhs.commission && lhs.yield == rhs.yield &&
           lhs.moneyOnAccountRemained == rhs.moneyOnAccountRemained && lhs.moneyOnAccountTotal == rhs.moneyOnAccountTotal &&
           lhs.pricePrecision == rhs.pricePrecision && lhs.paymentPrecision == rhs.paymentPrecision &&
           lhs.commissionPrecision == rhs.commissionPrecision && lhs.yieldPrecision == rhs.yieldPrecision &&
           lhs.moneyOnAccountRemainedPrecision == rhs.moneyOnAccountRemainedPrecision &&
           lhs.moneyOnAccountTotalPrecision == rhs.moneyOnAccountTotalPrecision;
}
