#include "src/threads/follow/followthread.h"

#include <QDebug>

#include "src/grpc/utils.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";



FollowThread::FollowThread(
    IUserStorage* userStorage, IInstrumentsStorage* instrumentsStorage, IGrpcClient* grpcClient, QObject* parent
) :
    IFollowThread(parent),
    mUserStorage(userStorage),
    mInstrumentsStorage(instrumentsStorage),
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
    const std::shared_ptr<tinkoff::PortfolioResponse>& portfolio,
    const std::shared_ptr<tinkoff::PortfolioResponse>& anotherPortfolio
)
{
    PortfolioMinItems instruments        = buildInstrumentToCostMap(portfolio);
    PortfolioMinItems anotherInstruments = buildInstrumentToCostMap(anotherPortfolio);

    const double totalCost        = calculateTotalCost(instruments);
    const double anotherTotalCost = calculateTotalCost(anotherInstruments);

    instruments.remove(RUBLE_UID);
    anotherInstruments.remove(RUBLE_UID);

    QMap<QString, double> instrumentsForSale; // Instrument UID => Expected cost
    QMap<QString, double> instrumentsForBuy;  // Instrument UID => Expected cost

    buildInstrumentsForTrading(
        instruments, anotherInstruments, totalCost, anotherTotalCost, instrumentsForSale, instrumentsForBuy
    );

    if (!instrumentsForSale.isEmpty())
    {
        emit tradeInstruments(instrumentsForSale);

        return;
    }

    if (!instrumentsForBuy.isEmpty())
    {
        emit tradeInstruments(instrumentsForBuy);
    }
}

PortfolioMinItems FollowThread::buildInstrumentToCostMap(const std::shared_ptr<tinkoff::PortfolioResponse>& tinkoffPortfolio)
{
    PortfolioMinItems res;

    for (int i = 0; i < tinkoffPortfolio->positions_size(); ++i)
    {
        const tinkoff::PortfolioPosition& position     = tinkoffPortfolio->positions(i);
        const QString                     instrumentId = QString::fromStdString(position.instrument_uid());

        PortfolioMinItem item;

        if (instrumentId == RUBLE_UID)
        {
            item.price = 1.0f;
            item.cost  = quotationToDouble(position.quantity());
        }
        else
        {
            item.price = quotationToFloat(position.current_price());
            item.cost  = quotationToDouble(position.quantity()) * quotationToFloat(position.average_position_price_fifo());
        }

        res[instrumentId] = item;
    }

    return res;
}

double FollowThread::calculateTotalCost(const PortfolioMinItems& instruments)
{
    double res = 0.0;

    for (auto it = instruments.constBegin(); it != instruments.constEnd(); ++it)
    {
        res += it.value().cost;
    }

    return res;
}

void FollowThread::buildInstrumentsForTrading(
    const PortfolioMinItems& instruments,
    const PortfolioMinItems& anotherInstruments,
    double                   totalCost,
    double                   anotherTotalCost,
    QMap<QString, double>&   instrumentsForSale,
    QMap<QString, double>&   instrumentsForBuy
)
{
    for (auto it = anotherInstruments.constBegin(); it != anotherInstruments.constEnd(); ++it)
    {
        const QString& instrumentId = it.key();

        const double anotherPart  = it.value().cost / anotherTotalCost;
        const double expectedCost = anotherPart * totalCost;

        if (!instruments.contains(instrumentId))
        {
            instrumentsForBuy[instrumentId] = expectedCost;

            continue;
        }

        mInstrumentsStorage->getMutex()->lock();
        const Instruments& instrumentsData = mInstrumentsStorage->getInstruments();
        Q_ASSERT_X(
            instrumentsData.contains(instrumentId), "FollowThread::buildInstrumentsForBuyMap()", "Data about instrument not found"
        );
        const qint32 lot = instrumentsData.value(instrumentId).lot;
        mInstrumentsStorage->getMutex()->unlock();

        const PortfolioMinItem& item  = instruments[instrumentId];
        const double            delta = expectedCost - item.cost;
        const double            lotPrice = item.price * lot;

        if (delta < -lotPrice)
        {
            instrumentsForSale[instrumentId] = expectedCost;
        }
        else if (delta > lotPrice)
        {
            instrumentsForBuy[instrumentId] = expectedCost;
        }
    }

    for (auto it = instruments.constBegin(); it != instruments.constEnd(); ++it)
    {
        const QString& instrumentId = it.key();

        if (!anotherInstruments.contains(instrumentId))
        {
            instrumentsForSale[instrumentId] = 0; // Need to sell all
        }
    }
}
