#include "src/threads/autopilotmakedecision/autopilotmakedecisionthread.h"

#include <QDebug>



AutoPilotMakeDecisionThread::AutoPilotMakeDecisionThread(
    IStocksStorage* stocksStorage, IDecisionMaker* decisionMaker, QObject* parent
) :
    IAutoPilotMakeDecisionThread(parent),
    mMutex(new QMutex()),
    mStocksStorage(stocksStorage),
    mDecisionMaker(decisionMaker),
    mAccountId(),
    mKeepMoney()
{
    qDebug() << "Create AutoPilotMakeDecisionThread";
}

AutoPilotMakeDecisionThread::~AutoPilotMakeDecisionThread()
{
    qDebug() << "Destroy AutoPilotMakeDecisionThread";

    delete mMutex;
}

void AutoPilotMakeDecisionThread::run()
{
    qDebug() << "Running AutoPilotMakeDecisionThread";

    blockSignals(false);

    // TODO: Do we need it?

    qDebug() << "Finish AutoPilotMakeDecisionThread";
}

void AutoPilotMakeDecisionThread::setAccount(const QString& accountId)
{
    const QMutexLocker lock(mMutex);

    mAccountId = accountId;
}

void AutoPilotMakeDecisionThread::setKeepMoney(int value)
{
    const QMutexLocker lock(mMutex);

    mKeepMoney = value;
}

QString AutoPilotMakeDecisionThread::accountId() const
{
    const QMutexLocker lock(mMutex);

    return mAccountId;
}

int AutoPilotMakeDecisionThread::keepMoney() const
{
    const QMutexLocker lock(mMutex);

    return mKeepMoney;
}

void AutoPilotMakeDecisionThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}
