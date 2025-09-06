#pragma once



#include "src/domain/operation/operationfifoitem.h"
#include "src/domain/quotation/quotation.h"



struct QuantityAndCost
{
    explicit QuantityAndCost();
    QuantityAndCost(const QuantityAndCost& another) = default;
    ~QuantityAndCost()                              = default;

    QuantityAndCost& operator=(const QuantityAndCost& another) = default;

    qint64                   quantity;
    QList<OperationFifoItem> fifoItems;
    Quotation                costFifo;
    Quotation                costWavg;
};

struct QuantityAndCostDouble
{
    explicit QuantityAndCostDouble();
    QuantityAndCostDouble(const QuantityAndCostDouble& another) = default;
    ~QuantityAndCostDouble()                                    = default;

    QuantityAndCostDouble& operator=(const QuantityAndCostDouble& another) = default;

    qint64 quantity;
    double cost;
};

using QuantityAndCostInstruments       = QMap<QString, QuantityAndCost>;       // Instrument Id => QuantityAndCost
using QuantityAndCostDoubleInstruments = QMap<QString, QuantityAndCostDouble>; // Instrument Id => QuantityAndCostDouble
