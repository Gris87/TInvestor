#pragma once



#include "src/threads/bidirtradingcontrol/ibidirtradingcontrolthread.h"

#include "src/config/iconfig.h"
#include "src/grpc/igrpcclient.h"
#include "src/storage/stocks/istocksstorage.h"
#include "src/storage/user/iuserstorage.h"



class BiDirTradingControlThread : public IBiDirTradingControlThread
{
    Q_OBJECT

public:
    explicit BiDirTradingControlThread(
        IStocksStorage* stocksStorage,
        IUserStorage*   userStorage,
        IConfig*        config,
        IGrpcClient*    grpcClient,
        QObject*        parent = nullptr
    );
    ~BiDirTradingControlThread() override;

    BiDirTradingControlThread(const BiDirTradingControlThread& another)            = delete;
    BiDirTradingControlThread& operator=(const BiDirTradingControlThread& another) = delete;

    void run() override;

    void setAccountId(const QString& accountId) override;

    void terminateThread() override;

private:
    void detectHugeSpreadStocks(bool tradeHugeSpread, bool tradeLiquidityEtfDaily);

    IStocksStorage* mStocksStorage;
    IUserStorage*   mUserStorage;
    IConfig*        mConfig;
    IGrpcClient*    mGrpcClient;
    QString         mAccountId;
    qint64          mLastDetectionTimestamp;
    bool            mLastTradeHugeSpread;
    bool            mLastTradeLiquidityEtfDaily;
};
