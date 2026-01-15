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
    explicit OptimizeOperationsInfo(const QList<Operation>& _operations)
    {
        operationsArray = _operations.constData();
    }

    const Operation* operationsArray;
};

static void optimizeOperationsForParallel(
    QThread* parentThread, int /*threadId*/, Operation* res, int /*size*/, int start, int end, void* additionalArgs
)
{
    const OptimizeOperationsInfo* optimizeOperationsInfo = reinterpret_cast<OptimizeOperationsInfo*>(additionalArgs);

    const Operation* operationsArray = optimizeOperationsInfo->operationsArray;

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        res[i] = operationsArray[i];
    }
}

QList<Operation>
Optimizer::optimizeOperations(const QList<Operation>& operations, int optimizeSize, const QStringList& instruments)
{
    QList<Operation> res;
    res.resizeForOverwrite(optimizeSize);

    OptimizeOperationsInfo optimizeOperationsInfo(operations);
    processInParallel(QThread::currentThread(), res, optimizeOperationsForParallel, &optimizeOperationsInfo);

    addInstrumentsAfterOptimization(res, operations, instruments);

    return res;
}

struct OptimizeLogsInfo
{
    explicit OptimizeLogsInfo(const QList<LogEntry>& _entries)
    {
        entriesArray = _entries.constData();
    }

    const LogEntry* entriesArray;
};

static void optimizeLogsForParallel(
    QThread* parentThread, int /*threadId*/, LogEntry* res, int /*size*/, int start, int end, void* additionalArgs
)
{
    const OptimizeLogsInfo* optimizeLogsInfo = reinterpret_cast<OptimizeLogsInfo*>(additionalArgs);

    const LogEntry* entriesArray = optimizeLogsInfo->entriesArray;

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        res[i] = entriesArray[i];
    }
}

QList<LogEntry> Optimizer::optimizeLogs(const QList<LogEntry>& entries, int optimizeSize)
{
    QList<LogEntry> res;
    res.resizeForOverwrite(optimizeSize);

    OptimizeLogsInfo optimizeLogsInfo(entries);
    processInParallel(QThread::currentThread(), res, optimizeLogsForParallel, &optimizeLogsInfo);

    return res;
}

struct AddInstrumentsInfo
{
    explicit AddInstrumentsInfo(const QList<Operation>* _oldOperations)
    {
        oldOperationsArray = _oldOperations->constData();
        oldOperationsSize  = _oldOperations->size();

        results.resize(getCpuCount());
        resultsArray = results.data();
    }

    const Operation*        oldOperationsArray;
    int                     oldOperationsSize;
    QList<QList<Operation>> results;
    QList<Operation>*       resultsArray;
};

static void addInstrumentsForParallel(
    QThread* parentThread, int threadId, QString* instrumentsToAdd, int /*size*/, int start, int end, void* additionalArgs
)
{
    const AddInstrumentsInfo* addInstrumentsInfo = reinterpret_cast<AddInstrumentsInfo*>(additionalArgs);

    const Operation*  oldOperationsArray = addInstrumentsInfo->oldOperationsArray;
    const int         oldOperationsSize  = addInstrumentsInfo->oldOperationsSize;
    QList<Operation>* resultsArray       = addInstrumentsInfo->resultsArray;

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        const QString& instrumentId = instrumentsToAdd[i];

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
    QList<Operation>& newOperations, const QList<Operation>& oldOperations, const QStringList& instruments
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

    for (const QString& instrumentId : instruments)
    {
        if (!newInstruments.contains(instrumentId))
        {
            instrumentsToAdd.append(instrumentId);
        }
    }

    AddInstrumentsInfo addInstrumentsInfo(&oldOperations);
    processInParallel(QThread::currentThread(), instrumentsToAdd, addInstrumentsForParallel, &addInstrumentsInfo);

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
