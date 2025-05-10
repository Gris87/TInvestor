#include "src/threads/operations/operationsthread.h"

#include <QDebug>



OperationsThread::OperationsThread(IUserStorage* userStorage, IGrpcClient* grpcClient, QObject* parent) :
    IOperationsThread(parent),
    mUserStorage(userStorage),
    mGrpcClient(grpcClient),
    mAccountId()
{
    qDebug() << "Create OperationsThread";
}

OperationsThread::~OperationsThread()
{
    qDebug() << "Destroy OperationsThread";
}

void OperationsThread::run()
{
    qDebug() << "Running OperationsThread";

    if (mAccountId != "")
    {
        requestOperations();
    }
    else
    {
        emit accountNotFound();
    }

    qDebug() << "Finish OperationsThread";
}

void OperationsThread::setAccount(const QString& account)
{
    const QMutexLocker lock(mUserStorage->getMutex());
    Accounts           accounts = mUserStorage->getAccounts();

    mAccountId = accounts[account].id;
}

void OperationsThread::requestOperations()
{
    const std::shared_ptr<tinkoff::OperationsResponse> tinkoffOperations =
        mGrpcClient->getOperations(QThread::currentThread(), mAccountId);

    if (tinkoffOperations != nullptr && !QThread::currentThread()->isInterruptionRequested())
    {
        qInfo() << tinkoffOperations->operations_size();
    }
}
