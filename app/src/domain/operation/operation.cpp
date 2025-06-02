#include "src/domain/operation/operation.h"



#include <QJsonArray>



Operation::Operation() :
    timestamp(),
    instrumentId(),
    description(),
    price(),
    fifoItems(),
    avgPriceFifo(),
    avgPriceWavg(),
    quantity(),
    remainedQuantity(),
    payment(),
    avgCost(),
    costFifo(),
    costWavg(),
    commission(),
    yield(),
    yieldWithCommission(),
    yieldWithCommissionPercent(),
    inputMoney(),
    maxInputMoney(),
    totalYieldWithCommission(),
    totalYieldWithCommissionPercent(),
    remainedMoney(),
    totalMoney(),
    paymentPrecision(),
    commissionPrecision()
{
}

void Operation::fromJsonObject(const QJsonObject& jsonObject)
{
    timestamp                       = jsonObject.value("timestamp").toInteger();
    instrumentId                    = jsonObject.value("instrumentId").toString();
    description                     = jsonObject.value("description").toString();
    price                           = jsonObject.value("price").toDouble();
    avgPriceFifo                    = jsonObject.value("avgPriceFifo").toDouble();
    avgPriceWavg                    = jsonObject.value("avgPriceWavg").toDouble();
    quantity                        = jsonObject.value("quantity").toInteger();
    remainedQuantity                = jsonObject.value("remainedQuantity").toInteger();
    payment                         = jsonObject.value("payment").toDouble();
    avgCost                         = jsonObject.value("avgCost").toDouble();
    commission                      = jsonObject.value("commission").toDouble();
    yield                           = jsonObject.value("yield").toDouble();
    yieldWithCommission             = jsonObject.value("yieldWithCommission").toDouble();
    yieldWithCommissionPercent      = jsonObject.value("yieldWithCommissionPercent").toDouble();
    totalYieldWithCommissionPercent = jsonObject.value("totalYieldWithCommissionPercent").toDouble();
    paymentPrecision                = jsonObject.value("paymentPrecision").toInt();
    commissionPrecision             = jsonObject.value("commissionPrecision").toInt();

    costFifo.fromJsonObject(jsonObject.value("costFifo").toObject());
    costWavg.fromJsonObject(jsonObject.value("costWavg").toObject());
    inputMoney.fromJsonObject(jsonObject.value("inputMoney").toObject());
    maxInputMoney.fromJsonObject(jsonObject.value("maxInputMoney").toObject());
    totalYieldWithCommission.fromJsonObject(jsonObject.value("totalYieldWithCommission").toObject());
    remainedMoney.fromJsonObject(jsonObject.value("remainedMoney").toObject());
    totalMoney.fromJsonObject(jsonObject.value("totalMoney").toObject());

    const QJsonArray jsonFifoItems = jsonObject.value("fifoItems").toArray();
    fifoItems.resizeForOverwrite(jsonFifoItems.size());

    for (int i = 0; i < jsonFifoItems.size(); ++i)
    {
        fifoItems[i].fromJsonObject(jsonFifoItems.at(i).toObject());
    }
}

QJsonObject Operation::toJsonObject() const
{
    QJsonObject res;

    QJsonArray jsonFifoItems;

    for (const OperationFifoItem& fifoItem : fifoItems)
    {
        jsonFifoItems.append(fifoItem.toJsonObject());
    }

    // clang-format off
    res.insert("timestamp",                       timestamp);
    res.insert("instrumentId",                    instrumentId);
    res.insert("description",                     description);
    res.insert("price",                           price);
    res.insert("fifoItems",                       jsonFifoItems);
    res.insert("avgPriceFifo",                    avgPriceFifo);
    res.insert("avgPriceWavg",                    avgPriceWavg);
    res.insert("quantity",                        quantity);
    res.insert("remainedQuantity",                remainedQuantity);
    res.insert("payment",                         payment);
    res.insert("avgCost",                         avgCost);
    res.insert("costFifo",                        costFifo.toJsonObject());
    res.insert("costWavg",                        costWavg.toJsonObject());
    res.insert("commission",                      commission);
    res.insert("yield",                           yield);
    res.insert("yieldWithCommission",             yieldWithCommission);
    res.insert("yieldWithCommissionPercent",      yieldWithCommissionPercent);
    res.insert("inputMoney",                      inputMoney.toJsonObject());
    res.insert("maxInputMoney",                   maxInputMoney.toJsonObject());
    res.insert("totalYieldWithCommission",        totalYieldWithCommission.toJsonObject());
    res.insert("totalYieldWithCommissionPercent", totalYieldWithCommissionPercent);
    res.insert("remainedMoney",                   remainedMoney.toJsonObject());
    res.insert("totalMoney",                      totalMoney.toJsonObject());
    res.insert("paymentPrecision",                paymentPrecision);
    res.insert("commissionPrecision",             commissionPrecision);
    // clang-format on

    return res;
}

bool operator==(const Operation& lhs, const Operation& rhs)
{
    return lhs.timestamp == rhs.timestamp && lhs.instrumentId == rhs.instrumentId && lhs.description == rhs.description &&
           lhs.price == rhs.price && lhs.fifoItems == rhs.fifoItems && lhs.avgPriceFifo == rhs.avgPriceFifo &&
           lhs.avgPriceWavg == rhs.avgPriceWavg && lhs.quantity == rhs.quantity && lhs.remainedQuantity == rhs.remainedQuantity &&
           lhs.payment == rhs.payment && lhs.avgCost == rhs.avgCost && lhs.costFifo == rhs.costFifo &&
           lhs.costWavg == rhs.costWavg && lhs.commission == rhs.commission && lhs.yield == rhs.yield &&
           lhs.yieldWithCommission == rhs.yieldWithCommission &&
           lhs.yieldWithCommissionPercent == rhs.yieldWithCommissionPercent && lhs.inputMoney == rhs.inputMoney &&
           lhs.maxInputMoney == rhs.maxInputMoney && lhs.totalYieldWithCommission == rhs.totalYieldWithCommission &&
           lhs.totalYieldWithCommissionPercent == rhs.totalYieldWithCommissionPercent && lhs.remainedMoney == rhs.remainedMoney &&
           lhs.totalMoney == rhs.totalMoney && lhs.paymentPrecision == rhs.paymentPrecision &&
           lhs.commissionPrecision == rhs.commissionPrecision;
}
