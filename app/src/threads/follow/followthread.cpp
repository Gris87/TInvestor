#include "src/threads/follow/followthread.h"

#include <QDebug>

#include "src/grpc/utils.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";



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
    QMap<QString, double> instruments        = buildInstrumentToCostMap(portfolio);
    QMap<QString, double> anotherInstruments = buildInstrumentToCostMap(anotherPortfolio);

    double totalCost        = calculateTotalCost(instruments);
    double anotherTotalCost = calculateTotalCost(anotherInstruments);

    QMap<QString, double> instrumentsForSale =
        buildInstrumentsForSaleMap(portfolio, anotherPortfolio, totalCost, anotherTotalCost);

    if (!instrumentsForSale.isEmpty())
    {
        emit tradeInstruments(instrumentsForSale);

        return;
    }

    QMap<QString, double> instrumentsForBuy = buildInstrumentsForBuyMap(portfolio, anotherPortfolio, totalCost, anotherTotalCost);

    if (!instrumentsForBuy.isEmpty())
    {
        emit tradeInstruments(instrumentsForBuy);
    }
}

QMap<QString, double> FollowThread::buildInstrumentToCostMap(std::shared_ptr<tinkoff::PortfolioResponse> tinkoffPortfolio)
{
    QMap<QString, double> res;

    for (int i = 0; i < tinkoffPortfolio->positions_size(); ++i)
    {
        const tinkoff::PortfolioPosition& position     = tinkoffPortfolio->positions(i);
        const QString                     instrumentId = QString::fromStdString(position.instrument_uid());

        if (instrumentId == RUBLE_UID)
        {
            res[instrumentId] = quotationToDouble(position.quantity());
        }
        else
        {
            res[instrumentId] = quotationToDouble(position.quantity()) * quotationToFloat(position.average_position_price_fifo());
        }
    }

    return res;
}

double FollowThread::calculateTotalCost(const QMap<QString, double>& instruments)
{
    double res = 0.0;

    for (auto it = instruments.constBegin(); it != instruments.constEnd(); ++it)
    {
        res += it.value();
    }

    return res;
}

QMap<QString, double> FollowThread::buildInstrumentsForSaleMap(
    std::shared_ptr<tinkoff::PortfolioResponse> portfolio,
    std::shared_ptr<tinkoff::PortfolioResponse> anotherPortfolio,
    double                                      totalCost,
    double                                      anotherTotalCost
)
{
    QMap<QString, double> res;

    qInfo() << portfolio->account_id() << anotherPortfolio->account_id() << totalCost << anotherTotalCost;

    return res;
}

QMap<QString, double> FollowThread::buildInstrumentsForBuyMap(
    std::shared_ptr<tinkoff::PortfolioResponse> portfolio,
    std::shared_ptr<tinkoff::PortfolioResponse> anotherPortfolio,
    double                                      totalCost,
    double                                      anotherTotalCost
)
{
    QMap<QString, double> res;

    qInfo() << portfolio->account_id() << anotherPortfolio->account_id() << totalCost << anotherTotalCost;

    return res;
}
