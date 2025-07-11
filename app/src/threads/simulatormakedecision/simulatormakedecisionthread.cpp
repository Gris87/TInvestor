#include "src/threads/simulatormakedecision/simulatormakedecisionthread.h"

#include <QDebug>



SimulatorMakeDecisionThread::SimulatorMakeDecisionThread(IConfig* config, IStocksStorage* stocksStorage, QObject* parent) :
    ISimulatorMakeDecisionThread(parent),
    mMutex(new QMutex()),
    mConfig(config),
    mStocksStorage(stocksStorage),
    mAccountId(),
    mKeepMoney()
{
    qDebug() << "Create SimulatorMakeDecisionThread";
}

SimulatorMakeDecisionThread::~SimulatorMakeDecisionThread()
{
    qDebug() << "Destroy SimulatorMakeDecisionThread";

    delete mMutex;
}

void SimulatorMakeDecisionThread::run()
{
    qDebug() << "Running SimulatorMakeDecisionThread";

    blockSignals(false);

    // TODO: Do we need it?

    qDebug() << "Finish SimulatorMakeDecisionThread";
}

void SimulatorMakeDecisionThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}
