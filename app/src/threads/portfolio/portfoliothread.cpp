#include "src/threads/portfolio/portfoliothread.h"

#include <QDebug>

#include "src/grpc/utils.h"



const char* const RUBLE_UID       = "a92e2e25-a698-45cc-a781-167cf465257c";
constexpr float   HUNDRED_PERCENT = 100.0f;



PortfolioThread::PortfolioThread(
    IInstrumentsStorage* instrumentsStorage, ILogosStorage* logosStorage, IGrpcClient* grpcClient, QObject* parent
) :
    IPortfolioThread(parent),
    mInstrumentsStorage(instrumentsStorage),
    mLogosStorage(logosStorage),
    mGrpcClient(grpcClient),
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
}

void PortfolioThread::run()
{
    qDebug() << "Running PortfolioThread";

    blockSignals(false);

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

    qDebug() << "Finish PortfolioThread";
}

void PortfolioThread::setAccountId(const QString& accountId)
{
    mAccountId = accountId;
}

void PortfolioThread::terminateThread()
{
    blockSignals(true);

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

    double                              totalCost = 0.0;
    QMap<QString, QList<PortfolioItem>> categories; // Instrument type => category

    mInstrumentsStorage->readLock();
    mLogosStorage->readLock();

    for (int i = 0; i < tinkoffPortfolio.positions_size(); ++i)
    {
        const tinkoff::PortfolioPosition& position = tinkoffPortfolio.positions(i);

        const QString instrumentId   = QString::fromStdString(position.instrument_uid());
        const QString instrumentType = QString::fromStdString(position.instrument_type());

        PortfolioItem item;

        Instrument instrument = mInstrumentsStorage->getInstruments().value(instrumentId);

        if (instrument.ticker == "" || instrument.name == "")
        {
            instrument.ticker         = instrumentId;
            instrument.name           = "?????";
            instrument.pricePrecision = 2;
        }

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
