#include "src/threads/simulatordecisionmaker/simulatordecisionmakerthread.h"

#include <QDebug>



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";



SimulatorDecisionMakerThread::SimulatorDecisionMakerThread(
    IOperationsDatabase* operationsDatabase,
    ILogsDatabase*       logsDatabase,
    IInstrumentsStorage* instrumentsStorage,
    ILogosStorage*       logosStorage,
    IStocksStorage*      stocksStorage,
    IDecisionMaker*      decisionMaker,
    QObject*             parent
) :
    ISimulatorDecisionMakerThread(parent),
    mOperationsDatabase(operationsDatabase),
    mLogsDatabase(logsDatabase),
    mInstrumentsStorage(instrumentsStorage),
    mLogosStorage(logosStorage),
    mStocksStorage(stocksStorage),
    mDecisionMaker(decisionMaker),
    mPortfolio(),
    mResetted(),
    mLoaded(),
    mStartMoney()
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
    const InstrumentsForTrading& instrumentsForTrading = mDecisionMaker->makeDecision(mPortfolio, mStocksStorage->getStocks(), 0);
    mStocksStorage->readUnlock();

    if (!instrumentsForTrading.isEmpty())
    {
        simulateTrading(instrumentsForTrading);
    }

    qDebug() << "Finish SimulatorDecisionMakerThread";
}

void SimulatorDecisionMakerThread::reset()
{
    mResetted = true;
}

void SimulatorDecisionMakerThread::setStartMoney(int value)
{
    mStartMoney = value;
}

void SimulatorDecisionMakerThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}

void SimulatorDecisionMakerThread::init()
{
    initOperations();
    initLogs();
    initPortfolio();
}

void SimulatorDecisionMakerThread::initOperations()
{
    QList<Operation> operations;

    Operation operation;

    mInstrumentsStorage->readLock();
    Instrument instrument = mInstrumentsStorage->getInstruments().value(RUBLE_UID);
    mInstrumentsStorage->readUnlock();

    if (instrument.ticker == "" || instrument.name == "")
    {
        instrument.ticker         = RUBLE_UID;
        instrument.name           = "?????";
        instrument.pricePrecision = 2;
    }

    mLogosStorage->readLock();
    Logo* logo = mLogosStorage->getLogo(RUBLE_UID);
    mLogosStorage->readUnlock();

    operation.timestamp                       = QDateTime::currentMSecsSinceEpoch();
    operation.instrumentId                    = RUBLE_UID;
    operation.instrumentLogo                  = logo;
    operation.instrumentTicker                = instrument.ticker;
    operation.instrumentName                  = instrument.name;
    operation.description                     = tr("Input money");
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
    operation.pricePrecision                  = instrument.pricePrecision;
    operation.paymentPrecision                = instrument.pricePrecision;
    operation.commissionPrecision             = instrument.pricePrecision;

    operations.append(operation);

    emit operationsRead(operations);
    mOperationsDatabase->writeOperations(operations);
}

void SimulatorDecisionMakerThread::initLogs()
{
    QList<LogEntry> entries;

    emit logsRead(entries);
    mLogsDatabase->writeLogs(entries);
}

void SimulatorDecisionMakerThread::initPortfolio()
{
    Portfolio             portfolio;
    PortfolioCategoryItem category1;
    PortfolioCategoryItem category2;
    PortfolioItem         item;

    mInstrumentsStorage->readLock();
    Instrument instrument = mInstrumentsStorage->getInstruments().value(RUBLE_UID);
    mInstrumentsStorage->readUnlock();

    if (instrument.ticker == "" || instrument.name == "")
    {
        instrument.ticker         = RUBLE_UID;
        instrument.name           = "?????";
        instrument.pricePrecision = 2;
    }

    mLogosStorage->readLock();
    Logo* logo = mLogosStorage->getLogo(RUBLE_UID);
    mLogosStorage->readUnlock();

    item.instrumentId       = RUBLE_UID;
    item.instrumentLogo     = logo;
    item.instrumentTicker   = instrument.ticker;
    item.instrumentName     = instrument.name;
    item.showPrices         = false;
    item.available          = mStartMoney;
    item.price              = 1.0f;
    item.avgPriceFifo       = 1.0f;
    item.avgPriceWavg       = 1.0f;
    item.cost               = mStartMoney;
    item.part               = 100.0;
    item.yield              = 0.0f;
    item.yieldPercent       = 0.0f;
    item.dailyYield         = 0.0f;
    item.priceForDailyYield = 0.0f;
    item.costForDailyYield  = 0.0;
    item.dailyYieldPercent  = 0.0f;
    item.pricePrecision     = instrument.pricePrecision;

    category1.id   = 0;
    category1.name = tr("Currency and metals");
    category1.cost = mStartMoney;
    category1.part = 100.0;
    category1.items.append(item);

    category2.id   = 1;
    category2.name = tr("Share");
    category2.cost = 0.0;
    category2.part = 0.0;

    portfolio.positions << category1 << category2;

    emit portfolioChanged(portfolio);

    // TODO: Store portfolio
}

void SimulatorDecisionMakerThread::load()
{
    loadOperations();
    loadLogs();
    loadPortfolio();
}

void SimulatorDecisionMakerThread::loadOperations()
{
    const QList<Operation> operations = mOperationsDatabase->readOperations();
    emit operationsRead(operations);
}

void SimulatorDecisionMakerThread::loadLogs()
{
    const QList<LogEntry> entries = mLogsDatabase->readLogs();
    emit logsRead(entries);
}

void SimulatorDecisionMakerThread::loadPortfolio()
{
}

void SimulatorDecisionMakerThread::simulateTrading(const InstrumentsForTrading& /*instrumentsForTrading*/)
{
}
