#include "src/utils/optimizer/optimizer.h"

#include <QDebug>

#include "src/threads/parallelhelper/parallelhelperthread.h"



Optimizer::Optimizer() :
    IOptimizer()
{
    qDebug() << "Create Optimizer";
}

Optimizer::~Optimizer()
{
    qDebug() << "Destroy Optimizer";
}

struct OptimizeOperationsInfo
{
    explicit OptimizeOperationsInfo(const QList<Operation>* _operations) :
        operations(_operations)
    {
    }

    const QList<Operation>* operations;
};

static void optimizeOperationsForParallel(
    QThread* parentThread, int /*threadId*/, QList<Operation>& res, int start, int end, void* additionalArgs
)
{
    OptimizeOperationsInfo* optimizeOperationsInfo = reinterpret_cast<OptimizeOperationsInfo*>(additionalArgs);

    const Operation* operationsArray = optimizeOperationsInfo->operations->data();

    Operation* resArray = res.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        resArray[i] = operationsArray[i];
    }
}

QList<Operation>
Optimizer::optimizeOperations(const QList<Operation>& operations, int optimizeSize, const QuantityAndCostInstruments& instruments)
{
    QList<Operation> res;
    res.resizeForOverwrite(optimizeSize);

    OptimizeOperationsInfo optimizeOperationsInfo(&operations);
    processInParallel(res, optimizeOperationsForParallel, &optimizeOperationsInfo);

    addInstrumentsAfterOptimization(res, operations, instruments);

    return res;
}

struct OptimizeLogsInfo
{
    explicit OptimizeLogsInfo(const QList<LogEntry>* _entries) :
        entries(_entries)
    {
    }

    const QList<LogEntry>* entries;
};

static void
optimizeLogsForParallel(QThread* parentThread, int /*threadId*/, QList<LogEntry>& res, int start, int end, void* additionalArgs)
{
    OptimizeLogsInfo* optimizeLogsInfo = reinterpret_cast<OptimizeLogsInfo*>(additionalArgs);

    const LogEntry* entriesArray = optimizeLogsInfo->entries->data();

    LogEntry* resArray = res.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        resArray[i] = entriesArray[i];
    }
}

QList<LogEntry> Optimizer::optimizeLogs(const QList<LogEntry>& entries, int optimizeSize)
{
    QList<LogEntry> res;
    res.resizeForOverwrite(optimizeSize);

    OptimizeLogsInfo optimizeLogsInfo(&entries);
    processInParallel(res, optimizeLogsForParallel, &optimizeLogsInfo);

    return res;
}

struct AddInstrumentsInfo
{
    explicit AddInstrumentsInfo(const QList<Operation>* _oldOperations) :
        oldOperations(_oldOperations)
    {
#ifndef TESTING_MODE
        const int cpuCount = QThread::idealThreadCount();
#else
        const int cpuCount = 1;
#endif

        results.resize(cpuCount);
    }

    const QList<Operation>* oldOperations;
    QList<QList<Operation>> results;
};

static void addInstrumentsForParallel(
    QThread* parentThread, int threadId, QList<QString>& instrumentsToAdd, int start, int end, void* additionalArgs
)
{
    AddInstrumentsInfo* addInstrumentsInfo = reinterpret_cast<AddInstrumentsInfo*>(additionalArgs);

    const Operation*  oldOperationsArray = addInstrumentsInfo->oldOperations->data();
    const int         oldOperationsSize  = addInstrumentsInfo->oldOperations->size();
    QList<Operation>* resultsArray       = addInstrumentsInfo->results.data();

    QString* instrumentsArray = instrumentsToAdd.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        const QString& instrumentId = instrumentsArray[i];

        for (int j = 0; j < oldOperationsSize; ++j)
        {
            if (oldOperationsArray[j].instrumentId == instrumentId)
            {
                resultsArray[threadId].append(oldOperationsArray[j]);

                break;
            }
        }
    }
}

void Optimizer::addInstrumentsAfterOptimization(
    QList<Operation>& newOperations, const QList<Operation>& oldOperations, const QuantityAndCostInstruments& instruments
)
{
    QSet<QString> newInstruments; // Instrument Id

    for (int i = newOperations.size() - 1; i >= 0; --i)
    {
        const Operation& operation = newOperations.at(i);

        if (operation.remainedQuantity > 0)
        {
            newInstruments.insert(operation.instrumentId);
        }
        else
        {
            newInstruments.remove(operation.instrumentId);
        }
    }

    QList<QString> instrumentsToAdd;

    for (auto it = instruments.constBegin(); it != instruments.constEnd(); ++it)
    {
        if (!newInstruments.contains(it.key()))
        {
            instrumentsToAdd.append(it.key());
        }
    }

    AddInstrumentsInfo addInstrumentsInfo(&oldOperations);
    processInParallel(instrumentsToAdd, addInstrumentsForParallel, &addInstrumentsInfo);

    QList<Operation> operationsWithInstruments;

    for (const QList<Operation>& operations : std::as_const(addInstrumentsInfo.results))
    {
        operationsWithInstruments.append(operations);
    }

    std::sort(operationsWithInstruments.begin(), operationsWithInstruments.end(), [](const Operation& l, const Operation& r) {
        return l.timestamp > r.timestamp;
    });

    newOperations.append(operationsWithInstruments);
}
