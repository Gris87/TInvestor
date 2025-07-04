#include "src/threads/portfoliolastprice/portfoliolastpricethread.h"

#include <QDebug>

#include "src/grpc/utils.h"



constexpr qint64 MS_IN_SECOND = 1000LL;
constexpr qint64 SLEEP_DELAY  = 5LL * MS_IN_SECOND; // 5 seconds



PortfolioLastPriceThread::PortfolioLastPriceThread(ITimeUtils* timeUtils, IGrpcClient* grpcClient, QObject* parent) :
    IPortfolioLastPriceThread(parent),
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
            createMarketDataStream();

            bool needToTerminate = !mGrpcClient->subscribeLastPrices(mMarketDataStream, mInstrumentIds);

            if (!needToTerminate)
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
            }

            mGrpcClient->finishMarketDataStream(mMarketDataStream);
            mMarketDataStream = nullptr;

            if (needToTerminate)
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

    if (mInstrumentIds != instrumentIds)
    {
        mInstrumentIds = instrumentIds;

        if (mMarketDataStream != nullptr)
        {
            if (!mInstrumentIds.isEmpty())
            {
                if (mGrpcClient->unsubscribeLastPrices(mMarketDataStream))
                {
                    mGrpcClient->subscribeLastPrices(mMarketDataStream, mInstrumentIds);
                }
            }
            else
            {
                mGrpcClient->closeWriteMarketDataStream(mMarketDataStream);
                mGrpcClient->cancelMarketDataStream(mMarketDataStream);
            }
        }
    }
}

void PortfolioLastPriceThread::terminateThread()
{
    blockSignals(true);

    if (mMarketDataStream != nullptr)
    {
        mGrpcClient->closeWriteMarketDataStream(mMarketDataStream);
        mGrpcClient->cancelMarketDataStream(mMarketDataStream);
    }

    requestInterruption();
}

void PortfolioLastPriceThread::createMarketDataStream()
{
    mMarketDataStream = mGrpcClient->createMarketDataStream();
}
