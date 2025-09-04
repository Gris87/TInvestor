#include "src/threads/hugespread/hugespreadthread.h"

#include <QDebug>



HugeSpreadThread::HugeSpreadThread(QObject* parent) :
    IHugeSpreadThread(parent)
{
    qDebug() << "Create HugeSpreadThread";
}

HugeSpreadThread::~HugeSpreadThread()
{
    qDebug() << "Destroy HugeSpreadThread";
}

void HugeSpreadThread::run()
{
    qDebug() << "Running HugeSpreadThread";

    blockSignals(false);

    qDebug() << "Finish HugeSpreadThread";
}

void HugeSpreadThread::setAccountId(const QString& accountId)
{
    mAccountId = accountId;
}

void HugeSpreadThread::setKeepMoney(int value)
{
    const QWriteLocker lock(mRwMutex);

    mKeepMoney = value;
}

int HugeSpreadThread::keepMoney() const
{
    const QReadLocker lock(mRwMutex);

    return mKeepMoney;
}

void HugeSpreadThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}
