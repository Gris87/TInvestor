#include "src/utils/tradesimulation/tradesimulation.h"

#include "src/domain/instrument/instrument.h"
#include "src/grpc/utils.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";

constexpr double INCREDIBLE_SELL_COEF = 3.0;
constexpr float  HUNDRED_PERCENT      = 100.0f;

constexpr int CURRENCY_ID = 0;
constexpr int SHARE_ID    = 1;



QList<Operation>
createInitOperations(IInstrumentsStorage* instrumentsStorage, ILogosStorage* logosStorage, qint64 timestamp, double startMoney)
{
    QList<Operation> res;

    instrumentsStorage->readLock();
    Instrument instrument = instrumentsStorage->getInstruments().value(RUBLE_UID);
    instrumentsStorage->readUnlock();

    instrument.resetIfNotFound(RUBLE_UID);

    logosStorage->readLock();
    Logo* logo = logosStorage->getLogo(RUBLE_UID);
    logosStorage->readUnlock();

    Operation operation;

    operation.timestamp                       = timestamp;
    operation.originalTimestamp               = timestamp;
    operation.instrumentId                    = RUBLE_UID;
    operation.instrumentLogo                  = logo;
    operation.instrumentTicker                = instrument.ticker;
    operation.instrumentName                  = instrument.name;
    operation.description                     = QObject::tr("Input money");
    operation.price                           = 0.0f;
    operation.avgPriceFifo                    = 0.0f;
    operation.avgPriceWavg                    = 0.0f;
    operation.quantity                        = 0;
    operation.remainedQuantity                = 0;
    operation.payment                         = startMoney;
    operation.avgCostFifo                     = 0.0f;
    operation.costFifo.units                  = 0;
    operation.costFifo.nano                   = 0;
    operation.costWavg.units                  = 0;
    operation.costWavg.nano                   = 0;
    operation.commission                      = 0.0f;
    operation.yield                           = 0.0f;
    operation.yieldWithCommission             = 0.0f;
    operation.yieldWithCommissionPercent      = 0.0f;
    operation.inputMoney.units                = startMoney;
    operation.inputMoney.nano                 = 0;
    operation.maxInputMoney.units             = startMoney;
    operation.maxInputMoney.nano              = 0;
    operation.totalYieldWithCommission.units  = 0;
    operation.totalYieldWithCommission.nano   = 0;
    operation.totalYieldWithCommissionPercent = 0.0f;
    operation.remainedMoney.units             = startMoney;
    operation.remainedMoney.nano              = 0;
    operation.totalMoney.units                = startMoney;
    operation.totalMoney.nano                 = 0;
    operation.pricePrecision                  = 2;
    operation.paymentPrecision                = 2;
    operation.commissionPrecision             = 2;

    res.append(operation);

    return res;
}

QList<LogEntry> createInitEntries()
{
    QList<LogEntry> res;

    return res;
}

Portfolio createInitPortfolio(IInstrumentsStorage* instrumentsStorage, ILogosStorage* logosStorage, double startMoney)
{
    Portfolio res;

    instrumentsStorage->readLock();
    Instrument instrument = instrumentsStorage->getInstruments().value(RUBLE_UID);
    instrumentsStorage->readUnlock();

    instrument.resetIfNotFound(RUBLE_UID);

    logosStorage->readLock();
    Logo* logo = logosStorage->getLogo(RUBLE_UID);
    logosStorage->readUnlock();

    PortfolioCategoryItem category1;
    PortfolioCategoryItem category2;
    PortfolioItem         item;

    item.instrumentId       = RUBLE_UID;
    item.instrumentLogo     = logo;
    item.instrumentTicker   = instrument.ticker;
    item.instrumentName     = instrument.name;
    item.showPrices         = false;
    item.available          = startMoney;
    item.price              = 1.0f;
    item.avgPriceFifo       = 1.0f;
    item.avgPriceWavg       = 1.0f;
    item.cost               = startMoney;
    item.part               = 100.0;
    item.yield              = 0.0f;
    item.yieldPercent       = 0.0f;
    item.dailyYield         = 0.0f;
    item.priceForDailyYield = 0.0f;
    item.costForDailyYield  = 0.0;
    item.dailyYieldPercent  = 0.0f;
    item.pricePrecision     = instrument.pricePrecision;

    category1.id   = 0;
    category1.name = QObject::tr("Currency and metals");
    category1.cost = startMoney;
    category1.part = 100.0;
    category1.items.append(item);

    category2.id   = 1;
    category2.name = QObject::tr("Share");
    category2.cost = 0.0;
    category2.part = 0.0;

    res.positions << category1 << category2;

    return res;
}

static void simulateSellForOperations(
    qint64&           timestamp,
    const QString&    instrumentId,
    Logo*             logo,
    const Instrument& instrument,
    qint64            quantity,
    double            costFifo,
    float             price,
    double            cost,
    double            totalCommission,
    double            startMoney,
    double&           totalMoney,
    QList<Operation>& operations,
    Portfolio&        portfolio
)
{
    const double avgPrice            = costFifo / quantity;
    const double yield               = cost - costFifo;
    const double yieldWithCommission = yield - totalCommission;

    totalMoney                            += yieldWithCommission;
    const double totalYieldWithCommission  = totalMoney - startMoney;

    Operation operation;

    operation.timestamp                       = timestamp;
    operation.originalTimestamp               = timestamp;
    operation.instrumentId                    = instrumentId;
    operation.instrumentLogo                  = logo;
    operation.instrumentTicker                = instrument.ticker;
    operation.instrumentName                  = instrument.name;
    operation.description                     = QObject::tr("Sale of shares");
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
    operation.inputMoney.units                = startMoney;
    operation.inputMoney.nano                 = 0;
    operation.maxInputMoney.units             = startMoney;
    operation.maxInputMoney.nano              = 0;
    operation.totalYieldWithCommission        = quotationFromDouble(totalYieldWithCommission);
    operation.totalYieldWithCommissionPercent = (totalYieldWithCommission / startMoney) * HUNDRED_PERCENT;
    operation.remainedMoney  = quotationFromDouble(portfolio.positions[CURRENCY_ID].items.first().cost + cost - totalCommission);
    operation.totalMoney     = quotationFromDouble(totalMoney);
    operation.pricePrecision = instrument.pricePrecision;
    operation.paymentPrecision    = instrument.pricePrecision;
    operation.commissionPrecision = instrument.pricePrecision;

    operations.append(operation);
    ++timestamp;
}

static void simulateSellForLogs(
    qint64&           timestamp,
    const QString&    instrumentId,
    Logo*             logo,
    const Instrument& instrument,
    const QString&    cause,
    qint64            quantity,
    float             price,
    QList<LogEntry>&  entries
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
    entry.message   = QObject::tr("Order to sell %1 created with a price %2")
                        .arg(QString::number(quantity), QString::number(price, 'f', instrument.pricePrecision) + " \u20BD");

    entries.append(entry);
    ++timestamp;

    entry.timestamp = timestamp;
    entry.level     = LOG_LEVEL_VERBOSE;
    entry.message   = QObject::tr("Order completed. %1 sold with a price %2")
                        .arg(QString::number(quantity), QString::number(price, 'f', instrument.pricePrecision) + " \u20BD");

    entries.append(entry);
    ++timestamp;

    entry.timestamp = timestamp;
    entry.level     = LOG_LEVEL_VERBOSE;
    entry.message   = QObject::tr("Trade completed successfully");

    entries.append(entry);
    ++timestamp;
}

static void simulateSellForPortfolio(const QString& instrumentId, double cost, double totalCommission, Portfolio& portfolio)
{
    PortfolioCategoryItem& category = portfolio.positions[SHARE_ID]; // clazy:exclude=detaching-member

    for (int i = 0; i < category.items.size(); ++i)
    {
        if (category.items.at(i).instrumentId == instrumentId)
        {
            const double costWithCommission = cost - totalCommission;

            portfolio.positions[CURRENCY_ID].items.first().available += costWithCommission;
            portfolio.positions[CURRENCY_ID].items.first().cost      += costWithCommission;
            category.items.removeAt(i);

            break;
        }
    }
}

static void simulateSellForInstruments(const QString& instrumentId, QuantityAndCostDoubleInstruments& instruments)
{
    instruments.remove(instrumentId);
}

static void simulateSellForInstrumentSells(qint64 timestamp, const QString& instrumentId, InstrumentSells& instrumentSells)
{
    instrumentSells[instrumentId] = timestamp;
}

static void simulateSell(
    qint64&                           timestamp,
    IInstrumentsStorage*              instrumentsStorage,
    ILogosStorage*                    logosStorage,
    IUserStorage*                     userStorage,
    const QString&                    instrumentId,
    const TradingInfo&                tradingInfo,
    double                            startMoney,
    double&                           totalMoney,
    QList<Operation>&                 operations,
    QList<LogEntry>&                  entries,
    Portfolio&                        portfolio,
    QuantityAndCostDoubleInstruments& instruments,
    InstrumentSells&                  instrumentSells
)
{
    if (!instruments.contains(instrumentId))
    {
        return;
    }

    instrumentsStorage->readLock();
    Instrument instrument = instrumentsStorage->getInstruments().value(instrumentId);
    instrumentsStorage->readUnlock();

    instrument.resetIfNotFound(instrumentId);

    const QuantityAndCostDouble quantityAndCost = instruments.value(instrumentId);

    userStorage->readLock();
    const float commission = userStorage->getCommission() / HUNDRED_PERCENT;
    userStorage->readUnlock();

    double cost            = quantityAndCost.quantity * tradingInfo.price;
    double totalCommission = cost * commission;

    if (cost > quantityAndCost.cost * INCREDIBLE_SELL_COEF)
    {
        qWarning() << "Incredible selling detected. Trying to negotiate it";

        cost            = quantityAndCost.cost;
        totalCommission = 0;
    }

    logosStorage->readLock();
    Logo* logo = logosStorage->getLogo(instrumentId);
    logosStorage->readUnlock();

    simulateSellForOperations(
        timestamp,
        instrumentId,
        logo,
        instrument,
        quantityAndCost.quantity,
        quantityAndCost.cost,
        tradingInfo.price,
        cost,
        totalCommission,
        startMoney,
        totalMoney,
        operations,
        portfolio
    );
    simulateSellForLogs(
        timestamp, instrumentId, logo, instrument, tradingInfo.cause, quantityAndCost.quantity, tradingInfo.price, entries
    );
    simulateSellForPortfolio(instrumentId, cost, totalCommission, portfolio);
    simulateSellForInstruments(instrumentId, instruments);
    simulateSellForInstrumentSells(timestamp, instrumentId, instrumentSells);
}

static void simulateBuyForOperations(
    qint64&           timestamp,
    const QString&    instrumentId,
    Logo*             logo,
    const Instrument& instrument,
    qint64            quantity,
    float             price,
    double            cost,
    double            totalCommission,
    double            startMoney,
    double&           totalMoney,
    QList<Operation>& operations,
    Portfolio&        portfolio
)
{
    totalMoney                            -= totalCommission;
    const double totalYieldWithCommission  = totalMoney - startMoney;

    Operation operation;

    operation.timestamp                       = timestamp;
    operation.originalTimestamp               = timestamp;
    operation.instrumentId                    = instrumentId;
    operation.instrumentLogo                  = logo;
    operation.instrumentTicker                = instrument.ticker;
    operation.instrumentName                  = instrument.name;
    operation.description                     = QObject::tr("Purchase of shares");
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
    operation.inputMoney.units                = startMoney;
    operation.inputMoney.nano                 = 0;
    operation.maxInputMoney.units             = startMoney;
    operation.maxInputMoney.nano              = 0;
    operation.totalYieldWithCommission        = quotationFromDouble(totalYieldWithCommission);
    operation.totalYieldWithCommissionPercent = (totalYieldWithCommission / startMoney) * HUNDRED_PERCENT;
    operation.remainedMoney  = quotationFromDouble(portfolio.positions[CURRENCY_ID].items.first().cost - cost - totalCommission);
    operation.totalMoney     = quotationFromDouble(totalMoney);
    operation.pricePrecision = instrument.pricePrecision;
    operation.paymentPrecision    = instrument.pricePrecision;
    operation.commissionPrecision = instrument.pricePrecision;

    operations.append(operation);
    ++timestamp;
}

static void simulateBuyForLogs(
    qint64&           timestamp,
    const QString&    instrumentId,
    Logo*             logo,
    const Instrument& instrument,
    const QString&    cause,
    qint64            quantity,
    float             price,
    QList<LogEntry>&  entries
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
    entry.message   = QObject::tr("Order to buy %1 created with a price %2")
                        .arg(QString::number(quantity), QString::number(price, 'f', instrument.pricePrecision) + " \u20BD");

    entries.append(entry);
    ++timestamp;

    entry.timestamp = timestamp;
    entry.level     = LOG_LEVEL_VERBOSE;
    entry.message   = QObject::tr("Order completed. %1 bought with a price %2")
                        .arg(QString::number(quantity), QString::number(price, 'f', instrument.pricePrecision) + " \u20BD");

    entries.append(entry);
    ++timestamp;

    entry.timestamp = timestamp;
    entry.level     = LOG_LEVEL_VERBOSE;
    entry.message   = QObject::tr("Trade completed successfully");

    entries.append(entry);
    ++timestamp;
}

static void simulateBuyForPortfolio(
    const QString&    instrumentId,
    Logo*             logo,
    const Instrument& instrument,
    qint64            quantity,
    float             price,
    double            cost,
    double            totalCommission,
    Portfolio&        portfolio
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

    portfolio.positions[CURRENCY_ID].items.first().available -= costWithCommission;
    portfolio.positions[CURRENCY_ID].items.first().cost      -= costWithCommission;
    portfolio.positions[SHARE_ID].items.append(item);
}

static void simulateBuyForInstruments(
    const QString& instrumentId, qint64 quantity, double cost, QuantityAndCostDoubleInstruments& instruments
)
{
    QuantityAndCostDouble quantityAndCost;

    quantityAndCost.quantity = quantity;
    quantityAndCost.cost     = cost;

    instruments[instrumentId] = quantityAndCost;
}

static void simulateBuy(
    qint64&                           timestamp,
    IInstrumentsStorage*              instrumentsStorage,
    ILogosStorage*                    logosStorage,
    IUserStorage*                     userStorage,
    const QString&                    instrumentId,
    const TradingInfo&                tradingInfo,
    double                            startMoney,
    double&                           totalMoney,
    QList<Operation>&                 operations,
    QList<LogEntry>&                  entries,
    Portfolio&                        portfolio,
    QuantityAndCostDoubleInstruments& instruments
)
{
    if (instruments.contains(instrumentId))
    {
        return;
    }

    instrumentsStorage->readLock();
    Instrument instrument = instrumentsStorage->getInstruments().value(instrumentId);
    instrumentsStorage->readUnlock();

    instrument.resetIfNotFound(instrumentId);

    userStorage->readLock();
    const float commission = userStorage->getCommission() / HUNDRED_PERCENT;
    userStorage->readUnlock();

    const double lotPrice               = instrument.lot * tradingInfo.price;
    const double lotPriceWithCommission = lotPrice * (1 + commission);

    const qint64 amountOfLots = qMin(
        qRound64(tradingInfo.expectedCost / lotPrice),
        static_cast<qint64>(portfolio.positions.at(CURRENCY_ID).items.constFirst().cost / lotPriceWithCommission)
    );

    if (amountOfLots > 0)
    {
        const qint64 quantity        = amountOfLots * instrument.lot;
        const double cost            = quantity * tradingInfo.price;
        const double totalCommission = cost * commission;

        logosStorage->readLock();
        Logo* logo = logosStorage->getLogo(instrumentId);
        logosStorage->readUnlock();

        simulateBuyForOperations(
            timestamp,
            instrumentId,
            logo,
            instrument,
            quantity,
            tradingInfo.price,
            cost,
            totalCommission,
            startMoney,
            totalMoney,
            operations,
            portfolio
        );
        simulateBuyForLogs(timestamp, instrumentId, logo, instrument, tradingInfo.cause, quantity, tradingInfo.price, entries);
        simulateBuyForPortfolio(instrumentId, logo, instrument, quantity, tradingInfo.price, cost, totalCommission, portfolio);
        simulateBuyForInstruments(instrumentId, quantity, cost, instruments);
    }
}

void simulateTrading(
    qint64                            timestamp,
    IInstrumentsStorage*              instrumentsStorage,
    ILogosStorage*                    logosStorage,
    IUserStorage*                     userStorage,
    const InstrumentsForTrading&      instrumentsForTrading,
    double                            startMoney,
    double&                           totalMoney,
    QList<Operation>&                 operations,
    QList<LogEntry>&                  entries,
    Portfolio&                        portfolio,
    QuantityAndCostDoubleInstruments& instruments,
    InstrumentSells&                  instrumentSells
)
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

    for (auto it = instrumentsForSell.constBegin(); it != instrumentsForSell.constEnd(); ++it)
    {
        simulateSell(
            timestamp,
            instrumentsStorage,
            logosStorage,
            userStorage,
            it.key(),
            it.value(),
            startMoney,
            totalMoney,
            operations,
            entries,
            portfolio,
            instruments,
            instrumentSells
        );
    }

    for (auto it = instrumentsForBuy.constBegin(); it != instrumentsForBuy.constEnd(); ++it)
    {
        simulateBuy(
            timestamp,
            instrumentsStorage,
            logosStorage,
            userStorage,
            it.key(),
            it.value(),
            startMoney,
            totalMoney,
            operations,
            entries,
            portfolio,
            instruments
        );
    }
}
