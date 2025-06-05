#include "src/threads/follow/followthread.h"

#include <QDebug>



FollowThread::FollowThread(IUserStorage* userStorage, IGrpcClient* grpcClient, QObject* parent) :
    IFollowThread(parent),
    mUserStorage(userStorage),
    mGrpcClient(grpcClient),
    mAccountId(),
    mAnotherAccountId(),
    mPortfolioStream()
{
    qDebug() << "Create FollowThread";
}

FollowThread::~FollowThread()
{
    qDebug() << "Destroy FollowThread";
}

void FollowThread::run()
{
    qDebug() << "Running FollowThread";

    if (mAccountId != "" || mAnotherAccountId != "")
    {
        const std::shared_ptr<tinkoff::PortfolioResponse> tinkoffPortfolio =
            mGrpcClient->getPortfolio(QThread::currentThread(), mAnotherAccountId);

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

    qDebug() << "Finish FollowThread";
}

void FollowThread::setAccounts(const QString& account, const QString& anotherAccount)
{
    const QMutexLocker lock(mUserStorage->getMutex());
    const Accounts     accounts = mUserStorage->getAccounts();

    mAccountId        = accounts.value(account).id;
    mAnotherAccountId = accounts.value(anotherAccount).id;
}

void FollowThread::terminateThread()
{
    if (mPortfolioStream != nullptr)
    {
        mGrpcClient->cancelPortfolioStream(mPortfolioStream);
    }

    requestInterruption();
}

void FollowThread::createPortfolioStream()
{
    mPortfolioStream = mGrpcClient->createPortfolioStream(mAnotherAccountId);
}

void FollowThread::handlePortfolioResponse(const tinkoff::PortfolioResponse& /*tinkoffPortfolio*/)
{
    qInfo() << "TBD";
}
