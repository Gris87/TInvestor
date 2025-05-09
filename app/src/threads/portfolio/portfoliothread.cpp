#include "src/threads/portfolio/portfoliothread.h"

#include <QDebug>



PortfolioThread::PortfolioThread(IUserStorage* userStorage, QObject* parent) :
    IPortfolioThread(parent),
    mUserStorage(userStorage)
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

    qDebug() << "Finish PortfolioThread";
}
