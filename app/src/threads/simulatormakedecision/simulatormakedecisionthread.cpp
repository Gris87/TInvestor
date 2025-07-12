#include "src/threads/simulatormakedecision/simulatormakedecisionthread.h"

#include <QDebug>



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";



SimulatorMakeDecisionThread::SimulatorMakeDecisionThread(
    IStocksStorage* stocksStorage, IDecisionMaker* decisionMaker, QObject* parent
) :
    ISimulatorMakeDecisionThread(parent),
    mStocksStorage(stocksStorage),
    mDecisionMaker(decisionMaker),
    mPortfolio(),
    mResetted(),
    mLoaded(),
    mStartMoney()
{
    qDebug() << "Create SimulatorMakeDecisionThread";
}

SimulatorMakeDecisionThread::~SimulatorMakeDecisionThread()
{
    qDebug() << "Destroy SimulatorMakeDecisionThread";
}

void SimulatorMakeDecisionThread::run()
{
    qDebug() << "Running SimulatorMakeDecisionThread";

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
    const InstrumentsForTrading& instrumentsForTrading = mDecisionMaker->makeDecision(mPortfolio, mStocksStorage->getStocks(), 0);
    mStocksStorage->readUnlock();

    if (!instrumentsForTrading.isEmpty())
    {
        simulateTrading(instrumentsForTrading);
    }

    qDebug() << "Finish SimulatorMakeDecisionThread";
}

void SimulatorMakeDecisionThread::reset()
{
    mResetted = true;
}

void SimulatorMakeDecisionThread::setStartMoney(int value)
{
    mStartMoney = value;
}

void SimulatorMakeDecisionThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}

void SimulatorMakeDecisionThread::init()
{
    initOperations();
    initLogs();
    initPortfolio();
}

void SimulatorMakeDecisionThread::initOperations()
{
    QList<Operation> operations;

    Operation operation;

    operation.timestamp                       = QDateTime::currentMSecsSinceEpoch();
    operation.instrumentId                    = RUBLE_UID;
    operation.instrumentTicker                = RUBLE_UID;         // TODO: Take from storage
    operation.instrumentName                  = "?????";           // TODO: Take from storage
    operation.description                     = "Give more money"; // TODO: Use translation
    operation.price                           = 0.0f;
    operation.avgPriceFifo                    = 0.0f;
    operation.avgPriceWavg                    = 0.0f;
    operation.quantity                        = 0;
    operation.remainedQuantity                = 0;
    operation.payment                         = mStartMoney;
    operation.avgCostFifo                     = 0.0f;
    operation.costFifo.units                  = 0;
    operation.costFifo.nano                   = 0;
    operation.costWavg.units                  = 0;
    operation.costWavg.nano                   = 0;
    operation.commission                      = 0.0f;
    operation.yield                           = 0.0f;
    operation.yieldWithCommission             = 0.0f;
    operation.yieldWithCommissionPercent      = 0.0f;
    operation.inputMoney.units                = mStartMoney;
    operation.inputMoney.nano                 = 0;
    operation.maxInputMoney.units             = mStartMoney;
    operation.maxInputMoney.nano              = 0;
    operation.totalYieldWithCommission.units  = 0;
    operation.totalYieldWithCommission.nano   = 0;
    operation.totalYieldWithCommissionPercent = 0.0f;
    operation.remainedMoney.units             = mStartMoney;
    operation.remainedMoney.nano              = 0;
    operation.totalMoney.units                = mStartMoney;
    operation.totalMoney.nano                 = 0;
    operation.pricePrecision                  = 2; // TODO: Take from storage
    operation.paymentPrecision                = 2; // TODO: Take from storage
    operation.commissionPrecision             = 2; // TODO: Take from storage

    operations.append(operation);

    emit operationsRead(operations);

    // TODO: Store operations
}

void SimulatorMakeDecisionThread::initLogs()
{
    const QList<LogEntry> entries;

    emit logsRead(entries);

    // TODO: Store logs
}

void SimulatorMakeDecisionThread::initPortfolio()
{
    // TODO: Store portfolio
}

void SimulatorMakeDecisionThread::load()
{
    loadOperations();
    loadLogs();
    loadPortfolio();
}

void SimulatorMakeDecisionThread::loadOperations()
{
}

void SimulatorMakeDecisionThread::loadLogs()
{
}

void SimulatorMakeDecisionThread::loadPortfolio()
{
}

void SimulatorMakeDecisionThread::simulateTrading(const InstrumentsForTrading& /*instrumentsForTrading*/)
{
}
