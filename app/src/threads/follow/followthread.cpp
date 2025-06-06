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
        std::shared_ptr<tinkoff::PortfolioResponse> portfolio = mGrpcClient->getPortfolio(QThread::currentThread(), mAccountId);
        std::shared_ptr<tinkoff::PortfolioResponse> anotherPortfolio =
            mGrpcClient->getPortfolio(QThread::currentThread(), mAnotherAccountId);

        if (!QThread::currentThread()->isInterruptionRequested() && portfolio != nullptr && anotherPortfolio != nullptr)
        {
            handlePortfolios(portfolio, anotherPortfolio);

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
                    tinkoff::PortfolioResponse tinkoffPortfolio = portfolioStreamResponse->portfolio();
                    QString                    accountId        = QString::fromStdString(tinkoffPortfolio.account_id());

                    Q_ASSERT_X(
                        accountId == mAccountId || accountId == mAnotherAccountId, "FollowThread::run()", "Unexpected account ID"
                    );

                    if (accountId == mAccountId)
                    {
                        *portfolio = tinkoffPortfolio;
                    }
                    else
                    {
                        *anotherPortfolio = tinkoffPortfolio;
                    }

                    handlePortfolios(portfolio, anotherPortfolio);
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
    mPortfolioStream = mGrpcClient->createPortfolioStream(mAccountId, mAnotherAccountId);
}

void FollowThread::handlePortfolios(
    std::shared_ptr<tinkoff::PortfolioResponse> portfolio, std::shared_ptr<tinkoff::PortfolioResponse> anotherPortfolio
)
{
    qInfo() << portfolio->account_id() << anotherPortfolio->account_id();
}
