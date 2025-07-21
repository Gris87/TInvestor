#include "src/threads/simulatordaterangedecisionmaker/simulatordaterangedecisionmakerthread.h"

#include <QDebug>

#include "src/grpc/utils.h"



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
    mLoaded(),
    mStartMoney(),
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
