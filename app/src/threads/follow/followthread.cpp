#include "src/threads/follow/followthread.h"

#include <QDebug>

#include "src/grpc/utils.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";



FollowThread::FollowThread(IInstrumentsStorage* instrumentsStorage, IGrpcClient* grpcClient, QObject* parent) :
    IFollowThread(parent),
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

// NOLINTBEGIN(readability-function-cognitive-complexity)
void FollowThread::run()
{
    qDebug() << "Running FollowThread";

    blockSignals(false);

    const std::shared_ptr<tinkoff::PortfolioResponse> portfolio = mGrpcClient->getPortfolio(QThread::currentThread(), mAccountId);
    const std::shared_ptr<tinkoff::PortfolioResponse> anotherPortfolio =
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
                const tinkoff::PortfolioResponse tinkoffPortfolio = portfolioStreamResponse->portfolio();
                const QString                    accountId        = QString::fromStdString(tinkoffPortfolio.account_id());

                Q_ASSERT_X(accountId == mAccountId || accountId == mAnotherAccountId, __FUNCTION__ "()", "Unexpected account ID");

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

    qDebug() << "Finish FollowThread";
}
// NOLINTEND(readability-function-cognitive-complexity)

void FollowThread::setAccounts(const QString& accountId, const QString& anotherAccountId, const QString& anotherAccountName)
{
    mAccountId          = accountId;
    mAnotherAccountId   = anotherAccountId;
    mAnotherAccountName = anotherAccountName;
}

void FollowThread::terminateThread()
{
    blockSignals(true);

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

    QMap<QString, TradingInfo> instrumentsForSale; // Instrument UID => TradingInfo
    QMap<QString, TradingInfo> instrumentsForBuy;  // Instrument UID => TradingInfo

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
    const PortfolioMinItems&    instruments,
    const PortfolioMinItems&    anotherInstruments,
    double                      totalCost,
    double                      anotherTotalCost,
    QMap<QString, TradingInfo>& instrumentsForSale,
    QMap<QString, TradingInfo>& instrumentsForBuy
)
{
    for (auto it = anotherInstruments.constBegin(); it != anotherInstruments.constEnd(); ++it)
    {
        const QString& instrumentId = it.key();

        const double anotherPart  = it.value().cost / anotherTotalCost;
        const double expectedCost = anotherPart * totalCost;

        if (!instruments.contains(instrumentId))
        {
            instrumentsForBuy[instrumentId] = TradingInfo(
                expectedCost,
                tr("Decided to buy up to cost %1 %2 due to following account \"%3\"")
                    .arg(QString::number(expectedCost, 'f', 2), "\u20BD", mAnotherAccountName)
            );

            continue;
        }

        mInstrumentsStorage->lock();
        const Instruments& instrumentsData = mInstrumentsStorage->getInstruments();
        Q_ASSERT_X(instrumentsData.contains(instrumentId), __FUNCTION__ "()", "Data about instrument not found");
        const qint32 lot = instrumentsData.value(instrumentId).lot;
        mInstrumentsStorage->unlock();

        const PortfolioMinItem& item     = instruments[instrumentId];
        const double            delta    = expectedCost - item.cost;
        const double            lotPrice = item.price * lot;

        if (delta < -lotPrice)
        {
            instrumentsForSale[instrumentId] = TradingInfo(
                expectedCost,
                tr("Decided to sell up to cost %1 %2 due to following account \"%3\"")
                    .arg(QString::number(expectedCost, 'f', 2), "\u20BD", mAnotherAccountName)
            );
        }
        else if (delta > lotPrice)
        {
            instrumentsForBuy[instrumentId] = TradingInfo(
                expectedCost,
                tr("Decided to buy up to cost %1 %2 due to following account \"%3\"")
                    .arg(QString::number(expectedCost, 'f', 2), "\u20BD", mAnotherAccountName)
            );
        }
    }

    for (auto it = instruments.constBegin(); it != instruments.constEnd(); ++it)
    {
        const QString& instrumentId = it.key();

        if (!anotherInstruments.contains(instrumentId))
        {
            instrumentsForSale[instrumentId] = TradingInfo(
                0, // Need to sell all
                tr("Decided to sell up to cost %1 %2 due to following account \"%3\"").arg("0.00", "\u20BD", mAnotherAccountName)
            );
        }
    }
}
