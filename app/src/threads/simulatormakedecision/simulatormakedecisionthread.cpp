#include "src/threads/simulatormakedecision/simulatormakedecisionthread.h"

#include <QDebug>



SimulatorMakeDecisionThread::SimulatorMakeDecisionThread(
    IStocksStorage* stocksStorage, IDecisionMaker* decisionMaker, QObject* parent
) :
    ISimulatorMakeDecisionThread(parent),
    mStocksStorage(stocksStorage),
    mDecisionMaker(decisionMaker),
    mPortfolio(),
    mResetted(),
    mLoaded(),
    mStartMoney()
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
    const InstrumentsForTrading& instrumentsForTrading = mDecisionMaker->makeDecision(mPortfolio, mStocksStorage->getStocks(), 0);
    mStocksStorage->readUnlock();

    if (!instrumentsForTrading.isEmpty())
    {
        simulateTrading(instrumentsForTrading);
    }

    qDebug() << "Finish SimulatorMakeDecisionThread";
}

void SimulatorMakeDecisionThread::reset()
{
    mResetted = true;
}

void SimulatorMakeDecisionThread::setStartMoney(int value)
{
    mStartMoney = value;
}

void SimulatorMakeDecisionThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}

void SimulatorMakeDecisionThread::init()
{
}

void SimulatorMakeDecisionThread::load()
{
}

void SimulatorMakeDecisionThread::simulateTrading(const InstrumentsForTrading& /*instrumentsForTrading*/)
{
}
