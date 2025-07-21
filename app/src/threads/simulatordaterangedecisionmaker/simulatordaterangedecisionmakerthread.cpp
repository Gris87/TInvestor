#include "src/threads/simulatordaterangedecisionmaker/simulatordaterangedecisionmakerthread.h"

#include <QDebug>

#include "src/grpc/utils.h"



const char* const DATE_FORMAT = "yyyy-MM-dd";



SimulatorDateRangeDecisionMakerThread::SimulatorDateRangeDecisionMakerThread(
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
    }
    else
    {
    }
}

void SimulatorDateRangeDecisionMakerThread::load()
{
    readSimulationConfig();
    loadConfigs();
}

void SimulatorDateRangeDecisionMakerThread::loadConfigs()
{
}
