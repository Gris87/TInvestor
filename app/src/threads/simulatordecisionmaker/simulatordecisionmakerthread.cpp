#include "src/threads/simulatordecisionmaker/simulatordecisionmakerthread.h"

#include <QDebug>

#include "src/grpc/utils.h"
#include "src/threads/parallelhelper/parallelhelperthread.h"
#include "src/utils/tradesimulation/tradesimulation.h"



constexpr int   LIMIT_OPERATIONS         = 100000;
constexpr int   OPTIMIZE_OPERATIONS_SIZE = 10000;
constexpr int   LIMIT_LOGS               = 1000000;
constexpr int   OPTIMIZE_LOGS_SIZE       = 10000;
constexpr float HUNDRED_PERCENT          = 100.0f;

constexpr int SHARE_ID = 1;



SimulatorDecisionMakerThread::SimulatorDecisionMakerThread(
    ISettingsEditor*     settingsEditor,
    IOperationsDatabase* operationsDatabase,
    ILogsDatabase*       logsDatabase,
    IPortfolioDatabase*  portfolioDatabase,
    IInstrumentsStorage* instrumentsStorage,
    ILogosStorage*       logosStorage,
    IUserStorage*        userStorage,
    IStocksStorage*      stocksStorage,
    IConfig*             config,
    IDecisionMaker*      decisionMaker,
    IOptimizer*          optimizer,
    QObject*             parent
) :
    ISimulatorDecisionMakerThread(parent),
    mSettingsEditor(settingsEditor),
    mOperationsDatabase(operationsDatabase),
    mLogsDatabase(logsDatabase),
    mPortfolioDatabase(portfolioDatabase),
    mInstrumentsStorage(instrumentsStorage),
    mLogosStorage(logosStorage),
    mUserStorage(userStorage),
    mStocksStorage(stocksStorage),
    mConfig(config),
    mDecisionMaker(decisionMaker),
    mOptimizer(optimizer),
    mPortfolio(),
    mAmountOfOperations(),
    mAmountOfLogs(),
    mLimitOperations(LIMIT_OPERATIONS),
    mOptimizeOperationsSize(OPTIMIZE_OPERATIONS_SIZE),
    mLimitLogs(LIMIT_LOGS),
    mOptimizeLogsSize(OPTIMIZE_LOGS_SIZE),
    mStocksMap(),
    mInstruments(),
    mInstrumentSells(),
    mResetted(),
    mLoaded(),
    mStartMoney(),
    mTotalMoney()
{
    qDebug() << "Create SimulatorDecisionMakerThread";
}

SimulatorDecisionMakerThread::~SimulatorDecisionMakerThread()
{
    qDebug() << "Destroy SimulatorDecisionMakerThread";
}

void SimulatorDecisionMakerThread::run()
{
    qDebug() << "Running SimulatorDecisionMakerThread";

    blockSignals(false);

    if (mResetted)
    {
        init();
        mResetted = false;
        mLoaded   = true;
    }
    else if (!mLoaded)
    {
        load();
        mLoaded = true;
    }

    mStocksStorage->readLock();
    const InstrumentsForTrading& instrumentsForTrading = mDecisionMaker->makeDecision(
        QThread::currentThread(),
        QDateTime::currentMSecsSinceEpoch(),
        mConfig,
        mInstrumentSells,
        mPortfolio,
        mStocksStorage->getStocks(),
        false,
        false,
        true
    );
    mStocksStorage->readUnlock();

    if (!instrumentsForTrading.isEmpty())
    {
        QList<Operation> operations;
        QList<LogEntry>  entries;

        simulateTrading(
            QDateTime::currentMSecsSinceEpoch(),
            mInstrumentsStorage,
            mLogosStorage,
            mUserStorage,
            instrumentsForTrading,
            mStartMoney,
            mTotalMoney,
            operations,
            entries,
            mPortfolio,
            mInstruments,
            mInstrumentSells
        );

        mAmountOfOperations += operations.size();
        mAmountOfLogs       += entries.size();

        if (!operations.isEmpty())
        {
            operations = reverseOperations(operations);

            emit operationsAdded(operations);
            mOperationsDatabase->appendOperations(operations);
        }

        for (const LogEntry& entry : std::as_const(entries))
        {
            emit logAdded(entry);
            mLogsDatabase->appendLog(entry);
        }

        updateCostAndPart();
        updatePrice();

        emit portfolioChanged(mPortfolio);
        mPortfolioDatabase->writePortfolio(mPortfolio);
    }

    optimizeOperations();
    optimizeLogs();

    qDebug() << "Finish SimulatorDecisionMakerThread";
}

bool SimulatorDecisionMakerThread::isLoaded() const
{
    return mLoaded;
}

void SimulatorDecisionMakerThread::reset()
{
    mResetted = true;
}

void SimulatorDecisionMakerThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}

void SimulatorDecisionMakerThread::init()
{
    readSimulationConfig();
    initOperations();
    initLogs();
    initPortfolio();
}

void SimulatorDecisionMakerThread::readSimulationConfig()
{
    mStartMoney = mSettingsEditor->value("Options/StartMoney", 0).toInt();
}

void SimulatorDecisionMakerThread::initOperations()
{
    QList<Operation> operations =
        createInitOperations(mInstrumentsStorage, mLogosStorage, QDateTime::currentMSecsSinceEpoch(), mStartMoney);

    emit operationsRead(operations);
    mOperationsDatabase->writeOperations(operations);
    mAmountOfOperations = operations.size();

    mInstrumentSells.clear();

    mTotalMoney = mStartMoney;
}

void SimulatorDecisionMakerThread::initLogs()
{
    QList<LogEntry> entries = createInitEntries();

    emit logsRead(entries);
    mLogsDatabase->writeLogs(entries);
    mAmountOfLogs = entries.size();
}

void SimulatorDecisionMakerThread::initPortfolio()
{
    mPortfolio = createInitPortfolio(mInstrumentsStorage, mLogosStorage, mStartMoney);

    emit portfolioChanged(mPortfolio);
    mPortfolioDatabase->writePortfolio(mPortfolio);

    mInstruments.clear();
}

void SimulatorDecisionMakerThread::load()
{
    readSimulationConfig();
    loadOperations();
    loadLogs();
    loadPortfolio();
}

void SimulatorDecisionMakerThread::loadOperations()
{
    const QList<Operation> operations = mOperationsDatabase->readOperations();
    emit operationsRead(operations);
    mAmountOfOperations = operations.size();

    mInstrumentSells.clear();

    if (mAmountOfOperations > 0)
    {
        for (int i = operations.size() - 1; i >= 0; --i)
        {
            const Operation& operation = operations.at(i);

            if (operation.remainedQuantity == 0)
            {
                mInstrumentSells[operation.instrumentId] = operation.timestamp;
            }
        }

        const Operation& lastOperation = operations.constFirst(); // Since it reversed

        mTotalMoney = quotationToDouble(lastOperation.totalMoney);
    }
}

void SimulatorDecisionMakerThread::loadLogs()
{
    const QList<LogEntry> entries = mLogsDatabase->readLogs();
    emit logsRead(entries);
    mAmountOfLogs = entries.size();
}

void SimulatorDecisionMakerThread::loadPortfolio()
{
    mPortfolio = mPortfolioDatabase->readPortfolio();
    updatePrice();

    emit portfolioChanged(mPortfolio);

    mInstruments.clear();

    for (const PortfolioCategoryItem& category : std::as_const(mPortfolio.positions))
    {
        for (const PortfolioItem& item : std::as_const(category.items))
        {
            if (item.showPrices)
            {
                QuantityAndCostDouble quantityAndCost;

                quantityAndCost.quantity = item.available;
                quantityAndCost.cost     = item.cost;

                mInstruments[item.instrumentId] = quantityAndCost;
            }
        }
    }
}

struct ReverseOperationsInfo
{
    explicit ReverseOperationsInfo(const QList<Operation>& _operations)
    {
        operationsArray = _operations.constData();
    }

    const Operation* operationsArray;
};

static void reverseOperationsForParallel(
    QThread* parentThread, int /*threadId*/, Operation* res, int size, int start, int end, void* additionalArgs
)
{
    const ReverseOperationsInfo* reverseOperationsInfo = reinterpret_cast<ReverseOperationsInfo*>(additionalArgs);

    const Operation* operationsArray = reverseOperationsInfo->operationsArray;

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        res[i] = operationsArray[size - i - 1];
    }
}

QList<Operation> SimulatorDecisionMakerThread::reverseOperations(const QList<Operation>& operations)
{
    QList<Operation> res;
    res.resizeForOverwrite(operations.size());

    ReverseOperationsInfo reverseOperationsInfo(operations);
    processInParallel(QThread::currentThread(), res, reverseOperationsForParallel, &reverseOperationsInfo);

    return res;
}

void SimulatorDecisionMakerThread::updateCostAndPart()
{
    for (PortfolioCategoryItem& category : mPortfolio.positions)
    {
        category.cost = 0;

        for (PortfolioItem& item : category.items)
        {
            item.part = (item.cost / mTotalMoney) * HUNDRED_PERCENT;

            category.cost += item.cost;
        }

        category.part = (category.cost / mTotalMoney) * HUNDRED_PERCENT;
    }
}

void SimulatorDecisionMakerThread::updatePrice()
{
    PortfolioCategoryItem& category = mPortfolio.positions[SHARE_ID]; // clazy:exclude=detaching-member

    for (PortfolioItem& item : category.items)
    {
        Stock* stock = mStocksMap.value(item.instrumentId);

        if (stock == nullptr)
        {
            mStocksStorage->readLock();
            const QList<Stock*>& stocks = mStocksStorage->getStocks();

            for (Stock* s : stocks)
            {
                s->readLock();
                const QString instrumentId = s->meta.instrumentId;
                s->readUnlock();

                if (instrumentId == item.instrumentId)
                {
                    stock                         = s;
                    mStocksMap[item.instrumentId] = stock;

                    break;
                }
            }
            mStocksStorage->readUnlock();
        }

        if (stock != nullptr)
        {
            stock->readLock();
            item.price = stock->lastPrice();
            stock->readUnlock();

            const double currentCost = item.available * item.price;

            item.yield             = currentCost - item.cost;
            item.yieldPercent      = item.available > 0 ? ((item.price / item.avgPriceFifo) * HUNDRED_PERCENT) - HUNDRED_PERCENT
                                                        : ((item.avgPriceFifo / item.price) * HUNDRED_PERCENT) - HUNDRED_PERCENT;
            item.dailyYield        = currentCost - item.costForDailyYield;
            item.dailyYieldPercent = item.available > 0
                                         ? ((item.price / item.priceForDailyYield) * HUNDRED_PERCENT) - HUNDRED_PERCENT
                                         : ((item.priceForDailyYield / item.price) * HUNDRED_PERCENT) - HUNDRED_PERCENT;
        }
    }
}

void SimulatorDecisionMakerThread::optimizeOperations()
{
    if (mAmountOfOperations > mLimitOperations)
    {
        QList<Operation> newOperations =
            mOptimizer->optimizeOperations(mOperationsDatabase->readOperations(), mOptimizeOperationsSize, mInstruments.keys());
        mAmountOfOperations = newOperations.size();

        emit operationsRead(newOperations);
        mOperationsDatabase->writeOperations(newOperations);
    }
}

void SimulatorDecisionMakerThread::optimizeLogs()
{
    if (mAmountOfLogs > mLimitLogs)
    {
        QList<LogEntry> newEntries = mOptimizer->optimizeLogs(mLogsDatabase->readLogs(), mOptimizeLogsSize);
        mAmountOfLogs              = newEntries.size();

        emit logsRead(newEntries);
        mLogsDatabase->writeLogs(newEntries);
    }
}
