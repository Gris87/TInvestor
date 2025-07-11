#include "src/threads/makedecision/makedecisionthread.h"

#include <QDebug>



MakeDecisionThread::MakeDecisionThread(IConfig* config, IStocksStorage* stocksStorage, QObject* parent) :
    IMakeDecisionThread(parent),
    mMutex(new QMutex()),
    mConfig(config),
    mStocksStorage(stocksStorage),
    mAccountId(),
    mKeepMoney()
{
    qDebug() << "Create MakeDecisionThread";
}

MakeDecisionThread::~MakeDecisionThread()
{
    qDebug() << "Destroy MakeDecisionThread";

    delete mMutex;
}

void MakeDecisionThread::run()
{
    qDebug() << "Running MakeDecisionThread";

    blockSignals(false);

    // TODO: Do we need it?

    qDebug() << "Finish MakeDecisionThread";
}

void MakeDecisionThread::setAccount(const QString& accountId)
{
    const QMutexLocker lock(mMutex);

    mAccountId = accountId;
}

void MakeDecisionThread::setKeepMoney(int value)
{
    const QMutexLocker lock(mMutex);

    mKeepMoney = value;
}

QString MakeDecisionThread::accountId() const
{
    const QMutexLocker lock(mMutex);

    return mAccountId;
}

int MakeDecisionThread::keepMoney() const
{
    const QMutexLocker lock(mMutex);

    return mKeepMoney;
}

void MakeDecisionThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}
