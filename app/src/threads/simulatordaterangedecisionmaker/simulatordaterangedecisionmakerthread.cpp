#include "src/threads/simulatordaterangedecisionmaker/simulatordaterangedecisionmakerthread.h"

#include <QCoreApplication>
#include <QDebug>

#include "src/grpc/utils.h"



const char* const DATE_FORMAT = "yyyy-MM-dd";

constexpr qint64 MS_IN_SECOND         = 1000LL;
constexpr qint64 ONE_MINUTE           = 60LL * MS_IN_SECOND;
constexpr qint64 ONE_HOUR             = 60LL * ONE_MINUTE;
constexpr qint64 NOTIFY_PROGRESS_STEP = 24LL * ONE_HOUR;
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

            mSettingsEditor->setValue("Options/LastConfigId", i);
            mConfig->getSimulatorConfig()->fromJsonObject(jsonObject);

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
                }

                timestamp += ONE_MINUTE;
            }

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
