#pragma once



#include "src/utils/optimizer/ioptimizer.h"



class Optimizer : public IOptimizer
{
public:
    Optimizer();
    ~Optimizer() override;

    Optimizer(const Optimizer& another)            = delete;
    Optimizer& operator=(const Optimizer& another) = delete;

    void optimizeOperations() override;
    void optimizeLogs() override;
};
