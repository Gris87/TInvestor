#include "src/threads/highliquidity/highliquiditythread.h"

#include <QDebug>



HighLiquidityThread::HighLiquidityThread(QObject* parent) :
    IHighLiquidityThread(parent)
{
    qDebug() << "Create HighLiquidityThread";
}

HighLiquidityThread::~HighLiquidityThread()
{
    qDebug() << "Destroy HighLiquidityThread";
}

void HighLiquidityThread::run()
{
    qDebug() << "Running HighLiquidityThread";

    blockSignals(false);

    qDebug() << "Finish HighLiquidityThread";
}

void HighLiquidityThread::setAccountId(const QString& accountId)
{
    mAccountId = accountId;
}

void HighLiquidityThread::setKeepMoney(int value)
{
    const QWriteLocker lock(mRwMutex);

    mKeepMoney = value;
}

int HighLiquidityThread::keepMoney() const
{
    const QReadLocker lock(mRwMutex);

    return mKeepMoney;
}

void HighLiquidityThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}
