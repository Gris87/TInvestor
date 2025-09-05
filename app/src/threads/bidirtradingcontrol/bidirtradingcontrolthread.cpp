#include "src/threads/bidirtradingcontrol/bidirtradingcontrolthread.h"

#include <QDebug>



BiDirTradingControlThread::BiDirTradingControlThread(QObject* parent) :
    IBiDirTradingControlThread(parent),
    mRwMutex(new QReadWriteLock()),
    mAccountId(),
    mKeepMoney()
{
    qDebug() << "Create BiDirTradingControlThread";
}

BiDirTradingControlThread::~BiDirTradingControlThread()
{
    qDebug() << "Destroy BiDirTradingControlThread";

    delete mRwMutex;
}

void BiDirTradingControlThread::run()
{
    qDebug() << "Running BiDirTradingControlThread";

    blockSignals(false);

    qDebug() << "Finish BiDirTradingControlThread";
}

void BiDirTradingControlThread::setAccountId(const QString& accountId)
{
    mAccountId = accountId;
}

void BiDirTradingControlThread::setKeepMoney(int value)
{
    const QWriteLocker lock(mRwMutex);

    mKeepMoney = value;
}

int BiDirTradingControlThread::keepMoney() const
{
    const QReadLocker lock(mRwMutex);

    return mKeepMoney;
}

void BiDirTradingControlThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}
