#include "src/threads/portfoliolastprice/portfoliolastpricethread.h"

#include <QDebug>

#include "src/grpc/utils.h"



constexpr qint64 MS_IN_SECOND = 1000LL;
constexpr qint64 SLEEP_DELAY  = 5LL * MS_IN_SECOND; // 5 seconds



PortfolioLastPriceThread::PortfolioLastPriceThread(ITimeUtils* timeUtils, IGrpcClient* grpcClient, QObject* parent) :
    IPortfolioLastPriceThread(parent),
    mRwMutex(new QReadWriteLock()),
    mTimeUtils(timeUtils),
    mGrpcClient(grpcClient),
    mMarketDataStream(),
    mInstrumentIds()
{
    qDebug() << "Create PortfolioLastPriceThread";
}

PortfolioLastPriceThread::~PortfolioLastPriceThread()
{
    qDebug() << "Destroy PortfolioLastPriceThread";

    delete mRwMutex;
}

// NOLINTBEGIN(readability-function-cognitive-complexity)
void PortfolioLastPriceThread::run()
{
    qDebug() << "Running PortfolioLastPriceThread";

    blockSignals(false);

    while (!QThread::currentThread()->isInterruptionRequested())
    {
        if (!mInstrumentIds.isEmpty())
        {
            if (createMarketDataStream())
            {
                while (true)
                {
                    const std::shared_ptr<tinkoff::MarketDataResponse> marketDataResponse =
                        mGrpcClient->readMarketDataStream(mMarketDataStream);

                    if (QThread::currentThread()->isInterruptionRequested() || marketDataResponse == nullptr)
                    {
                        break;
                    }

                    if (marketDataResponse->has_last_price())
                    {
                        const tinkoff::LastPrice& lastPriceResp = marketDataResponse->last_price();

                        const QString instrumentId = QString::fromStdString(lastPriceResp.instrument_uid());
                        const float   price        = quotationToFloat(lastPriceResp.price());

                        emit lastPriceChanged(instrumentId, price);
                    }
                }

                const QWriteLocker lock(mRwMutex);

                mGrpcClient->finishMarketDataStream(mMarketDataStream);
                mMarketDataStream = nullptr;
            }
            else
            {
                break;
            }
        }
        else
        {
            if (mTimeUtils->interruptibleSleep(SLEEP_DELAY, QThread::currentThread()))
            {
                break;
            }
        }
    }

    qDebug() << "Finish PortfolioLastPriceThread";
}
// NOLINTEND(readability-function-cognitive-complexity)

void PortfolioLastPriceThread::portfolioChanged(const Portfolio& portfolio)
{
    QStringList instrumentIds;

    for (const PortfolioCategoryItem& category : portfolio.positions)
    {
        for (const PortfolioItem& item : category.items)
        {
            if (item.showPrices)
            {
                instrumentIds.append(item.instrumentId);
            }
        }
    }

    const QWriteLocker lock(mRwMutex);

    if (mInstrumentIds != instrumentIds)
    {
        mInstrumentIds = instrumentIds;

        if (mMarketDataStream != nullptr)
        {
            mGrpcClient->cancelMarketDataStream(mMarketDataStream);
        }
    }
}

void PortfolioLastPriceThread::terminateThread()
{
    blockSignals(true);

    const QReadLocker lock(mRwMutex);

    if (mMarketDataStream != nullptr)
    {
        mGrpcClient->cancelMarketDataStream(mMarketDataStream);
    }

    requestInterruption();
}

bool PortfolioLastPriceThread::createMarketDataStream()
{
    bool res = false;

    const QWriteLocker lock(mRwMutex);

    if (!QThread::currentThread()->isInterruptionRequested())
    {
        mMarketDataStream = mGrpcClient->createMarketDataStreamForLastPrice(mInstrumentIds);

        res = mMarketDataStream != nullptr;
    }

    return res;
}
