#pragma once



#include "src/domain/log/logentry.h"
#include "src/domain/operation/operation.h"
#include "src/domain/quantityandcost/quantityandcost.h"



class IOptimizer
{
public:
    IOptimizer()          = default;
    virtual ~IOptimizer() = default;

    IOptimizer(const IOptimizer& another)            = delete;
    IOptimizer& operator=(const IOptimizer& another) = delete;

    virtual QList<Operation>
    optimizeOperations(const QList<Operation>& operations, int optimizeSize, const QuantityAndCostInstruments& instruments) = 0;
    virtual QList<LogEntry> optimizeLogs(const QList<LogEntry>& entries, int optimizeSize)                                  = 0;
};
