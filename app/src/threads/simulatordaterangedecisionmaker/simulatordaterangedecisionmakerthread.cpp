#include "src/threads/simulatordaterangedecisionmaker/simulatordaterangedecisionmakerthread.h"

#include <QCoreApplication>
#include <QDebug>

#include "src/grpc/utils.h"



const char* const DATE_FORMAT = "yyyy-MM-dd";



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
    mPortfolio(),
    mInstruments(),
    mResetted(),
    mStartMoney(),
    mStartTimestamp(),
    mEndTimestamp(),
    mBestConfig(),
    mConfigVariants(),
    mTotalYieldWithCommission(),
    mTotalMoney()
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

    const int configId = mSettingsEditor->value("Options/LastConfigId", 0).toInt();

    const simdjson::padded_string jsonData(mConfigVariants.toStdString());

    simdjson::ondemand::parser parser;

    try
    {
        simdjson::ondemand::document doc = parser.iterate(jsonData);

        simdjson::ondemand::array jsonConfigs = doc.get_array();
        const int                 amountOfConfigs = static_cast<int>(jsonConfigs.count_elements());

        int i = 0;

        for (const simdjson::ondemand::object jsonObject : jsonConfigs)
        {
            if (i < configId || QThread::currentThread()->isInterruptionRequested())
            {
                ++i;

                continue;
            }

            emit totalProgressChanged(i, amountOfConfigs);

            mSettingsEditor->setValue("Options/LastConfigId", i);
            mConfig->getSimulatorConfig()->fromJsonObject(jsonObject);

            ++i;
        }
    }
    catch (...)
    {
        qWarning() << "Failed to parse configs";
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
    initConfigs();
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
    loadConfigs();
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
