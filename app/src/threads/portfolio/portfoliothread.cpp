#include "src/threads/portfolio/portfoliothread.h"

#include <QDebug>



PortfolioThread::PortfolioThread(IUserStorage* userStorage, IGrpcClient* grpcClient, QObject* parent) :
    IPortfolioThread(parent),
    mUserStorage(userStorage),
    mGrpcClient(grpcClient),
    mAccountId(),
    mPortfolioStream()
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
        const std::shared_ptr<tinkoff::PortfolioResponse> tinkoffPortfolio =
            mGrpcClient->getPortfolio(QThread::currentThread(), mAccountId);

        if (!QThread::currentThread()->isInterruptionRequested() && tinkoffPortfolio != nullptr)
        {
            handlePortfolioResponse(*tinkoffPortfolio);

            createPortfolioStream();

            while (true)
            {
                const std::shared_ptr<tinkoff::PortfolioStreamResponse> portfolioStreamResponse =
                    mGrpcClient->readPortfolioStream(mPortfolioStream);

                if (QThread::currentThread()->isInterruptionRequested() || portfolioStreamResponse == nullptr)
                {
                    break;
                }

                if (portfolioStreamResponse->has_portfolio())
                {
                    handlePortfolioResponse(portfolioStreamResponse->portfolio());
                }
            }

            mGrpcClient->finishPortfolioStream(mPortfolioStream);
            mPortfolioStream = nullptr;
        }
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
    if (mPortfolioStream != nullptr)
    {
        mGrpcClient->cancelPortfolioStream(mPortfolioStream);
    }

    requestInterruption();
}

void PortfolioThread::createPortfolioStream()
{
    mPortfolioStream = mGrpcClient->createPortfolioStream(mAccountId);
}

void PortfolioThread::handlePortfolioResponse(const tinkoff::PortfolioResponse& tinkoffPortfolio)
{
    qInfo() << tinkoffPortfolio.positions_size();
}
