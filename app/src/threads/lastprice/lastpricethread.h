#pragma once



#include "src/threads/lastprice/ilastpricethread.h"

#include <QReadWriteLock>

#include "src/grpc/igrpcclient.h"
#include "src/storage/stocks/istocksstorage.h"
#include "src/utils/timeutils/itimeutils.h"



class LastPriceThread : public ILastPriceThread
{
    Q_OBJECT

public:
    explicit LastPriceThread(
        IStocksStorage* stocksStorage, ITimeUtils* timeUtils, IGrpcClient* grpcClient, QObject* parent = nullptr
    );
    ~LastPriceThread() override;

    LastPriceThread(const LastPriceThread& another)            = delete;
    LastPriceThread& operator=(const LastPriceThread& another) = delete;

    void run() override;

    void stocksChanged() override;
    void terminateThread() override;

    bool createMarketDataStream(const QStringList& stocks);

private:
    void handleLastPrice(Stock* stock, const tinkoff::LastPrice& lastPriceResp);

    QStringList           getStockUIDs();
    QMap<QString, Stock*> buildStocksMap();

    QReadWriteLock*                   mRwMutex;
    IStocksStorage*                   mStocksStorage;
    ITimeUtils*                       mTimeUtils;
    IGrpcClient*                      mGrpcClient;
    std::shared_ptr<MarketDataStream> mMarketDataStream;
};
