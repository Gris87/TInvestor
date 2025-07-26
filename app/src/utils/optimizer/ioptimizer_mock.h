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

    MOCK_METHOD(void, optimizeOperations, (), (override));
    MOCK_METHOD(void, optimizeLogs, (), (override));
};
