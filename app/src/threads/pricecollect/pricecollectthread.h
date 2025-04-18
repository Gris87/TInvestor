#pragma once



#include "src/threads/pricecollect/ipricecollectthread.h"

#include "src/config/iconfig.h"
#include "src/grpc/igrpcclient.h"
#include "src/storage/stocks/istocksstorage.h"
#include "src/storage/user/iuserstorage.h"
#include "src/utils/fs/dir/idirfactory.h"
#include "src/utils/fs/file/ifilefactory.h"
#include "src/utils/fs/zip/qzip/iqzipfactory.h"
#include "src/utils/fs/zip/qzipfile/iqzipfilefactory.h"
#include "src/utils/http/ihttpclient.h"
#include "src/utils/timeutils/itimeutils.h"



class PriceCollectThread : public IPriceCollectThread
{
    Q_OBJECT

public:
    explicit PriceCollectThread(
        IConfig*          config,
        IUserStorage*     userStorage,
        IStocksStorage*   stocksStorage,
        IDirFactory*      dirFactory,
        IFileFactory*     fileFactory,
        IQZipFactory*     qZipFactory,
        IQZipFileFactory* qZipFileFactory,
        ITimeUtils*       timeUtils,
        IHttpClient*      httpClient,
        IGrpcClient*      grpcClient,
        QObject*          parent = nullptr
    );
    ~PriceCollectThread() override;

    PriceCollectThread(const PriceCollectThread& another)            = delete;
    PriceCollectThread& operator=(const PriceCollectThread& another) = delete;

    void run() override;

    void downloadLogo(const QUrl& url, const std::shared_ptr<IFile>& stockLogoFile);
    bool storeNewStocksInfo(const std::shared_ptr<tinkoff::SharesResponse>& tinkoffStocks);
    void obtainStocksData();
    void cleanupOperationalData();
    bool obtainStocksDayStartPrice();
    void obtainTurnover();
    void obtainPayback();
    void notifyAboutChanges(bool needStocksUpdate, bool needPricesUpdate);

private:
    IConfig*          mConfig;
    IUserStorage*     mUserStorage;
    IStocksStorage*   mStocksStorage;
    IFileFactory*     mFileFactory;
    IQZipFactory*     mQZipFactory;
    IQZipFileFactory* mQZipFileFactory;
    ITimeUtils*       mTimeUtils;
    IHttpClient*      mHttpClient;
    IGrpcClient*      mGrpcClient;
    qint64            mDayStartTimestamp;
};
