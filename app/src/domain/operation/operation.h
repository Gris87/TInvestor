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
    qint64    quantity;
    float     payment;
    float     commission;
    float     yield;
    Quotation moneyOnAccountRemained;
    Quotation moneyOnAccountTotal;
    qint8     pricePrecision;
    qint8     paymentPrecision;
    qint8     commissionPrecision;
    qint8     yieldPrecision;
    qint8     moneyOnAccountRemainedPrecision;
    qint8     moneyOnAccountTotalPrecision;
};



bool operator==(const Operation& lhs, const Operation& rhs);
