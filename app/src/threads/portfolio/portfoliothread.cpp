#include "src/threads/portfolio/portfoliothread.h"

#include <QDebug>



PortfolioThread::PortfolioThread(IUserStorage* userStorage, QObject* parent) :
    IPortfolioThread(parent),
    mUserStorage(userStorage),
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

    qInfo() << mAccountId;

    qDebug() << "Finish PortfolioThread";
}

void PortfolioThread::setAccount(const QString& account)
{
    const QMutexLocker lock(mUserStorage->getMutex());
    Accounts           accounts = mUserStorage->getAccounts();

    mAccountId = accounts[account].id;
}
