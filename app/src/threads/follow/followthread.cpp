#include "src/threads/follow/followthread.h"

#include <QDebug>

#include "src/grpc/utils.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";

constexpr qint64 MS_IN_SECOND         = 1000LL;
constexpr qint64 SLEEP_BEFORE_REQUEST = 1LL * MS_IN_SECOND; // 1 second



FollowThread::FollowThread(
    IInstrumentsStorage* instrumentsStorage, ITimeUtils* timeUtils, IGrpcClient* grpcClient, QObject* parent
) :
    IFollowThread(parent),
    mRwMutex(new QReadWriteLock()),
    mInstrumentsStorage(instrumentsStorage),
    mTimeUtils(timeUtils),
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

    delete mRwMutex;
}

// NOLINTBEGIN(readability-function-cognitive-complexity)
void FollowThread::run()
{
    qDebug() << "Running FollowThread";

    blockSignals(false);

    std::shared_ptr<tinkoff::PortfolioResponse> portfolio        = getValidPortfolio(mAccountId);
    std::shared_ptr<tinkoff::PortfolioResponse> anotherPortfolio = getValidPortfolio(mAnotherAccountId);

    if (!QThread::currentThread()->isInterruptionRequested() && portfolio != nullptr && anotherPortfolio != nullptr)
    {
        handlePortfolios(portfolio, anotherPortfolio);

        if (createPortfolioStream())
        {
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

                    Q_ASSERT_X(accountId == mAccountId || accountId == mAnotherAccountId, __FUNCTION__, "Unexpected account ID");

                    if (accountId == mAccountId)
                    {
                        portfolio = getValidPortfolio(mAccountId);
                    }
                    else
                    {
                        anotherPortfolio = getValidPortfolio(mAnotherAccountId);
                    }

                    handlePortfolios(portfolio, anotherPortfolio);
                }
            }

            const QWriteLocker lock(mRwMutex);

            mGrpcClient->finishPortfolioStream(mPortfolioStream);
            mPortfolioStream = nullptr;
        }
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

    const QReadLocker lock(mRwMutex);

    if (mPortfolioStream != nullptr)
    {
        mGrpcClient->cancelPortfolioStream(mPortfolioStream);
    }

    requestInterruption();
}

bool FollowThread::createPortfolioStream()
{
    bool res = false;

    const QWriteLocker lock(mRwMutex);

    if (!QThread::currentThread()->isInterruptionRequested())
    {
        mPortfolioStream = mGrpcClient->createPortfolioStream(mAccountId, mAnotherAccountId);

        res = mPortfolioStream != nullptr;
    }

    return res;
}

std::shared_ptr<tinkoff::PortfolioResponse> FollowThread::getValidPortfolio(const QString& accountId)
{
    bool                                        success          = false;
    std::shared_ptr<tinkoff::PortfolioResponse> tinkoffPortfolio = nullptr;

    while (!QThread::currentThread()->isInterruptionRequested() && !success)
    {
        tinkoffPortfolio = mGrpcClient->getPortfolio(QThread::currentThread(), accountId);

        if (!QThread::currentThread()->isInterruptionRequested() && tinkoffPortfolio != nullptr)
        {
            if (validatePortfolioResponse(*tinkoffPortfolio))
            {
                success = true;
            }
            else
            {
                qDebug() << "Invalid portfolio received. Try one more time";

                if (mTimeUtils->interruptibleSleep(SLEEP_BEFORE_REQUEST, QThread::currentThread()))
                {
                    break;
                }
            }
        }
        else
        {
            break;
        }
    }

    return tinkoffPortfolio;
}

bool FollowThread::validatePortfolioResponse(const tinkoff::PortfolioResponse& tinkoffPortfolio)
{
    bool res = true;

    for (int i = 0; i < tinkoffPortfolio.positions_size(); ++i)
    {
        const tinkoff::PortfolioPosition& position = tinkoffPortfolio.positions(i);

        const QString instrumentId = QString::fromStdString(position.instrument_uid());

        if (instrumentId != RUBLE_UID)
        {
            if (position.average_position_price_fifo().units() <= 0 && position.average_position_price_fifo().nano() <= 0)
            {
                res = false;

                break;
            }
        }
    }

    return res;
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

    InstrumentsForTrading instrumentsForSale; // Instrument UID => TradingInfo
    InstrumentsForTrading instrumentsForBuy;  // Instrument UID => TradingInfo

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
    InstrumentsForTrading&   instrumentsForSale,
    InstrumentsForTrading&   instrumentsForBuy
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
                ASAP_MODE_FOLLOW_PRICE,
                -1.0f,
                it.value().price,
                expectedCost,
                tr("Decided to buy up to cost %1 due to following account \"%2\"")
                    .arg(QString::number(expectedCost, 'f', 2) + " \u20BD", mAnotherAccountName)
            );

            continue;
        }

        const qint32 lot = getInstrumentLot(instrumentId);

        const PortfolioMinItem& item     = instruments[instrumentId];
        const double            delta    = expectedCost - item.cost;
        const double            lotPrice = item.price * lot;

        if (delta < -lotPrice)
        {
            instrumentsForSale[instrumentId] = TradingInfo(
                ASAP_MODE_FOLLOW_PRICE,
                -1.0f,
                item.price,
                expectedCost,
                tr("Decided to sell up to cost %1 due to following account \"%2\"")
                    .arg(QString::number(expectedCost, 'f', 2) + " \u20BD", mAnotherAccountName)
            );
        }
        else if (delta > lotPrice)
        {
            instrumentsForBuy[instrumentId] = TradingInfo(
                ASAP_MODE_FOLLOW_PRICE,
                -1.0f,
                item.price,
                expectedCost,
                tr("Decided to buy up to cost %1 due to following account \"%2\"")
                    .arg(QString::number(expectedCost, 'f', 2) + " \u20BD", mAnotherAccountName)
            );
        }
    }

    for (auto it = instruments.constBegin(); it != instruments.constEnd(); ++it)
    {
        const QString& instrumentId = it.key();

        if (!anotherInstruments.contains(instrumentId))
        {
            instrumentsForSale[instrumentId] = TradingInfo(
                ASAP_MODE_FOLLOW_PRICE,
                -1.0f,
                it.value().price,
                0, // Need to sell all
                tr("Decided to sell up to cost %1 due to following account \"%2\"").arg("0.00 \u20BD", mAnotherAccountName)
            );
        }
    }
}

qint32 FollowThread::getInstrumentLot(const QString& instrumentId) const
{
    mInstrumentsStorage->readLock();

    const Instruments& instruments = mInstrumentsStorage->getInstruments();
    Q_ASSERT_X(instruments.contains(instrumentId), __FUNCTION__, "Data about instrument not found");
    const qint32 res = instruments.value(instrumentId).lot;

    mInstrumentsStorage->readUnlock();

    return res;
}
