#pragma once



#include <QJsonObject>

#include "src/domain/operation/operationfifoitem.h"
#include "src/domain/quotation/quotation.h"



struct Operation
{
    Operation();
    Operation(const Operation& another) = default;
    ~Operation()                        = default;

    Operation& operator=(const Operation& another) = default;

    void fromJsonObject(simdjson::ondemand::object jsonObject); // clazy:exclude=function-args-by-ref

    [[nodiscard]]
    QJsonObject toJsonObject() const;

    qint64                   timestamp;
    QString                  instrumentId;
    QPixmap*                 instrumentLogo;
    QString                  instrumentTicker;
    QString                  instrumentName;
    QString                  description;
    float                    price;
    QList<OperationFifoItem> fifoItems;
    float                    avgPriceFifo;
    float                    avgPriceWavg;
    qint64                   quantity;
    qint64                   remainedQuantity;
    float                    payment;
    float                    avgCostFifo;
    Quotation                costFifo;
    Quotation                costWavg;
    float                    commission;
    float                    yield;
    float                    yieldWithCommission;
    float                    yieldWithCommissionPercent;
    Quotation                inputMoney;
    Quotation                maxInputMoney;
    Quotation                totalYieldWithCommission;
    float                    totalYieldWithCommissionPercent;
    Quotation                remainedMoney;
    Quotation                totalMoney;
    qint8                    pricePrecision;
    qint8                    paymentPrecision;
    qint8                    commissionPrecision;
};



bool operator==(const Operation& lhs, const Operation& rhs);
