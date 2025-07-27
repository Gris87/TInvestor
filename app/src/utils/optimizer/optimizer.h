#pragma once



#include "src/utils/optimizer/ioptimizer.h"



class Optimizer : public IOptimizer
{
public:
    Optimizer();
    ~Optimizer() override;

    Optimizer(const Optimizer& another)            = delete;
    Optimizer& operator=(const Optimizer& another) = delete;

    QList<Operation>
    optimizeOperations(const QList<Operation>& operations, int optimizeSize, const QStringList& instruments) override;
    QList<LogEntry> optimizeLogs(const QList<LogEntry>& entries, int optimizeSize) override;

private:
    void addInstrumentsAfterOptimization(
        QList<Operation>& newOperations, const QList<Operation>& oldOperations, const QStringList& instruments
    );
};
