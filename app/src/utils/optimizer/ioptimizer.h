#pragma once



class IOptimizer
{
public:
    IOptimizer()          = default;
    virtual ~IOptimizer() = default;

    IOptimizer(const IOptimizer& another)            = delete;
    IOptimizer& operator=(const IOptimizer& another) = delete;

    virtual void optimizeOperations() = 0;
    virtual void optimizeLogs()       = 0;
};
