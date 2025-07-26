#pragma once



#include "src/utils/optimizer/ioptimizer.h"

#include <gmock/gmock.h>



class OptimizerMock : public IOptimizer
{
public:
    OptimizerMock() :
        IOptimizer()
    {
    }
    ~OptimizerMock() override = default;

    OptimizerMock(const OptimizerMock& another)            = delete;
    OptimizerMock& operator=(const OptimizerMock& another) = delete;

    MOCK_METHOD(
        QList<Operation>,
        optimizeOperations,
        (const QList<Operation>& operations, int optimizeSize, const QuantityAndCostInstruments& instruments),
        (override)
    );
    MOCK_METHOD(QList<LogEntry>, optimizeLogs, (const QList<LogEntry>& entries, int optimizeSize), (override));
};
