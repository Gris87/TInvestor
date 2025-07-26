#pragma once



#include "src/domain/operation/operationfifoitem.h"
#include "src/domain/quotation/quotation.h"



struct QuantityAndCost
{
    explicit QuantityAndCost();

    qint64                   quantity;
    QList<OperationFifoItem> fifoItems;
    Quotation                costFifo;
    Quotation                costWavg;
};

struct QuantityAndCostDouble
{
    explicit QuantityAndCostDouble();

    qint64 quantity;
    double cost;
};

using QuantityAndCostInstruments = QMap<QString, QuantityAndCost>; // Instrument Id => QuantityAndCost
