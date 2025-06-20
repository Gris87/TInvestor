#include "src/threads/portfoliolastprice/portfoliolastpricethread.h"

#include <QDebug>
#include <QMutexLocker>

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

void PortfolioLastPriceThread::run()
{
    qDebug() << "Running PortfolioLastPriceThread";

    blockSignals(false);

    while (!QThread::currentThread()->isInterruptionRequested())
    {
        if (!mInstrumentIds.isEmpty())
        {
            createMarketDataStream();

            if (mGrpcClient->subscribeLastPrices(mMarketDataStream, mInstrumentIds))
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

void PortfolioLastPriceThread::portfolioChanged(const Portfolio& portfolio)
{
    QStringList instrumentIds;

    for (auto it = portfolio.positionsMap.constBegin(), end = portfolio.positionsMap.constEnd(); it != end; ++it)
    {
        const PortfolioItems& portfolioItems = it.value();

        for (auto it2 = portfolioItems.constBegin(), end = portfolioItems.constEnd(); it2 != end; ++it2)
        {
            if (it2.value().showPrices)
            {
                instrumentIds.append(it2.key());
            }
        }
    }

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
