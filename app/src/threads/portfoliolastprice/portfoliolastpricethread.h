#pragma once



#include "src/threads/portfoliolastprice/iportfoliolastpricethread.h"

#include "src/grpc/igrpcclient.h"
#include "src/storage/stocks/istocksstorage.h"
#include "src/utils/timeutils/itimeutils.h"



class PortfolioLastPriceThread : public IPortfolioLastPriceThread
{
    Q_OBJECT

public:
    explicit PortfolioLastPriceThread(
        IStocksStorage* stocksStorage, ITimeUtils* timeUtils, IGrpcClient* grpcClient, QObject* parent = nullptr
    );
    ~PortfolioLastPriceThread() override;

    PortfolioLastPriceThread(const PortfolioLastPriceThread& another)            = delete;
    PortfolioLastPriceThread& operator=(const PortfolioLastPriceThread& another) = delete;

    void run() override;

    void stocksChanged() override;
    void terminateThread() override;

    void createMarketDataStream();

private:
    QStringList           getStockUIDs();
    QMap<QString, Stock*> buildStocksMap();

    IStocksStorage*                   mStocksStorage;
    ITimeUtils*                       mTimeUtils;
    IGrpcClient*                      mGrpcClient;
    std::shared_ptr<MarketDataStream> mMarketDataStream;
    bool                              mNeedToRebuildStocksMap;
};
