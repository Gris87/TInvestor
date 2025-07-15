#include "src/threads/simulatordecisionmaker/simulatordecisionmakerthread.h"

#include <QDebug>



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";
constexpr float   HUNDRED_PERCENT = 100.0f;

constexpr int CURRENCY_ID = 0;
constexpr int SHARE_ID    = 1;



SimulatorDecisionMakerThread::SimulatorDecisionMakerThread(
    IOperationsDatabase* operationsDatabase,
    ILogsDatabase*       logsDatabase,
    IPortfolioDatabase*  portfolioDatabase,
    IInstrumentsStorage* instrumentsStorage,
    ILogosStorage*       logosStorage,
    IUserStorage*        userStorage,
    IStocksStorage*      stocksStorage,
    IDecisionMaker*      decisionMaker,
    QObject*             parent
) :
    ISimulatorDecisionMakerThread(parent),
    mOperationsDatabase(operationsDatabase),
    mLogsDatabase(logsDatabase),
    mPortfolioDatabase(portfolioDatabase),
    mInstrumentsStorage(instrumentsStorage),
    mLogosStorage(logosStorage),
    mUserStorage(userStorage),
    mStocksStorage(stocksStorage),
    mDecisionMaker(decisionMaker),
    mPortfolio(),
    mInstruments(),
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
    mInstrumentsStorage->readLock();
    Instrument instrument = mInstrumentsStorage->getInstruments().value(RUBLE_UID);
    mInstrumentsStorage->readUnlock();

    if (instrument.ticker == "" || instrument.name == "")
    {
        instrument.ticker         = RUBLE_UID;
        instrument.name           = "?????";
        instrument.lot            = 1;
        instrument.pricePrecision = 2;
    }

    mLogosStorage->readLock();
    Logo* logo = mLogosStorage->getLogo(RUBLE_UID);
    mLogosStorage->readUnlock();

    QList<Operation> operations;

    Operation operation;

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
    mInstrumentsStorage->readLock();
    Instrument instrument = mInstrumentsStorage->getInstruments().value(RUBLE_UID);
    mInstrumentsStorage->readUnlock();

    if (instrument.ticker == "" || instrument.name == "")
    {
        instrument.ticker         = RUBLE_UID;
        instrument.name           = "?????";
        instrument.lot            = 1;
        instrument.pricePrecision = 2;
    }

    mLogosStorage->readLock();
    Logo* logo = mLogosStorage->getLogo(RUBLE_UID);
    mLogosStorage->readUnlock();

    PortfolioCategoryItem category1;
    PortfolioCategoryItem category2;
    PortfolioItem         item;

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

    mPortfolio.positions << category1 << category2;

    emit portfolioChanged(mPortfolio);
    mPortfolioDatabase->writePortfolio(mPortfolio);

    mInstruments.clear();
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
    mPortfolio = mPortfolioDatabase->readPortfolio();
    emit portfolioChanged(mPortfolio);

    mInstruments.clear();

    for (const PortfolioCategoryItem& category : mPortfolio.positions)
    {
        for (const PortfolioItem& item : category.items)
        {
            QuantityAndCostDouble quantityAndCost;

            quantityAndCost.quantity = item.available;
            quantityAndCost.cost     = item.cost;

            mInstruments[item.instrumentId] = quantityAndCost;
        }
    }
}

void SimulatorDecisionMakerThread::simulateTrading(const InstrumentsForTrading& instrumentsForTrading)
{
    QStringList           instrumentsForSell;
    InstrumentsForTrading instrumentsForBuy;

    for (auto it = instrumentsForTrading.constBegin(); it != instrumentsForTrading.constEnd(); ++it)
    {
        if (it.value().expectedCost > 0)
        {
            instrumentsForBuy[it.key()] = it.value();
        }
        else
        {
            instrumentsForSell.append(it.key());
        }
    }

    for (const QString& instrumentId : instrumentsForSell)
    {
        simulateSell(instrumentId);
    }

    for (auto it = instrumentsForBuy.constBegin(); it != instrumentsForBuy.constEnd(); ++it)
    {
        simulateBuy(it.key(), it.value());
    }

    updateCostAndPart();

    emit portfolioChanged(mPortfolio);
    mPortfolioDatabase->writePortfolio(mPortfolio);
}

void SimulatorDecisionMakerThread::simulateSell(const QString& instrumentId)
{
    if (!mInstruments.contains(instrumentId))
    {
        return;
    }
}

void SimulatorDecisionMakerThread::simulateBuy(const QString& instrumentId, const TradingInfo& tradingInfo)
{
    if (mInstruments.contains(instrumentId))
    {
        return;
    }

    mInstrumentsStorage->readLock();
    Instrument instrument = mInstrumentsStorage->getInstruments().value(instrumentId);
    mInstrumentsStorage->readUnlock();

    if (instrument.ticker == "" || instrument.name == "")
    {
        instrument.ticker         = instrumentId;
        instrument.name           = "?????";
        instrument.lot            = 1;
        instrument.pricePrecision = 2;
    }

    const float commission = mUserStorage->getCommission();

    const double lotPrice               = instrument.lot * tradingInfo.price;
    const double lotPriceWithCommission = lotPrice * (1 + commission / 100);

    const qint64 amountOfLots = qMin(
        tradingInfo.expectedCost / lotPrice, mPortfolio.positions.at(CURRENCY_ID).items.constFirst().cost / lotPriceWithCommission
    );

    if (amountOfLots > 0)
    {
        const double cost            = amountOfLots * lotPrice;
        const double totalCommission = cost * commission;

        mLogosStorage->readLock();
        Logo* logo = mLogosStorage->getLogo(instrumentId);
        mLogosStorage->readUnlock();

        QuantityAndCostDouble quantityAndCost;

        quantityAndCost.quantity = amountOfLots;
        quantityAndCost.cost     = cost;

        mInstruments[instrumentId] = quantityAndCost;

        PortfolioItem item;

        item.instrumentId       = instrumentId;
        item.instrumentLogo     = logo;
        item.instrumentTicker   = instrument.ticker;
        item.instrumentName     = instrument.name;
        item.showPrices         = false;
        item.available          = amountOfLots;
        item.price              = tradingInfo.price;
        item.avgPriceFifo       = tradingInfo.price;
        item.avgPriceWavg       = tradingInfo.price;
        item.cost               = cost;
        item.part               = 0.0;
        item.yield              = 0.0f;
        item.yieldPercent       = 0.0f;
        item.dailyYield         = 0.0f;
        item.priceForDailyYield = tradingInfo.price;
        item.costForDailyYield  = cost;
        item.dailyYieldPercent  = 0.0f;
        item.pricePrecision     = instrument.pricePrecision;

        mPortfolio.positions[CURRENCY_ID].items.first().cost -= cost + totalCommission;
        mPortfolio.positions[SHARE_ID].items.append(item);
    }
}

void SimulatorDecisionMakerThread::updateCostAndPart()
{
    double totalCost = 0.0;

    for (const PortfolioCategoryItem& category : mPortfolio.positions)
    {
        for (const PortfolioItem& item : category.items)
        {
            totalCost += item.cost;
        }
    }

    for (PortfolioCategoryItem& category : mPortfolio.positions)
    {
        category.cost = 0;

        for (PortfolioItem& item : category.items)
        {
            item.part = (item.cost / totalCost) * HUNDRED_PERCENT;

            category.cost += item.cost;
        }

        category.part = (category.cost / totalCost) * HUNDRED_PERCENT;
    }
}
