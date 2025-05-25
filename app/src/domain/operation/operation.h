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
    float     avgPrice;
    qint64    quantity;
    qint64    remainedQuantity;
    float     payment;
    float     avgCost;
    Quotation cost;
    float     commission;
    float     yield;
    float     yieldWithCommission;
    float     yieldWithCommissionPercent;
    Quotation remainedMoney;
    Quotation totalMoney;
    Quotation totalCost;
    Quotation totalYieldWithCommission;
    float     totalYieldWithCommissionPercent;
    qint8     paymentPrecision;
    qint8     commissionPrecision;
};



bool operator==(const Operation& lhs, const Operation& rhs);
