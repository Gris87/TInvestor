#pragma once



#include "src/threads/pricecollect/ipricecollectthread.h"

#include "src/grpc/igrpcclient.h"
#include "src/storage/stocks/istocksstorage.h"
#include "src/utils/fs/file/ifilefactory.h"
#include "src/utils/http/ihttpclientfactory.h"



class PriceCollectThread : public IPriceCollectThread
{
    Q_OBJECT

public:
    explicit PriceCollectThread(
        IStocksStorage*     stocksStorage,
        IFileFactory*       fileFactory,
        IHttpClientFactory* httpClientFactory,
        IGrpcClient*        grpcClient,
        QObject*            parent = nullptr
    );
    ~PriceCollectThread();

    PriceCollectThread(const PriceCollectThread& another)            = delete;
    PriceCollectThread& operator=(const PriceCollectThread& another) = delete;

    void run() override;

private:
    IStocksStorage*     mStocksStorage;
    IFileFactory*       mFileFactory;
    IHttpClientFactory* mHttpClientFactory;
    IGrpcClient*        mGrpcClient;
};
