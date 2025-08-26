#include "src/threads/lastprice/lastpricethread.h"

#include <QDebug>

#include "src/grpc/utils.h"



constexpr qint64 MS_IN_SECOND = 1000LL;
constexpr qint64 SLEEP_DELAY  = 5LL * MS_IN_SECOND; // 5 seconds



LastPriceThread::LastPriceThread(IStocksStorage* stocksStorage, ITimeUtils* timeUtils, IGrpcClient* grpcClient, QObject* parent) :
    ILastPriceThread(parent),
    mRwMutex(new QReadWriteLock()),
    mStocksStorage(stocksStorage),
    mTimeUtils(timeUtils),
    mGrpcClient(grpcClient),
    mMarketDataStream()
{
    qDebug() << "Create LastPriceThread";
}

LastPriceThread::~LastPriceThread()
{
    qDebug() << "Destroy LastPriceThread";

    delete mRwMutex;
}

// NOLINTBEGIN(readability-function-cognitive-complexity)
void LastPriceThread::run()
{
    qDebug() << "Running LastPriceThread";

    blockSignals(false);

    while (!QThread::currentThread()->isInterruptionRequested())
    {
        const QStringList stocks = getStockUIDs();

        if (!stocks.isEmpty())
        {
            if (createMarketDataStream(stocks))
            {
                QMap<QString, Stock*> stocksMap = buildStocksMap();

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

                        StockOperationalData stockData; // NOLINT(cppcoreguidelines-pro-type-member-init)

                        stockData.timestamp = timeToTimestamp(lastPriceResp.time());
                        stockData.price     = quotationToFloat(lastPriceResp.price());

                        const QString instrumentId = QString::fromStdString(lastPriceResp.instrument_uid());

                        Stock* stock = stocksMap[instrumentId];

                        stock->writeLock();
                        stock->operational.detailedData.insert(
                            std::distance(
                                stock->operational.detailedData.constBegin(),
                                std::lower_bound(
                                    stock->operational.detailedData.constBegin(),
                                    stock->operational.detailedData.constEnd(),
                                    stockData.timestamp,
                                    [](const StockOperationalData& stockData, qint64 value) {
                                        return stockData.timestamp < value;
                                    }
                                )
                            ),
                            stockData
                        );

                        Q_ASSERT_X(
                            std::is_sorted(
                                stock->operational.detailedData.constBegin(),
                                stock->operational.detailedData.constEnd(),
                                [](const StockOperationalData& l, const StockOperationalData& r) {
                                    return l.timestamp < r.timestamp;
                                }
                            ),
                            __FUNCTION__,
                            "Stock data is unsorted"
                        );
                        stock->writeUnlock();

                        emit lastPriceChanged(instrumentId);
                    }
                }

                const QWriteLocker lock(mRwMutex);

                mGrpcClient->finishMarketDataStream(mMarketDataStream);
                mMarketDataStream = nullptr;
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

    qDebug() << "Finish LastPriceThread";
}
// NOLINTEND(readability-function-cognitive-complexity)

QStringList LastPriceThread::getStockUIDs()
{
    QStringList res;

    mStocksStorage->readLock();
    const QList<Stock*>& stocks = mStocksStorage->getStocks();

    res.resizeForOverwrite(stocks.size());

    for (int i = 0; i < stocks.size(); ++i)
    {
        Stock* stock = stocks.at(i);

        stock->readLock();
        res[i] = stock->meta.instrumentId;
        stock->readUnlock();
    }
    mStocksStorage->readUnlock();

    return res;
}

QMap<QString, Stock*> LastPriceThread::buildStocksMap()
{
    QMap<QString, Stock*> res; // Instrument ID => Stock

    mStocksStorage->readLock();
    const QList<Stock*>& stocks = mStocksStorage->getStocks();

    for (Stock* stock : stocks)
    {
        stock->readLock();
        res[stock->meta.instrumentId] = stock;
        stock->readUnlock();
    }
    mStocksStorage->readUnlock();

    return res;
}

void LastPriceThread::stocksChanged()
{
    const QReadLocker lock(mRwMutex);

    if (mMarketDataStream != nullptr)
    {
        mGrpcClient->cancelMarketDataStream(mMarketDataStream);
    }
}

void LastPriceThread::terminateThread()
{
    blockSignals(true);

    const QReadLocker lock(mRwMutex);

    if (mMarketDataStream != nullptr)
    {
        mGrpcClient->cancelMarketDataStream(mMarketDataStream);
    }

    requestInterruption();
}

bool LastPriceThread::createMarketDataStream(const QStringList& stocks)
{
    bool res = false;

    const QWriteLocker lock(mRwMutex);

    if (!QThread::currentThread()->isInterruptionRequested())
    {
        mMarketDataStream = mGrpcClient->createMarketDataStreamForLastPrice(stocks);

        res = mMarketDataStream != nullptr;
    }

    return res;
}
