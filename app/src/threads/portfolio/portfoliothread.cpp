#include "src/threads/portfolio/portfoliothread.h"

#include <QDebug>



PortfolioThread::PortfolioThread(IUserStorage* userStorage, IGrpcClient* grpcClient, QObject* parent) :
    IPortfolioThread(parent),
    mUserStorage(userStorage),
    mGrpcClient(grpcClient),
    mAccountId()
{
    qDebug() << "Create PortfolioThread";
}

PortfolioThread::~PortfolioThread()
{
    qDebug() << "Destroy PortfolioThread";
}

void PortfolioThread::run()
{
    qDebug() << "Running PortfolioThread";

    if (mAccountId != "")
    {
        qInfo() << mAccountId; // TODO: Implement
    }
    else
    {
        emit accountNotFound();
    }

    qDebug() << "Finish PortfolioThread";
}

void PortfolioThread::setAccount(const QString& account)
{
    const QMutexLocker lock(mUserStorage->getMutex());
    Accounts           accounts = mUserStorage->getAccounts();

    mAccountId = accounts[account].id;
}

void PortfolioThread::terminateThread()
{
    requestInterruption();
}
