#pragma once



#include "src/threads/pricecollect/ipricecollectthread.h"

#include "src/config/iconfig.h"
#include "src/grpc/igrpcclient.h"
#include "src/storage/stocks/istocksstorage.h"
#include "src/utils/fs/file/ifilefactory.h"
#include "src/utils/http/ihttpclient.h"



class PriceCollectThread : public IPriceCollectThread
{
    Q_OBJECT

public:
    explicit PriceCollectThread(
        IConfig*        config,
        IStocksStorage* stocksStorage,
        IFileFactory*   fileFactory,
        IHttpClient*    httpClient,
        IGrpcClient*    grpcClient,
        QObject*        parent = nullptr
    );
    ~PriceCollectThread();

    PriceCollectThread(const PriceCollectThread& another)            = delete;
    PriceCollectThread& operator=(const PriceCollectThread& another) = delete;

    void run() override;

private:
    void storeNewStocksInfo(std::shared_ptr<tinkoff::SharesResponse> tinkoffStocks);
    void obtainStocksData();

    IConfig*        mConfig;
    IStocksStorage* mStocksStorage;
    IFileFactory*   mFileFactory;
    IHttpClient*    mHttpClient;
    IGrpcClient*    mGrpcClient;
};
