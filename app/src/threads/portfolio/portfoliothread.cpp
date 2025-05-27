#include "src/threads/portfolio/portfoliothread.h"

#include <QDebug>

#include "src/grpc/utils.h"



const char* const RUBLE_UID       = "a92e2e25-a698-45cc-a781-167cf465257c";
constexpr float   HUNDRED_PERCENT = 100.0f;



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
    Portfolio portfolio;

    double totalCost = 0.0;

    for (int i = 0; i < tinkoffPortfolio.positions_size(); ++i)
    {
        const tinkoff::PortfolioPosition& position = tinkoffPortfolio.positions(i);

        const QString instrumentId   = QString::fromStdString(position.instrument_uid());
        const QString instrumentType = QString::fromStdString(position.instrument_type());

        PortfolioItem item;

        item.showPrices = instrumentId != RUBLE_UID;

        item.quantity           = quotationToDouble(position.quantity());
        item.price              = item.showPrices ? quotationToFloat(position.current_price()) : 1.0f;
        item.avgPrice           = item.showPrices ? quotationToFloat(position.average_position_price()) : 1.0f;
        item.cost               = item.quantity * item.avgPrice;
        item.yield              = (item.quantity * item.price) - item.cost;
        item.yieldPercent       = (item.yield / item.cost) * HUNDRED_PERCENT;
        item.priceForDailyYield = item.price - (quotationToFloat(position.daily_yield()) / item.quantity);
        item.dailyYieldPercent  = ((item.price / item.priceForDailyYield) * HUNDRED_PERCENT) - HUNDRED_PERCENT;

        totalCost += item.cost;

        portfolio.positions[instrumentType][instrumentId] = item;
    }

    for (auto it = portfolio.positions.begin(), end = portfolio.positions.end(); it != end; ++it)
    {
        PortfolioItems& items = it.value();
        PortfolioItem   categoryItem;

        for (auto it2 = items.begin(), end = items.end(); it2 != end; ++it2)
        {
            it2->part = (it2->cost / totalCost) * HUNDRED_PERCENT;

            categoryItem.cost += it2->cost;
        }

        categoryItem.part = (categoryItem.cost / totalCost) * HUNDRED_PERCENT;

        items["total"] = categoryItem;
    }

    emit portfolioChanged(portfolio);
}
