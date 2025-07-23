#include "src/threads/simulatordaterangedecisionmaker/simulatordaterangedecisionmakerthread.h"

#include <QCoreApplication>
#include <QDebug>

#include "src/grpc/utils.h"



const char* const DATE_FORMAT = "yyyy-MM-dd";
const char* const RUBLE_UID   = "a92e2e25-a698-45cc-a781-167cf465257c";
const QColor      GREEN_COLOR  = QColor("#2BD793"); // clazy:exclude=non-pod-global-static
const QColor      RED_COLOR    = QColor("#ED6F7E"); // clazy:exclude=non-pod-global-static
const QColor      NORMAL_COLOR = QColor("#97AEC4"); // clazy:exclude=non-pod-global-static

constexpr float  ZERO_LIMIT           = 0.0001f;
constexpr qint64 MS_IN_SECOND         = 1000LL;
constexpr qint64 ONE_MINUTE           = 60LL * MS_IN_SECOND;
constexpr qint64 ONE_HOUR             = 60LL * ONE_MINUTE;
constexpr qint64 NOTIFY_PROGRESS_STEP = 1LL * ONE_HOUR;
const int        SECONDS_IN_MINUTE    = 60;
const int        MINUTES_IN_HOUR      = 60;



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
    mInitOperations(),
    mInitEntries(),
    mInitPortfolio(),
    mOperations(),
    mEntries(),
    mPortfolio(),
    mBestOperations(),
    mBestEntries(),
    mBestPortfolio(),
    mInstruments(),
    mResetted(),
    mStartMoney(),
    mStartTimestamp(),
    mEndTimestamp(),
    mBestConfig(),
    mConfigVariants(),
    mTotalMoney(),
    mBestTotalMoney()
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

    const int    configId     = mSettingsEditor->value("Options/LastConfigId", 0).toInt();
    const qint64 totalMinutes = (mEndTimestamp - mStartTimestamp) / ONE_MINUTE;

    const simdjson::padded_string jsonData(mConfigVariants.toStdString());

    simdjson::ondemand::parser parser;

    try
    {
        simdjson::ondemand::document doc = parser.iterate(jsonData);

        simdjson::ondemand::array jsonConfigs     = doc.get_array();
        const int                 amountOfConfigs = static_cast<int>(jsonConfigs.count_elements());

        qint64 startTime = QDateTime::currentMSecsSinceEpoch();
        int    i         = 0;

        for (const simdjson::ondemand::object jsonObject : jsonConfigs)
        {
            if (i < configId || QThread::currentThread()->isInterruptionRequested())
            {
                startTime = QDateTime::currentMSecsSinceEpoch();
                ++i;

                continue;
            }

            emit totalProgressChanged(i, amountOfConfigs);

            mConfig->getSimulatorConfig()->fromJsonObject(jsonObject);

            mTotalMoney = mStartMoney;
            mOperations = mInitOperations;
            mEntries    = mInitEntries;
            mPortfolio  = mInitPortfolio;
            mInstruments.clear();

            qint64 timestamp = mStartTimestamp;

            while (timestamp < mEndTimestamp && !QThread::currentThread()->isInterruptionRequested())
            {
                if (timestamp % NOTIFY_PROGRESS_STEP == 0)
                {
                    const qint64 currentMinute = (timestamp - mStartTimestamp) / ONE_MINUTE;

                    if (i != configId || currentMinute > 0)
                    {
                        const qint64 deltaTime = QDateTime::currentMSecsSinceEpoch() - startTime;

                        const double processedMinutes = ((i - configId) * totalMinutes) + currentMinute;
                        const double remainingMinutes = ((amountOfConfigs - i) * totalMinutes) - currentMinute;

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

                    // TODO: Remove it
                    msleep(10);
                }

                timestamp += ONE_MINUTE;
            }

            if (!QThread::currentThread()->isInterruptionRequested())
            {
                if (mTotalMoney > mBestTotalMoney)
                {
                    mBestTotalMoney = mTotalMoney;

                    mBestOperations = mOperations;
                    mBestEntries    = mEntries;
                    mBestPortfolio  = mPortfolio;

                    mOperationsDatabase->writeOperations(mBestOperations);
                    mLogsDatabase->writeLogs(mBestEntries);
                    mPortfolioDatabase->writePortfolio(mBestPortfolio);

                    notifyBestResult();
                }

                ++i;
                mSettingsEditor->setValue("Options/LastConfigId", i);
            }
        }
    }
    catch (...)
    {
        qWarning() << "Failed to parse configs";
    }

    emit operationsRead(mBestOperations);
    emit logsRead(mBestEntries);
    emit portfolioChanged(mBestPortfolio);

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

    mBestTotalMoney = 0;
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
        mConfigVariants = mConfig->getSimulatorConfig()->variantsToJsonString();
    }
    else
    {
        mConfigVariants = "[" + mConfig->getSimulatorConfig()->toJsonString() + "]";
    }

    const std::shared_ptr<IDir> dir = mDirFactory->newInstance();

    bool ok = dir->mkpath(qApp->applicationDirPath() + "/data/simulator");
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to create dir");

    const std::shared_ptr<IFile> configsFile =
        mFileFactory->newInstance(qApp->applicationDirPath() + "/data/simulator/configs.json");

    ok = configsFile->open(QIODevice::WriteOnly);
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to open file");

    configsFile->write(mConfigVariants.toUtf8());
    configsFile->close();

    mSettingsEditor->setValue("Options/LastConfigId", 0);
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
        const Operation& lastOperation = mBestOperations.constFirst(); // Since it reversed

        mBestTotalMoney = quotationToDouble(lastOperation.totalMoney);
        notifyBestResult();
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
    const std::shared_ptr<IFile> configsFile =
        mFileFactory->newInstance(qApp->applicationDirPath() + "/data/simulator/configs.json");

    if (configsFile->open(QIODevice::ReadOnly))
    {
        mConfigVariants = QString::fromUtf8(configsFile->readAll());
        configsFile->close();
    }
}

void SimulatorDateRangeDecisionMakerThread::notifyBestResult()
{
    const double totalYieldWithCommissionPercent = mBestOperations.constFirst().totalYieldWithCommissionPercent;

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
