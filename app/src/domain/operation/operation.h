#pragma once



#include <QJsonObject>

#include "src/domain/quotation/quotation.h"



struct Operation
{
    Operation();
    Operation(const Operation& another) = default;
    ~Operation()                        = default;

    Operation& operator=(const Operation& another) = default;

    void fromJsonObject(const QJsonObject& jsonObject);

    [[nodiscard]]
    QJsonObject toJsonObject() const;

    qint64    timestamp;
    QString   instrumentId;
    QString   description;
    float     price;
    Quotation avgPrice;
    qint64    quantity;
    qint64    remainedQuantity;
    float     payment;
    float     commission;
    float     yield;
    float     yieldWithCommission;
    float     yieldWithCommissionPercent;
    Quotation remainedMoney;
    Quotation totalMoney;
    qint8     pricePrecision;
    qint8     avgPricePrecision;
    qint8     paymentPrecision;
    qint8     commissionPrecision;
    qint8     yieldPrecision;
    qint8     yieldWithCommissionPrecision;
    qint8     remainedMoneyPrecision;
    qint8     totalMoneyPrecision;
};



bool operator==(const Operation& lhs, const Operation& rhs);
