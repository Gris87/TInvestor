#include "src/threads/simulatormakedecision/simulatormakedecisionthread.h"

#include <QDebug>



SimulatorMakeDecisionThread::SimulatorMakeDecisionThread(
    IStocksStorage* stocksStorage, IDecisionMaker* decisionMaker, QObject* parent
) :
    ISimulatorMakeDecisionThread(parent),
    mStocksStorage(stocksStorage),
    mDecisionMaker(decisionMaker)
{
    qDebug() << "Create SimulatorMakeDecisionThread";
}

SimulatorMakeDecisionThread::~SimulatorMakeDecisionThread()
{
    qDebug() << "Destroy SimulatorMakeDecisionThread";
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
