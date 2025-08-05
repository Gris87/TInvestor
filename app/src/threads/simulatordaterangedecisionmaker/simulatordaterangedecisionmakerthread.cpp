#include "src/threads/simulatordaterangedecisionmaker/simulatordaterangedecisionmakerthread.h"

#include <QCoreApplication>
#include <QDebug>
#include <QMutex>

#include "src/grpc/utils.h"
#include "src/threads/parallelhelper/parallelhelperthread.h"



const char* const DATE_FORMAT  = "yyyy-MM-dd";
const char* const RUBLE_UID    = "a92e2e25-a698-45cc-a781-167cf465257c";
const QColor      GREEN_COLOR  = QColor("#2BD793"); // clazy:exclude=non-pod-global-static
const QColor      RED_COLOR    = QColor("#ED6F7E"); // clazy:exclude=non-pod-global-static
const QColor      NORMAL_COLOR = QColor("#97AEC4"); // clazy:exclude=non-pod-global-static

constexpr int    LIMIT_OPERATIONS         = 100000;
constexpr int    OPTIMIZE_OPERATIONS_SIZE = 10000;
constexpr int    LIMIT_LOGS               = 1000000;
constexpr int    OPTIMIZE_LOGS_SIZE       = 10000;
constexpr float  ZERO_LIMIT               = 0.0001f;
constexpr float  HUNDRED_PERCENT          = 100.0f;
constexpr qint64 MS_IN_SECOND             = 1000LL;
constexpr qint64 ONE_MINUTE               = 60LL * MS_IN_SECOND;
constexpr qint64 ONE_HOUR                 = 60LL * ONE_MINUTE;
constexpr qint64 NOTIFY_PROGRESS_STEP     = 1LL * ONE_HOUR;
const int        SECONDS_IN_MINUTE        = 60;
const int        MINUTES_IN_HOUR          = 60;

constexpr int CURRENCY_ID = 0;
constexpr int SHARE_ID    = 1;



SimulatorDateRangeDecisionMakerThread::SimulatorDateRangeDecisionMakerThread(
    IDirFactory*         dirFactory,
    IFileFactory*        fileFactory,
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
    ISimulatorDateRangeDecisionMakerThread(parent),
    mDirFactory(dirFactory),
    mFileFactory(fileFactory),
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
    mInitOperations(),
    mInitEntries(),
    mInitPortfolio(),
    mBestOperations(),
    mBestEntries(),
    mBestPortfolio(),
    mLimitOperations(LIMIT_OPERATIONS),
    mOptimizeOperationsSize(OPTIMIZE_OPERATIONS_SIZE),
    mLimitLogs(LIMIT_LOGS),
    mOptimizeLogsSize(OPTIMIZE_LOGS_SIZE),
    mResetted(),
    mStartMoney(),
    mStartTimestamp(),
    mEndTimestamp(),
    mBestConfig(),
    mConfigVariants()
{
    qDebug() << "Create SimulatorDateRangeDecisionMakerThread";
}

SimulatorDateRangeDecisionMakerThread::~SimulatorDateRangeDecisionMakerThread()
{
    qDebug() << "Destroy SimulatorDateRangeDecisionMakerThread";
}

void SimulatorDateRangeDecisionMakerThread::run()
{
    qDebug() << "Running SimulatorDateRangeDecisionMakerThread";

    blockSignals(false);

    if (mResetted)
    {
        init();
        mResetted = false;
    }
    else
    {
        load();
    }

    const qint64 startTime = QDateTime::currentMSecsSinceEpoch();

    if (mBestConfig)
    {
        simulationWithBestConfig(startTime);
    }
    else
    {
        simulationWithoutBestConfig(startTime);
    }

    qInfo() << "Simulation completed in" << QDateTime::currentMSecsSinceEpoch() - startTime << "ms";

    if (!mBestPortfolio.positions.isEmpty())
    {
        if (mBestConfig)
        {
            emit bestConfigFound();
        }

        optimizeOperations();
        optimizeLogs();
        updateCostAndPart();
        updatePrice();

        emit operationsRead(mBestOperations);
        emit logsRead(mBestEntries);
        emit portfolioChanged(mBestPortfolio);
    }

    qDebug() << "Finish SimulatorDateRangeDecisionMakerThread";
}

void SimulatorDateRangeDecisionMakerThread::reset()
{
    mResetted = true;
}

void SimulatorDateRangeDecisionMakerThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}

void SimulatorDateRangeDecisionMakerThread::init()
{
    readSimulationConfig();
    initOperations();
    initLogs();
    initPortfolio();
    initConfigs();

    mOperationsDatabase->deleteOperations();
    mLogsDatabase->deleteLogs();
    mPortfolioDatabase->deletePortfolio();

    for (int i = 0; i < mConfigVariants.size(); ++i)
    {
        mOperationsDatabase->deleteOperations(i);
        mLogsDatabase->deleteLogs(i);
        mPortfolioDatabase->deletePortfolio(i);
    }
}

void SimulatorDateRangeDecisionMakerThread::readSimulationConfig()
{
    mStartMoney = mSettingsEditor->value("Options/StartMoney", 0).toInt();
    mStartTimestamp =
        QDateTime(QDate::fromString(mSettingsEditor->value("Options/FromDate", "").toString(), DATE_FORMAT), QTime(0, 0))
            .toMSecsSinceEpoch();
    mEndTimestamp =
        QDateTime(QDate::fromString(mSettingsEditor->value("Options/ToDate", "").toString(), DATE_FORMAT), QTime(0, 0))
            .toMSecsSinceEpoch();
    mBestConfig = mSettingsEditor->value("Options/BestConfig", false).toBool();

    if (mBestConfig)
    {
        mConfig->setSimulatorConfigCommon(true);
        mConfig->setAutoPilotConfigCommon(false);
    }
}

void SimulatorDateRangeDecisionMakerThread::initOperations()
{
    mInstrumentsStorage->readLock();
    Instrument instrument = mInstrumentsStorage->getInstruments().value(RUBLE_UID);
    mInstrumentsStorage->readUnlock();

    instrument.resetIfNotFound(RUBLE_UID);

    mLogosStorage->readLock();
    Logo* logo = mLogosStorage->getLogo(RUBLE_UID);
    mLogosStorage->readUnlock();

    mInitOperations.clear();

    Operation operation;

    operation.timestamp                       = mStartTimestamp;
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

    mInitOperations.append(operation);
}

void SimulatorDateRangeDecisionMakerThread::initLogs()
{
    mInitEntries.clear();
}

void SimulatorDateRangeDecisionMakerThread::initPortfolio()
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

    mInitPortfolio.positions.clear();
    mInitPortfolio.positions << category1 << category2;
}

void SimulatorDateRangeDecisionMakerThread::initConfigs()
{
    if (mBestConfig)
    {
        mConfigVariants = mConfig->getSimulatorConfig()->variantsToJsonStringList();
    }
    else
    {
        mConfigVariants.clear();
        mConfigVariants.append("[\n" + mConfig->getSimulatorConfig()->toJsonString() + "\n]");
    }

    mSettingsEditor->setValue("Options/AmountOfBuyDecisions", mConfigVariants.size());

    const std::shared_ptr<IDir> dir = mDirFactory->newInstance();

    bool ok = dir->mkpath(qApp->applicationDirPath() + "/data/simulator");
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to create dir");

    for (int i = 0; i < mConfigVariants.size(); ++i)
    {
        const std::shared_ptr<IFile> configsFile = mFileFactory->newInstance(
            QString("%1/data/simulator/configs%2.json").arg(qApp->applicationDirPath(), QString::number(i))
        );

        ok = configsFile->open(QIODevice::WriteOnly);
        Q_ASSERT_X(ok, __FUNCTION__, "Failed to open file");

        configsFile->write(mConfigVariants.at(i).toUtf8());
        configsFile->close();

        mSettingsEditor->setValue(QString("Options/LastConfigId%1").arg(i), 0);
    }
}

void SimulatorDateRangeDecisionMakerThread::load()
{
    readSimulationConfig();
    initOperations();
    initLogs();
    initPortfolio();
    loadBestOperations();
    loadBestLogs();
    loadBestPortfolio();
    loadConfigs();
}

void SimulatorDateRangeDecisionMakerThread::loadBestOperations()
{
    mBestOperations = mOperationsDatabase->readOperations();

    if (!mBestOperations.isEmpty())
    {
        notifyBestResult(mBestOperations.constFirst().totalYieldWithCommissionPercent);
    }
}

void SimulatorDateRangeDecisionMakerThread::loadBestLogs()
{
    mBestEntries = mLogsDatabase->readLogs();
}

void SimulatorDateRangeDecisionMakerThread::loadBestPortfolio()
{
    mBestPortfolio = mPortfolioDatabase->readPortfolio();
}

void SimulatorDateRangeDecisionMakerThread::loadConfigs()
{
    const int amountOfBuyDecisions = mSettingsEditor->value("Options/AmountOfBuyDecisions", 0).toInt();

    mConfigVariants.clear();

    for (int i = 0; i < amountOfBuyDecisions; ++i)
    {
        const std::shared_ptr<IFile> configsFile = mFileFactory->newInstance(
            QString("%1/data/simulator/configs%2.json").arg(qApp->applicationDirPath(), QString::number(i))
        );

        if (configsFile->open(QIODevice::ReadOnly))
        {
            mConfigVariants.append(QString::fromUtf8(configsFile->readAll()));
            configsFile->close();
        }
    }
}

struct SimulationInfo
{
    explicit SimulationInfo(
        SimulatorDateRangeDecisionMakerThread* _thread, qint64 _startTime, IConfig* _config, const QStringList& _configVariants
    ) :
        thread(_thread),
        startTime(_startTime),
        config(_config)
    {
        configId.fill(0, _configVariants.size());
        amountOfConfigs.fill(0, _configVariants.size());
        processedMinutes.fill(0.0, _configVariants.size());
        remainingMinutes.fill(0.0, _configVariants.size());
        currentMinute.fill(0, _configVariants.size());
        bestTotalMoney.fill(0.0, _configVariants.size());
    }

    SimulatorDateRangeDecisionMakerThread* thread;
    qint64                                 startTime;
    IConfig*                               config;
    QList<int>                             configId;
    QList<int>                             amountOfConfigs;
    QList<double>                          processedMinutes;
    QList<double>                          remainingMinutes;
    QList<qint64>                          currentMinute;
    QList<double>                          bestTotalMoney;
};

static void simulationForParallel(
    QThread* parentThread, int /*threadId*/, QList<QString>& /*configVariants*/, int start, int end, void* additionalArgs
)
{
    SimulationInfo* simulationInfo = reinterpret_cast<SimulationInfo*>(additionalArgs);

    SimulatorDateRangeDecisionMakerThread* thread                = simulationInfo->thread;
    const qint64                           startTime             = simulationInfo->startTime;
    IConfig*                               config                = simulationInfo->config->clone();
    int*                                   configIdArray         = simulationInfo->configId.data();
    int*                                   amountOfConfigsArray  = simulationInfo->amountOfConfigs.data();
    double*                                processedMinutesArray = simulationInfo->processedMinutes.data();
    double*                                remainingMinutesArray = simulationInfo->remainingMinutes.data();
    qint64*                                currentMinuteArray    = simulationInfo->currentMinute.data();
    double*                                bestTotalMoneyArray   = simulationInfo->bestTotalMoney.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        thread->simulationWithBestConfigForBuyDecision(
            parentThread,
            startTime,
            i,
            config,
            configIdArray,
            amountOfConfigsArray,
            processedMinutesArray,
            remainingMinutesArray,
            currentMinuteArray,
            bestTotalMoneyArray
        );
    }

    config->deleteRecursively();
}

// NOLINTBEGIN(readability-function-cognitive-complexity)
void SimulatorDateRangeDecisionMakerThread::simulationWithBestConfigForBuyDecision(
    QThread* parentThread,
    qint64   startTime,
    int      buyDecisionId,
    IConfig* config,
    int*     configIdArray,
    int*     amountOfConfigsArray,
    double*  processedMinutesArray,
    double*  remainingMinutesArray,
    qint64*  currentMinuteArray,
    double*  bestTotalMoneyArray
)
{
    const int        configId       = mSettingsEditor->value(QString("Options/LastConfigId%1").arg(buyDecisionId), 0).toInt();
    const qint64     totalMinutes   = (mEndTimestamp - mStartTimestamp) / ONE_MINUTE;
    double           bestTotalMoney = 0.0;
    QList<Operation> bestOperations = mOperationsDatabase->readOperations(buyDecisionId);
    QList<LogEntry>  bestEntries    = mLogsDatabase->readLogs(buyDecisionId);
    Portfolio        bestPortfolio  = mPortfolioDatabase->readPortfolio(buyDecisionId);

    if (!bestOperations.isEmpty())
    {
        const Operation& lastOperation = bestOperations.constFirst(); // Since it reversed

        bestTotalMoney = quotationToDouble(lastOperation.totalMoney);
        notifyBestResult(bestTotalMoneyArray, buyDecisionId, bestTotalMoney);
    }

    mStocksStorage->readLock();
    const QList<Stock*> stocks = mStocksStorage->getStocks();
    mStocksStorage->readUnlock();

    const simdjson::padded_string jsonData(mConfigVariants.at(buyDecisionId).toStdString());

    simdjson::ondemand::parser parser;

    try
    {
        simdjson::ondemand::document doc = parser.iterate(jsonData);

        simdjson::ondemand::array jsonConfigs     = doc.get_array();
        const int                 amountOfConfigs = static_cast<int>(jsonConfigs.count_elements());

        int i = 0;

        for (const simdjson::ondemand::object jsonObject : jsonConfigs)
        {
            if (!parentThread->isInterruptionRequested())
            {
                if (i < configId)
                {
                    ++i;

                    continue;
                }

                notifyTotalProgressChanged(configIdArray, amountOfConfigsArray, buyDecisionId, i, amountOfConfigs);

                config->getSimulatorConfig()->fromJsonObject(jsonObject);

                double                           totalMoney = mStartMoney;
                QList<Operation>                 operations = mInitOperations;
                QList<LogEntry>                  entries    = mInitEntries;
                Portfolio                        portfolio  = mInitPortfolio;
                QuantityAndCostDoubleInstruments instruments;

                qint64 timestamp = mStartTimestamp;

                while (timestamp < mEndTimestamp && !parentThread->isInterruptionRequested())
                {
                    if (timestamp % NOTIFY_PROGRESS_STEP == 0)
                    {
                        const qint64 currentMinute = (timestamp - mStartTimestamp) / ONE_MINUTE;

                        if (i != configId || currentMinute > 0)
                        {
                            notifyProgressChanged(
                                startTime,
                                configId,
                                i,
                                amountOfConfigs,
                                processedMinutesArray,
                                remainingMinutesArray,
                                currentMinuteArray,
                                buyDecisionId,
                                currentMinute,
                                totalMinutes
                            );
                        }
                    }

                    const InstrumentsForTrading& instrumentsForTrading =
                        mDecisionMaker->makeDecision(parentThread, timestamp, config, portfolio, stocks, false, 0, true, false);

                    if (!instrumentsForTrading.isEmpty())
                    {
                        simulateTrading(
                            timestamp, instrumentsForTrading, totalMoney, operations, entries, portfolio, instruments
                        );
                    }

                    timestamp += ONE_MINUTE;
                }

                if (!parentThread->isInterruptionRequested())
                {
                    if (totalMoney > bestTotalMoney)
                    {
                        bestTotalMoney = totalMoney;

                        mSettingsEditor->setValue(QString("Options/BestConfigId%1").arg(buyDecisionId), i);

                        bestOperations = reverseOperations(operations);
                        bestEntries    = reverseEntries(entries);
                        bestPortfolio  = portfolio;

                        mOperationsDatabase->writeOperations(bestOperations, buyDecisionId);
                        mLogsDatabase->writeLogs(bestEntries, buyDecisionId);
                        mPortfolioDatabase->writePortfolio(bestPortfolio, buyDecisionId);

                        notifyBestResult(bestTotalMoneyArray, buyDecisionId, bestTotalMoney);
                    }

                    ++i;
                    mSettingsEditor->setValue(QString("Options/LastConfigId%1").arg(buyDecisionId), i);
                }
            }
        }

        notifyTotalProgressChanged(configIdArray, amountOfConfigsArray, buyDecisionId, amountOfConfigs, amountOfConfigs);
        notifyProgressChanged(
            startTime,
            configId,
            amountOfConfigs,
            amountOfConfigs,
            processedMinutesArray,
            remainingMinutesArray,
            currentMinuteArray,
            buyDecisionId,
            0,
            totalMinutes
        );
    }
    catch (...)
    {
        qWarning() << "Failed to parse configs";
    }
}
// NOLINTEND(readability-function-cognitive-complexity)

void SimulatorDateRangeDecisionMakerThread::simulationWithBestConfig(qint64 startTime)
{
    SimulationInfo simulationInfo(this, startTime, mConfig, mConfigVariants);
    processInParallel(QThread::currentThread(), mConfigVariants, simulationForParallel, &simulationInfo);
}

void SimulatorDateRangeDecisionMakerThread::simulationWithoutBestConfig(qint64 startTime)
{
    const int configId = mSettingsEditor->value("Options/LastConfigId0", 0).toInt();

    if (configId > 0)
    {
        return;
    }

    const qint64 totalMinutes = (mEndTimestamp - mStartTimestamp) / ONE_MINUTE;

    mStocksStorage->readLock();
    const QList<Stock*> stocks = mStocksStorage->getStocks();
    mStocksStorage->readUnlock();

    double                           totalMoney = mStartMoney;
    QList<Operation>                 operations = mInitOperations;
    QList<LogEntry>                  entries    = mInitEntries;
    Portfolio                        portfolio  = mInitPortfolio;
    QuantityAndCostDoubleInstruments instruments;

    qint64 timestamp = mStartTimestamp;

    while (timestamp < mEndTimestamp && !QThread::currentThread()->isInterruptionRequested())
    {
        if (timestamp % NOTIFY_PROGRESS_STEP == 0)
        {
            const qint64 currentMinute = (timestamp - mStartTimestamp) / ONE_MINUTE;

            if (currentMinute > 0)
            {
                const qint64 deltaTime = QDateTime::currentMSecsSinceEpoch() - startTime;

                const double processedMinutes = currentMinute;
                const double remainingMinutes = totalMinutes - currentMinute;

                qint64 remainingMilliseconds  = (deltaTime / processedMinutes) * remainingMinutes;
                remainingMilliseconds        /= MS_IN_SECOND;
                const int seconds             = remainingMilliseconds % SECONDS_IN_MINUTE;
                remainingMilliseconds        /= SECONDS_IN_MINUTE;
                const int minutes             = remainingMilliseconds % MINUTES_IN_HOUR;
                remainingMilliseconds        /= MINUTES_IN_HOUR;
                const int hours               = remainingMilliseconds;

                emit progressChanged(
                    currentMinute,
                    totalMinutes,
                    QString("%1:%2:%3")
                        .arg(
                            QString::number(hours).rightJustified(2, '0'),
                            QString::number(minutes).rightJustified(2, '0'),
                            QString::number(seconds).rightJustified(2, '0')
                        )
                );
            }
        }

        const InstrumentsForTrading& instrumentsForTrading =
            mDecisionMaker->makeDecision(QThread::currentThread(), timestamp, mConfig, portfolio, stocks, false, 0, true, true);

        if (!instrumentsForTrading.isEmpty())
        {
            simulateTrading(timestamp, instrumentsForTrading, totalMoney, operations, entries, portfolio, instruments);
        }

        timestamp += ONE_MINUTE;
    }

    if (!QThread::currentThread()->isInterruptionRequested())
    {
        mBestOperations = reverseOperations(operations);
        mBestEntries    = reverseEntries(entries);
        mBestPortfolio  = portfolio;

        mOperationsDatabase->writeOperations(mBestOperations);
        mLogsDatabase->writeLogs(mBestEntries);
        mPortfolioDatabase->writePortfolio(mBestPortfolio);

        mSettingsEditor->setValue("Options/LastConfigId0", 1);
    }
}

void SimulatorDateRangeDecisionMakerThread::simulateTrading(
    qint64                            timestamp,
    const InstrumentsForTrading&      instrumentsForTrading,
    double&                           totalMoney,
    QList<Operation>&                 operations,
    QList<LogEntry>&                  entries,
    Portfolio&                        portfolio,
    QuantityAndCostDoubleInstruments& instruments
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
        simulateSell(timestamp, it.key(), it.value(), totalMoney, operations, entries, portfolio, instruments);
    }

    for (auto it = instrumentsForBuy.constBegin(); it != instrumentsForBuy.constEnd(); ++it)
    {
        simulateBuy(timestamp, it.key(), it.value(), totalMoney, operations, entries, portfolio, instruments);
    }
}

void SimulatorDateRangeDecisionMakerThread::simulateSell(
    qint64&                           timestamp,
    const QString&                    instrumentId,
    const TradingInfo&                tradingInfo,
    double&                           totalMoney,
    QList<Operation>&                 operations,
    QList<LogEntry>&                  entries,
    Portfolio&                        portfolio,
    QuantityAndCostDoubleInstruments& instruments
)
{
    if (!instruments.contains(instrumentId))
    {
        return;
    }

    mInstrumentsStorage->readLock();
    Instrument instrument = mInstrumentsStorage->getInstruments().value(instrumentId);
    mInstrumentsStorage->readUnlock();

    instrument.resetIfNotFound(instrumentId);

    const QuantityAndCostDouble quantityAndCost = instruments.value(instrumentId);

    mUserStorage->readLock();
    const float commission = mUserStorage->getCommission() / HUNDRED_PERCENT;
    mUserStorage->readUnlock();

    const double cost            = quantityAndCost.quantity * tradingInfo.price;
    const double totalCommission = cost * commission;

    mLogosStorage->readLock();
    Logo* logo = mLogosStorage->getLogo(instrumentId);
    mLogosStorage->readUnlock();

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
        totalMoney,
        operations,
        portfolio
    );
    simulateSellForLogs(
        timestamp, instrumentId, logo, instrument, tradingInfo.cause, quantityAndCost.quantity, tradingInfo.price, entries
    );
    simulateSellForPortfolio(instrumentId, cost, totalCommission, portfolio);
    simulateSellForInstruments(instrumentId, instruments);
}

void SimulatorDateRangeDecisionMakerThread::simulateSellForOperations(
    qint64&           timestamp,
    const QString&    instrumentId,
    Logo*             logo,
    const Instrument& instrument,
    qint64            quantity,
    double            costFifo,
    float             price,
    double            cost,
    double            totalCommission,
    double&           totalMoney,
    QList<Operation>& operations,
    Portfolio&        portfolio
)
{
    const double avgPrice            = costFifo / quantity;
    const double yield               = cost - costFifo;
    const double yieldWithCommission = yield - totalCommission;

    totalMoney                            += yieldWithCommission;
    const double totalYieldWithCommission  = totalMoney - mStartMoney;

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
    operation.remainedMoney  = quotationFromDouble(portfolio.positions[CURRENCY_ID].items.first().cost + cost - totalCommission);
    operation.totalMoney     = quotationFromDouble(totalMoney);
    operation.pricePrecision = instrument.pricePrecision;
    operation.paymentPrecision    = instrument.pricePrecision;
    operation.commissionPrecision = instrument.pricePrecision;

    operations.append(operation);
    ++timestamp;
}

void SimulatorDateRangeDecisionMakerThread::simulateSellForLogs(
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
    entry.message   = tr("Order to sell %1 created with a price %2")
                        .arg(QString::number(quantity), QString::number(price, 'f', instrument.pricePrecision) + " \u20BD");

    entries.append(entry);
    ++timestamp;

    entry.timestamp = timestamp;
    entry.level     = LOG_LEVEL_VERBOSE;
    entry.message   = tr("Order completed. %1 sold with a price %2")
                        .arg(QString::number(quantity), QString::number(price, 'f', instrument.pricePrecision) + " \u20BD");

    entries.append(entry);
    ++timestamp;

    entry.timestamp = timestamp;
    entry.level     = LOG_LEVEL_VERBOSE;
    entry.message   = tr("Trade completed successfully");

    entries.append(entry);
    ++timestamp;
}

void SimulatorDateRangeDecisionMakerThread::simulateSellForPortfolio(
    const QString& instrumentId, double cost, double totalCommission, Portfolio& portfolio
)
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

void SimulatorDateRangeDecisionMakerThread::simulateSellForInstruments(
    const QString& instrumentId, QuantityAndCostDoubleInstruments& instruments
)
{
    instruments.remove(instrumentId);
}

void SimulatorDateRangeDecisionMakerThread::simulateBuy(
    qint64&                           timestamp,
    const QString&                    instrumentId,
    const TradingInfo&                tradingInfo,
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
        static_cast<qint64>(portfolio.positions.at(CURRENCY_ID).items.constFirst().cost / lotPriceWithCommission)
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
            timestamp,
            instrumentId,
            logo,
            instrument,
            quantity,
            tradingInfo.price,
            cost,
            totalCommission,
            totalMoney,
            operations,
            portfolio
        );
        simulateBuyForLogs(timestamp, instrumentId, logo, instrument, tradingInfo.cause, quantity, tradingInfo.price, entries);
        simulateBuyForPortfolio(instrumentId, logo, instrument, quantity, tradingInfo.price, cost, totalCommission, portfolio);
        simulateBuyForInstruments(instrumentId, quantity, cost, instruments);
    }
}

void SimulatorDateRangeDecisionMakerThread::simulateBuyForOperations(
    qint64&           timestamp,
    const QString&    instrumentId,
    Logo*             logo,
    const Instrument& instrument,
    qint64            quantity,
    float             price,
    double            cost,
    double            totalCommission,
    double&           totalMoney,
    QList<Operation>& operations,
    Portfolio&        portfolio
)
{
    totalMoney                            -= totalCommission;
    const double totalYieldWithCommission  = totalMoney - mStartMoney;

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
    operation.remainedMoney  = quotationFromDouble(portfolio.positions[CURRENCY_ID].items.first().cost - cost - totalCommission);
    operation.totalMoney     = quotationFromDouble(totalMoney);
    operation.pricePrecision = instrument.pricePrecision;
    operation.paymentPrecision    = instrument.pricePrecision;
    operation.commissionPrecision = instrument.pricePrecision;

    operations.append(operation);
    ++timestamp;
}

void SimulatorDateRangeDecisionMakerThread::simulateBuyForLogs(
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
    entry.message   = tr("Order to buy %1 created with a price %2")
                        .arg(QString::number(quantity), QString::number(price, 'f', instrument.pricePrecision) + " \u20BD");

    entries.append(entry);
    ++timestamp;

    entry.timestamp = timestamp;
    entry.level     = LOG_LEVEL_VERBOSE;
    entry.message   = tr("Order completed. %1 bought with a price %2")
                        .arg(QString::number(quantity), QString::number(price, 'f', instrument.pricePrecision) + " \u20BD");

    entries.append(entry);
    ++timestamp;

    entry.timestamp = timestamp;
    entry.level     = LOG_LEVEL_VERBOSE;
    entry.message   = tr("Trade completed successfully");

    entries.append(entry);
    ++timestamp;
}

void SimulatorDateRangeDecisionMakerThread::simulateBuyForPortfolio(
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

void SimulatorDateRangeDecisionMakerThread::simulateBuyForInstruments(
    const QString& instrumentId, qint64 quantity, double cost, QuantityAndCostDoubleInstruments& instruments
)
{
    QuantityAndCostDouble quantityAndCost;

    quantityAndCost.quantity = quantity;
    quantityAndCost.cost     = cost;

    instruments[instrumentId] = quantityAndCost;
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

QList<Operation> SimulatorDateRangeDecisionMakerThread::reverseOperations(QList<Operation>& operations)
{
    QList<Operation> res;
    res.resizeForOverwrite(operations.size());

    ReverseOperationsInfo reverseOperationsInfo(&operations);
    processInParallel(QThread::currentThread(), res, reverseOperationsForParallel, &reverseOperationsInfo);

    return res;
}

struct ReverseEntriesInfo
{
    explicit ReverseEntriesInfo(QList<LogEntry>* _entries) :
        entries(_entries)
    {
    }

    QList<LogEntry>* entries;
};

static void
reverseEntriesForParallel(QThread* parentThread, int /*threadId*/, QList<LogEntry>& res, int start, int end, void* additionalArgs)
{
    ReverseEntriesInfo* reverseEntriesInfo = reinterpret_cast<ReverseEntriesInfo*>(additionalArgs);

    LogEntry* entriesArray = reverseEntriesInfo->entries->data();

    LogEntry* resArray = res.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        resArray[i] = entriesArray[res.size() - i - 1];
    }
}

QList<LogEntry> SimulatorDateRangeDecisionMakerThread::reverseEntries(QList<LogEntry>& entries)
{
    QList<LogEntry> res;
    res.resizeForOverwrite(entries.size());

    ReverseEntriesInfo reverseEntriesInfo(&entries);
    processInParallel(QThread::currentThread(), res, reverseEntriesForParallel, &reverseEntriesInfo);

    return res;
}

void SimulatorDateRangeDecisionMakerThread::updateCostAndPart()
{
    const Operation& lastOperation  = mBestOperations.constFirst(); // Since it reversed
    const double     bestTotalMoney = quotationToDouble(lastOperation.totalMoney);

    for (PortfolioCategoryItem& category : mBestPortfolio.positions)
    {
        category.cost = 0.0;

        for (PortfolioItem& item : category.items)
        {
            item.part = (item.cost / bestTotalMoney) * HUNDRED_PERCENT;

            category.cost += item.cost;
        }

        category.part = (category.cost / bestTotalMoney) * HUNDRED_PERCENT;
    }
}

void SimulatorDateRangeDecisionMakerThread::updatePrice()
{
    PortfolioCategoryItem& category = mBestPortfolio.positions[SHARE_ID]; // clazy:exclude=detaching-member

    mStocksStorage->readLock();
    const QList<Stock*> stocks = mStocksStorage->getStocks();
    mStocksStorage->readUnlock();

    for (PortfolioItem& item : category.items)
    {
        Stock* stock = nullptr;

        for (Stock* s : stocks)
        {
            if (s->meta.instrumentId == item.instrumentId)
            {
                stock = s;

                break;
            }
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

void SimulatorDateRangeDecisionMakerThread::notifyTotalProgressChanged(
    int* configIdArray, int* amountOfConfigsArray, int buyDecisionId, int configId, int amountOfConfigs
)
{
    static QMutex      mutex;
    const QMutexLocker lock(&mutex);

    configIdArray[buyDecisionId]        = configId;
    amountOfConfigsArray[buyDecisionId] = amountOfConfigs;

    configId        = 0;
    amountOfConfigs = 0;

    for (int i = 0; i < mConfigVariants.size(); ++i)
    {
        configId        += configIdArray[i];
        amountOfConfigs += amountOfConfigsArray[i];
    }

    emit totalProgressChanged(configId, amountOfConfigs);
}

void SimulatorDateRangeDecisionMakerThread::notifyProgressChanged(
    qint64  startTime,
    int     configId,
    int     currentConfig,
    int     amountOfConfigs,
    double* processedMinutesArray,
    double* remainingMinutesArray,
    qint64* currentMinuteArray,
    int     buyDecisionId,
    qint64  currentMinute,
    qint64  totalMinutes
)
{
    const qint64 deltaTime = QDateTime::currentMSecsSinceEpoch() - startTime;

    double processedMinutes = ((currentConfig - configId) * totalMinutes) + currentMinute;
    double remainingMinutes = ((amountOfConfigs - currentConfig) * totalMinutes) - currentMinute;

    static QMutex      mutex;
    const QMutexLocker lock(&mutex);

    processedMinutesArray[buyDecisionId] = processedMinutes;
    remainingMinutesArray[buyDecisionId] = remainingMinutes;
    currentMinuteArray[buyDecisionId]    = currentMinute;

    processedMinutes = 0.0;
    remainingMinutes = 0.0;

    for (int i = 0; i < mConfigVariants.size(); ++i)
    {
        processedMinutes += processedMinutesArray[i];
        remainingMinutes += remainingMinutesArray[i];
        currentMinute     = qMax(currentMinute, currentMinuteArray[i]);
    }

    qint64 remainingMilliseconds  = (deltaTime / processedMinutes) * remainingMinutes;
    remainingMilliseconds        /= MS_IN_SECOND;
    const int seconds             = remainingMilliseconds % SECONDS_IN_MINUTE;
    remainingMilliseconds        /= SECONDS_IN_MINUTE;
    const int minutes             = remainingMilliseconds % MINUTES_IN_HOUR;
    remainingMilliseconds        /= MINUTES_IN_HOUR;
    const int hours               = remainingMilliseconds;

    emit progressChanged(
        currentMinute,
        totalMinutes,
        QString("%1:%2:%3")
            .arg(
                QString::number(hours).rightJustified(2, '0'),
                QString::number(minutes).rightJustified(2, '0'),
                QString::number(seconds).rightJustified(2, '0')
            )
    );
}

void
SimulatorDateRangeDecisionMakerThread::notifyBestResult(double* bestTotalMoneyArray, int buyDecisionId, double bestTotalMoney)
{
    static QMutex      mutex;
    const QMutexLocker lock(&mutex);

    bestTotalMoneyArray[buyDecisionId] = bestTotalMoney;

    for (int i = 0; i < mConfigVariants.size(); ++i)
    {
        bestTotalMoney = qMax(bestTotalMoney, bestTotalMoneyArray[i]);
    }

    const double totalYieldWithCommission        = bestTotalMoney - mStartMoney;
    const double totalYieldWithCommissionPercent = (totalYieldWithCommission / mStartMoney) * HUNDRED_PERCENT;

    notifyBestResult(totalYieldWithCommissionPercent);
}

void SimulatorDateRangeDecisionMakerThread::notifyBestResult(double totalYieldWithCommissionPercent)
{
    const QString prefix     = totalYieldWithCommissionPercent > 0 ? "+" : "";
    const QString bestResult = prefix + QString::number(totalYieldWithCommissionPercent, 'f', 2) + "%";

    QColor color;

    if (totalYieldWithCommissionPercent > -ZERO_LIMIT && totalYieldWithCommissionPercent < ZERO_LIMIT)
    {
        color = NORMAL_COLOR;
    }
    else
    {
        if (totalYieldWithCommissionPercent > 0)
        {
            color = GREEN_COLOR;
        }
        else
        {
            color = RED_COLOR;
        }
    }

    emit bestResultChanged(bestResult, color);
}

void SimulatorDateRangeDecisionMakerThread::optimizeOperations()
{
    if (mBestOperations.size() > mLimitOperations)
    {
        mBestOperations = mOptimizer->optimizeOperations(mBestOperations, mOptimizeOperationsSize, QStringList());
        mOperationsDatabase->writeOperations(mBestOperations);
    }
}

void SimulatorDateRangeDecisionMakerThread::optimizeLogs()
{
    if (mBestEntries.size() > mLimitLogs)
    {
        mBestEntries = mOptimizer->optimizeLogs(mBestEntries, mOptimizeLogsSize);
        mLogsDatabase->writeLogs(mBestEntries);
    }
}
