#include "src/threads/simulatordecisionmaker/simulatordecisionmakerthread.h"

#include <QDebug>

#include "src/grpc/utils.h"
#include "src/threads/parallelhelper/parallelhelperthread.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";

constexpr int   LIMIT_OPERATIONS         = 100000;
constexpr int   OPTIMIZE_OPERATIONS_SIZE = 10000;
constexpr int   LIMIT_LOGS               = 1000000;
constexpr int   OPTIMIZE_LOGS_SIZE       = 10000;
constexpr float HUNDRED_PERCENT          = 100.0f;

constexpr int CURRENCY_ID = 0;
constexpr int SHARE_ID    = 1;



SimulatorDecisionMakerThread::SimulatorDecisionMakerThread(
    ISettingsEditor*     settingsEditor,
    IOperationsDatabase* operationsDatabase,
    ILogsDatabase*       logsDatabase,
    IPortfolioDatabase*  portfolioDatabase,
    IInstrumentsStorage* instrumentsStorage,
    ILogosStorage*       logosStorage,
    IUserStorage*        userStorage,
    IStocksStorage*      stocksStorage,
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
    const InstrumentsForTrading& instrumentsForTrading =
        mDecisionMaker->makeDecision(QDateTime::currentMSecsSinceEpoch(), mPortfolio, mStocksStorage->getStocks(), 0, false);
    mStocksStorage->readUnlock();

    if (!instrumentsForTrading.isEmpty())
    {
        simulateTrading(QDateTime::currentMSecsSinceEpoch(), instrumentsForTrading);
    }

    optimizeOperations();
    optimizeLogs();

    qDebug() << "Finish SimulatorDecisionMakerThread";
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
    mInstrumentsStorage->readLock();
    Instrument instrument = mInstrumentsStorage->getInstruments().value(RUBLE_UID);
    mInstrumentsStorage->readUnlock();

    instrument.resetIfNotFound(RUBLE_UID);

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
    mAmountOfOperations = operations.size();

    mTotalMoney = mStartMoney;
}

void SimulatorDecisionMakerThread::initLogs()
{
    QList<LogEntry> entries;

    emit logsRead(entries);
    mLogsDatabase->writeLogs(entries);
    mAmountOfLogs = entries.size();
}

void SimulatorDecisionMakerThread::initPortfolio()
{
    mInstrumentsStorage->readLock();
    Instrument instrument = mInstrumentsStorage->getInstruments().value(RUBLE_UID);
    mInstrumentsStorage->readUnlock();

    instrument.resetIfNotFound(RUBLE_UID);

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

    mPortfolio.positions.clear();
    mPortfolio.positions << category1 << category2;

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

    if (mAmountOfOperations > 0)
    {
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
            QuantityAndCostDouble quantityAndCost;

            quantityAndCost.quantity = item.available;
            quantityAndCost.cost     = item.cost;

            mInstruments[item.instrumentId] = quantityAndCost;
        }
    }
}

void SimulatorDecisionMakerThread::simulateTrading(qint64 timestamp, const InstrumentsForTrading& instrumentsForTrading)
{
    InstrumentsForTrading instrumentsForSell;
    InstrumentsForTrading instrumentsForBuy;

    for (auto it = instrumentsForTrading.constBegin(); it != instrumentsForTrading.constEnd(); ++it)
    {
        if (it.value().expectedCost > 0)
        {
            instrumentsForBuy[it.key()] = it.value();
        }
        else
        {
            instrumentsForSell[it.key()] = it.value();
        }
    }

    QList<Operation> operations;
    QList<LogEntry>  entries;

    for (auto it = instrumentsForSell.constBegin(); it != instrumentsForSell.constEnd(); ++it)
    {
        simulateSell(timestamp, it.key(), it.value(), operations, entries);
    }

    for (auto it = instrumentsForBuy.constBegin(); it != instrumentsForBuy.constEnd(); ++it)
    {
        simulateBuy(timestamp, it.key(), it.value(), operations, entries);
    }

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

void SimulatorDecisionMakerThread::simulateSell(
    qint64&            timestamp,
    const QString&     instrumentId,
    const TradingInfo& tradingInfo,
    QList<Operation>&  operations,
    QList<LogEntry>&   entries
)
{
    if (!mInstruments.contains(instrumentId))
    {
        return;
    }

    mInstrumentsStorage->readLock();
    Instrument instrument = mInstrumentsStorage->getInstruments().value(instrumentId);
    mInstrumentsStorage->readUnlock();

    instrument.resetIfNotFound(instrumentId);

    const QuantityAndCostDouble quantityAndCost = mInstruments.value(instrumentId);

    mUserStorage->readLock();
    const float commission = mUserStorage->getCommission() / HUNDRED_PERCENT;
    mUserStorage->readUnlock();

    const double cost            = quantityAndCost.quantity * tradingInfo.price;
    const double totalCommission = cost * commission;

    mLogosStorage->readLock();
    Logo* logo = mLogosStorage->getLogo(instrumentId);
    mLogosStorage->readUnlock();

    simulateSellForOperations(
        operations,
        timestamp,
        instrumentId,
        logo,
        instrument,
        quantityAndCost.quantity,
        quantityAndCost.cost,
        tradingInfo.price,
        cost,
        totalCommission
    );
    simulateSellForLogs(
        entries, timestamp, instrumentId, logo, instrument, tradingInfo.cause, quantityAndCost.quantity, tradingInfo.price
    );
    simulateSellForPortfolio(instrumentId, cost, totalCommission);
    simulateSellForInstruments(instrumentId);
}

void SimulatorDecisionMakerThread::simulateSellForOperations(
    QList<Operation>& operations,
    qint64&           timestamp,
    const QString&    instrumentId,
    Logo*             logo,
    const Instrument& instrument,
    qint64            quantity,
    double            costFifo,
    float             price,
    double            cost,
    double            totalCommission
)
{
    const double avgPrice            = costFifo / quantity;
    const double yield               = cost - costFifo;
    const double yieldWithCommission = yield - totalCommission;

    mTotalMoney                           += yieldWithCommission;
    const double totalYieldWithCommission  = mTotalMoney - mStartMoney;

    Operation operation;

    operation.timestamp                       = timestamp;
    operation.instrumentId                    = instrumentId;
    operation.instrumentLogo                  = logo;
    operation.instrumentTicker                = instrument.ticker;
    operation.instrumentName                  = instrument.name;
    operation.description                     = tr("Sale of shares");
    operation.price                           = price;
    operation.avgPriceFifo                    = avgPrice;
    operation.avgPriceWavg                    = avgPrice;
    operation.quantity                        = quantity;
    operation.remainedQuantity                = 0;
    operation.payment                         = cost;
    operation.avgCostFifo                     = costFifo;
    operation.costFifo.units                  = 0;
    operation.costFifo.nano                   = 0;
    operation.costWavg.units                  = 0;
    operation.costWavg.nano                   = 0;
    operation.commission                      = -totalCommission;
    operation.yield                           = yield;
    operation.yieldWithCommission             = yieldWithCommission;
    operation.yieldWithCommissionPercent      = (yieldWithCommission / costFifo) * HUNDRED_PERCENT;
    operation.inputMoney.units                = mStartMoney;
    operation.inputMoney.nano                 = 0;
    operation.maxInputMoney.units             = mStartMoney;
    operation.maxInputMoney.nano              = 0;
    operation.totalYieldWithCommission        = quotationFromDouble(totalYieldWithCommission);
    operation.totalYieldWithCommissionPercent = (totalYieldWithCommission / mStartMoney) * HUNDRED_PERCENT;
    operation.remainedMoney  = quotationFromDouble(mPortfolio.positions[CURRENCY_ID].items.first().cost + cost - totalCommission);
    operation.totalMoney     = quotationFromDouble(mTotalMoney);
    operation.pricePrecision = instrument.pricePrecision;
    operation.paymentPrecision    = instrument.pricePrecision;
    operation.commissionPrecision = instrument.pricePrecision;

    operations.append(operation);
    ++timestamp;
}

void SimulatorDecisionMakerThread::simulateSellForLogs(
    QList<LogEntry>&  entries,
    qint64&           timestamp,
    const QString&    instrumentId,
    Logo*             logo,
    const Instrument& instrument,
    const QString&    cause,
    qint64            quantity,
    float             price
)
{
    LogEntry entry;

    entry.timestamp        = timestamp;
    entry.level            = LOG_LEVEL_DEBUG;
    entry.instrumentId     = instrumentId;
    entry.instrumentLogo   = logo;
    entry.instrumentTicker = instrument.ticker;
    entry.instrumentName   = instrument.name;
    entry.message          = cause;

    entries.append(entry);
    ++timestamp;

    entry.timestamp = timestamp;
    entry.level     = LOG_LEVEL_VERBOSE;
    entry.message   = tr("Order to sell %1 created with a price %2 %3")
                        .arg(QString::number(quantity), QString::number(price, 'f', instrument.pricePrecision), "\u20BD");

    entries.append(entry);
    ++timestamp;

    entry.timestamp = timestamp;
    entry.level     = LOG_LEVEL_VERBOSE;
    entry.message   = tr("Order completed. %1 sold with a price %2 %3")
                        .arg(QString::number(quantity), QString::number(price, 'f', instrument.pricePrecision), "\u20BD");

    entries.append(entry);
    ++timestamp;

    entry.timestamp = timestamp;
    entry.level     = LOG_LEVEL_VERBOSE;
    entry.message   = tr("Trade completed successfully");

    entries.append(entry);
    ++timestamp;
}

void SimulatorDecisionMakerThread::simulateSellForPortfolio(const QString& instrumentId, double cost, double totalCommission)
{
    PortfolioCategoryItem& category = mPortfolio.positions[SHARE_ID]; // clazy:exclude=detaching-member

    for (int i = 0; i < category.items.size(); ++i)
    {
        if (category.items.at(i).instrumentId == instrumentId)
        {
            const double costWithCommission = cost - totalCommission;

            mPortfolio.positions[CURRENCY_ID].items.first().available += costWithCommission;
            mPortfolio.positions[CURRENCY_ID].items.first().cost      += costWithCommission;
            category.items.removeAt(i);

            break;
        }
    }
}

void SimulatorDecisionMakerThread::simulateSellForInstruments(const QString& instrumentId)
{
    mInstruments.remove(instrumentId);
}

void SimulatorDecisionMakerThread::simulateBuy(
    qint64&            timestamp,
    const QString&     instrumentId,
    const TradingInfo& tradingInfo,
    QList<Operation>&  operations,
    QList<LogEntry>&   entries
)
{
    if (mInstruments.contains(instrumentId))
    {
        return;
    }

    mInstrumentsStorage->readLock();
    Instrument instrument = mInstrumentsStorage->getInstruments().value(instrumentId);
    mInstrumentsStorage->readUnlock();

    instrument.resetIfNotFound(instrumentId);

    mUserStorage->readLock();
    const float commission = mUserStorage->getCommission() / HUNDRED_PERCENT;
    mUserStorage->readUnlock();

    const double lotPrice               = instrument.lot * tradingInfo.price;
    const double lotPriceWithCommission = lotPrice * (1 + commission);

    const qint64 amountOfLots = qMin(
        qRound64(tradingInfo.expectedCost / lotPrice),
        static_cast<qint64>(mPortfolio.positions.at(CURRENCY_ID).items.constFirst().cost / lotPriceWithCommission)
    );

    if (amountOfLots > 0)
    {
        const qint64 quantity        = amountOfLots * instrument.lot;
        const double cost            = quantity * tradingInfo.price;
        const double totalCommission = cost * commission;

        mLogosStorage->readLock();
        Logo* logo = mLogosStorage->getLogo(instrumentId);
        mLogosStorage->readUnlock();

        simulateBuyForOperations(
            operations, timestamp, instrumentId, logo, instrument, quantity, tradingInfo.price, cost, totalCommission
        );
        simulateBuyForLogs(entries, timestamp, instrumentId, logo, instrument, tradingInfo.cause, quantity, tradingInfo.price);
        simulateBuyForPortfolio(instrumentId, logo, instrument, quantity, tradingInfo.price, cost, totalCommission);
        simulateBuyForInstruments(instrumentId, quantity, cost);
    }
}

void SimulatorDecisionMakerThread::simulateBuyForOperations(
    QList<Operation>& operations,
    qint64&           timestamp,
    const QString&    instrumentId,
    Logo*             logo,
    const Instrument& instrument,
    qint64            quantity,
    float             price,
    double            cost,
    double            totalCommission
)
{
    mTotalMoney                           -= totalCommission;
    const double totalYieldWithCommission  = mTotalMoney - mStartMoney;

    Operation operation;

    operation.timestamp                       = timestamp;
    operation.instrumentId                    = instrumentId;
    operation.instrumentLogo                  = logo;
    operation.instrumentTicker                = instrument.ticker;
    operation.instrumentName                  = instrument.name;
    operation.description                     = tr("Purchase of shares");
    operation.price                           = price;
    operation.avgPriceFifo                    = price;
    operation.avgPriceWavg                    = price;
    operation.quantity                        = quantity;
    operation.remainedQuantity                = quantity;
    operation.payment                         = -cost;
    operation.avgCostFifo                     = cost;
    operation.costFifo                        = quotationFromDouble(cost);
    operation.costWavg                        = quotationFromDouble(cost);
    operation.commission                      = -totalCommission;
    operation.yield                           = 0.0f;
    operation.yieldWithCommission             = -totalCommission;
    operation.yieldWithCommissionPercent      = -(totalCommission / cost) * HUNDRED_PERCENT;
    operation.inputMoney.units                = mStartMoney;
    operation.inputMoney.nano                 = 0;
    operation.maxInputMoney.units             = mStartMoney;
    operation.maxInputMoney.nano              = 0;
    operation.totalYieldWithCommission        = quotationFromDouble(totalYieldWithCommission);
    operation.totalYieldWithCommissionPercent = (totalYieldWithCommission / mStartMoney) * HUNDRED_PERCENT;
    operation.remainedMoney  = quotationFromDouble(mPortfolio.positions[CURRENCY_ID].items.first().cost - cost - totalCommission);
    operation.totalMoney     = quotationFromDouble(mTotalMoney);
    operation.pricePrecision = instrument.pricePrecision;
    operation.paymentPrecision    = instrument.pricePrecision;
    operation.commissionPrecision = instrument.pricePrecision;

    operations.append(operation);
    ++timestamp;
}

void SimulatorDecisionMakerThread::simulateBuyForLogs(
    QList<LogEntry>&  entries,
    qint64&           timestamp,
    const QString&    instrumentId,
    Logo*             logo,
    const Instrument& instrument,
    const QString&    cause,
    qint64            quantity,
    float             price
)
{
    LogEntry entry;

    entry.timestamp        = timestamp;
    entry.level            = LOG_LEVEL_DEBUG;
    entry.instrumentId     = instrumentId;
    entry.instrumentLogo   = logo;
    entry.instrumentTicker = instrument.ticker;
    entry.instrumentName   = instrument.name;
    entry.message          = cause;

    entries.append(entry);
    ++timestamp;

    entry.timestamp = timestamp;
    entry.level     = LOG_LEVEL_VERBOSE;
    entry.message   = tr("Order to buy %1 created with a price %2 %3")
                        .arg(QString::number(quantity), QString::number(price, 'f', instrument.pricePrecision), "\u20BD");

    entries.append(entry);
    ++timestamp;

    entry.timestamp = timestamp;
    entry.level     = LOG_LEVEL_VERBOSE;
    entry.message   = tr("Order completed. %1 bought with a price %2 %3")
                        .arg(QString::number(quantity), QString::number(price, 'f', instrument.pricePrecision), "\u20BD");

    entries.append(entry);
    ++timestamp;

    entry.timestamp = timestamp;
    entry.level     = LOG_LEVEL_VERBOSE;
    entry.message   = tr("Trade completed successfully");

    entries.append(entry);
    ++timestamp;
}

void SimulatorDecisionMakerThread::simulateBuyForPortfolio(
    const QString&    instrumentId,
    Logo*             logo,
    const Instrument& instrument,
    qint64            quantity,
    float             price,
    double            cost,
    double            totalCommission
)
{
    PortfolioItem item;

    item.instrumentId       = instrumentId;
    item.instrumentLogo     = logo;
    item.instrumentTicker   = instrument.ticker;
    item.instrumentName     = instrument.name;
    item.showPrices         = true;
    item.available          = quantity;
    item.price              = price;
    item.avgPriceFifo       = price;
    item.avgPriceWavg       = price;
    item.cost               = cost;
    item.part               = 0.0;
    item.yield              = 0.0f;
    item.yieldPercent       = 0.0f;
    item.dailyYield         = 0.0f;
    item.priceForDailyYield = price;
    item.costForDailyYield  = cost;
    item.dailyYieldPercent  = 0.0f;
    item.pricePrecision     = instrument.pricePrecision;

    const double costWithCommission = cost + totalCommission;

    mPortfolio.positions[CURRENCY_ID].items.first().available -= costWithCommission;
    mPortfolio.positions[CURRENCY_ID].items.first().cost      -= costWithCommission;
    mPortfolio.positions[SHARE_ID].items.append(item);
}

void SimulatorDecisionMakerThread::simulateBuyForInstruments(const QString& instrumentId, qint64 quantity, double cost)
{
    QuantityAndCostDouble quantityAndCost;

    quantityAndCost.quantity = quantity;
    quantityAndCost.cost     = cost;

    mInstruments[instrumentId] = quantityAndCost;
}

struct ReverseOperationsInfo
{
    explicit ReverseOperationsInfo(QList<Operation>* _operations) :
        operations(_operations)
    {
    }

    QList<Operation>* operations;
};

static void reverseOperationsForParallel(
    QThread* parentThread, int /*threadId*/, QList<Operation>& res, int start, int end, void* additionalArgs
)
{
    ReverseOperationsInfo* reverseOperationsInfo = reinterpret_cast<ReverseOperationsInfo*>(additionalArgs);

    Operation* operationsArray = reverseOperationsInfo->operations->data();

    Operation* resArray = res.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        resArray[i] = operationsArray[res.size() - i - 1];
    }
}

QList<Operation> SimulatorDecisionMakerThread::reverseOperations(QList<Operation>& operations)
{
    QList<Operation> res;
    res.resizeForOverwrite(operations.size());

    ReverseOperationsInfo reverseOperationsInfo(&operations);
    processInParallel(res, reverseOperationsForParallel, &reverseOperationsInfo);

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
                if (s->meta.instrumentId == item.instrumentId)
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
            item.yieldPercent      = (item.yield / item.cost) * HUNDRED_PERCENT;
            item.dailyYield        = currentCost - item.costForDailyYield;
            item.dailyYieldPercent = (item.dailyYield / item.costForDailyYield) * HUNDRED_PERCENT;
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
