#include "src/threads/portfolio/portfoliothread.h"

#include <QDebug>

#include "src/grpc/utils.h"



const char* const RUBLE_UID            = "a92e2e25-a698-45cc-a781-167cf465257c";
constexpr float   HUNDRED_PERCENT      = 100.0f;
constexpr qint64  MS_IN_SECOND         = 1000LL;
constexpr qint64  ONE_MINUTE           = 60LL * MS_IN_SECOND;
constexpr qint64  ONE_HOUR             = 60LL * ONE_MINUTE;
constexpr qint64  RECONNECT_INTERVAL   = 1LL * ONE_HOUR;      // 1 hour
constexpr qint64  SLEEP_DELAY          = 5LL * MS_IN_SECOND;  // 5 seconds
constexpr qint64  SLEEP_BEFORE_REQUEST = 10LL * MS_IN_SECOND; // 10 seconds



PortfolioThread::PortfolioThread(
    IInstrumentsStorage* instrumentsStorage,
    ILogosStorage*       logosStorage,
    ITimeUtils*          timeUtils,
    IGrpcClient*         grpcClient,
    IGrpcRetryClient*    grpcRetryClient,
    QObject*             parent
) :
    IPortfolioThread(parent),
    mRwMutex(new QReadWriteLock()),
    mInstrumentsStorage(instrumentsStorage),
    mLogosStorage(logosStorage),
    mTimeUtils(timeUtils),
    mGrpcClient(grpcClient),
    mGrpcRetryClient(grpcRetryClient),
    mAccountId(),
    mPortfolioStream(),
    mSortedCategories(),
    mCategoryNames()
{
    qDebug() << "Create PortfolioThread";

    mSortedCategories << "currency" << "share" << "etf" << "bond" << "futures" << "options";

    mCategoryNames["currency"] = tr("Currency and metals");
    mCategoryNames["share"]    = tr("Share");
    mCategoryNames["etf"]      = tr("ETF");
    mCategoryNames["bond"]     = tr("Bond");
    mCategoryNames["futures"]  = tr("Futures");
    mCategoryNames["options"]  = tr("Options");
}

PortfolioThread::~PortfolioThread()
{
    qDebug() << "Destroy PortfolioThread";

    delete mRwMutex;
}

// NOLINTBEGIN(readability-function-cognitive-complexity)
void PortfolioThread::run()
{
    qDebug() << "Running PortfolioThread";

    blockSignals(false);

    while (!QThread::currentThread()->isInterruptionRequested())
    {
        if (createPortfolioStream())
        {
            if (requestPortfolio())
            {
                const qint64 startStreamTimestamp = QDateTime::currentMSecsSinceEpoch();

                while (QDateTime::currentMSecsSinceEpoch() - startStreamTimestamp < RECONNECT_INTERVAL)
                {
                    const std::shared_ptr<tinkoff::PortfolioStreamResponse> portfolioStreamResponse =
                        mGrpcClient->readPortfolioStream(mPortfolioStream);

                    if (QThread::currentThread()->isInterruptionRequested() || portfolioStreamResponse == nullptr)
                    {
                        mTimeUtils->interruptibleSleep(SLEEP_DELAY, QThread::currentThread());

                        break;
                    }

                    if (portfolioStreamResponse->has_portfolio())
                    {
                        if (mTimeUtils->interruptibleSleep(SLEEP_BEFORE_REQUEST, QThread::currentThread()))
                        {
                            break;
                        }

                        requestPortfolio();
                    }
                }
            }
            else
            {
                mTimeUtils->interruptibleSleep(SLEEP_DELAY, QThread::currentThread());
            }

            const QWriteLocker lock(mRwMutex);

            mGrpcClient->finishPortfolioStream(mPortfolioStream);
            mPortfolioStream = nullptr;
        }
        else
        {
            if (mTimeUtils->interruptibleSleep(SLEEP_DELAY, QThread::currentThread()))
            {
                break;
            }
        }
    }

    qDebug() << "Finish PortfolioThread";
}
// NOLINTEND(readability-function-cognitive-complexity)

void PortfolioThread::setAccountId(const QString& accountId)
{
    mAccountId = accountId;
}

void PortfolioThread::terminateThread()
{
    blockSignals(true);

    const QReadLocker lock(mRwMutex);

    if (mPortfolioStream != nullptr)
    {
        mGrpcClient->cancelPortfolioStream(mPortfolioStream);
    }

    requestInterruption();
}

bool PortfolioThread::createPortfolioStream()
{
    bool res = false;

    const QWriteLocker lock(mRwMutex);

    if (!QThread::currentThread()->isInterruptionRequested())
    {
        mPortfolioStream = mGrpcClient->createPortfolioStream(mAccountId);

        res = mPortfolioStream != nullptr;
    }

    return res;
}

bool PortfolioThread::requestPortfolio()
{
    const std::shared_ptr<tinkoff::PortfolioResponse> tinkoffPortfolio =
        mGrpcRetryClient->getValidPortfolio(QThread::currentThread(), mAccountId);

    if (!QThread::currentThread()->isInterruptionRequested() && tinkoffPortfolio != nullptr)
    {
        handlePortfolioResponse(*tinkoffPortfolio);

        return true;
    }

    return false;
}

void PortfolioThread::handlePortfolioResponse(const tinkoff::PortfolioResponse& tinkoffPortfolio)
{
    Portfolio portfolio;

    double                              totalCost = 0.0;
    QMap<QString, QList<PortfolioItem>> categories; // Instrument type => category

    mInstrumentsStorage->readLock();
    mLogosStorage->readLock();

    const Instruments& instruments = mInstrumentsStorage->getInstruments();

    for (int i = 0; i < tinkoffPortfolio.positions_size(); ++i)
    {
        const tinkoff::PortfolioPosition& position = tinkoffPortfolio.positions(i);

        const QString instrumentId   = QString::fromStdString(position.instrument_uid());
        const QString instrumentType = QString::fromStdString(position.instrument_type());

        PortfolioItem item;

        Instrument instrument = instruments.value(instrumentId);
        instrument.resetIfNotFound(instrumentId);

        item.instrumentId       = instrumentId;
        item.instrumentLogo     = mLogosStorage->getLogo(instrumentId);
        item.instrumentTicker   = instrument.ticker;
        item.instrumentName     = instrument.name;
        item.showPrices         = instrumentId != RUBLE_UID;
        item.available          = quotationToDouble(position.quantity());
        item.price              = item.showPrices ? quotationToFloat(position.current_price()) : 1.0f;
        item.avgPriceFifo       = item.showPrices ? quotationToFloat(position.average_position_price_fifo()) : 1.0f;
        item.avgPriceWavg       = item.showPrices ? quotationToFloat(position.average_position_price()) : 1.0f;
        item.cost               = item.available * item.avgPriceFifo;
        item.yield              = (item.available * item.price) - item.cost;
        item.yieldPercent       = (item.yield / item.cost) * HUNDRED_PERCENT;
        item.dailyYield         = quotationToFloat(position.daily_yield());
        item.priceForDailyYield = item.price - (item.dailyYield / item.available);
        item.costForDailyYield  = item.available * item.priceForDailyYield;
        item.dailyYieldPercent  = ((item.price / item.priceForDailyYield) * HUNDRED_PERCENT) - HUNDRED_PERCENT;
        item.pricePrecision     = instrument.pricePrecision;

        totalCost += item.cost;

        categories[instrumentType].append(item);
    }

    mLogosStorage->readUnlock();
    mInstrumentsStorage->readUnlock();

    for (const QString& category : std::as_const(mSortedCategories))
    {
        if (!categories.contains(category))
        {
            continue;
        }

        QList<PortfolioItem>& items = categories[category];

        PortfolioCategoryItem categoryItem;

        categoryItem.id = portfolio.positions.size();

        Q_ASSERT_X(mCategoryNames.contains(category), __FUNCTION__, "Missing translation");
        categoryItem.name = mCategoryNames.value(category, "UNKNOWN");

        for (PortfolioItem& item : items)
        {
            item.part = (item.cost / totalCost) * HUNDRED_PERCENT;

            categoryItem.cost += item.cost;
        }

        categoryItem.part  = (categoryItem.cost / totalCost) * HUNDRED_PERCENT;
        categoryItem.items = items;

        portfolio.positions.append(categoryItem);
    }

    emit portfolioChanged(portfolio);
}
