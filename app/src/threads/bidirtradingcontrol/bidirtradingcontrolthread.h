#pragma once



#include "src/threads/bidirtradingcontrol/ibidirtradingcontrolthread.h"

#include "src/config/iconfig.h"
#include "src/grpc/igrpcclient.h"
#include "src/storage/stocks/istocksstorage.h"



class BiDirTradingControlThread : public IBiDirTradingControlThread
{
    Q_OBJECT

public:
    explicit BiDirTradingControlThread(
        IStocksStorage* stocksStorage, IConfig* config, IGrpcClient* grpcClient, QObject* parent = nullptr
    );
    ~BiDirTradingControlThread() override;

    BiDirTradingControlThread(const BiDirTradingControlThread& another)            = delete;
    BiDirTradingControlThread& operator=(const BiDirTradingControlThread& another) = delete;

    void run() override;

    void setAccountId(const QString& accountId) override;

    void terminateThread() override;

private:
    void detectHugeSpreadStocks();

    IStocksStorage* mStocksStorage;
    IConfig*        mConfig;
    IGrpcClient*    mGrpcClient;
    QString         mAccountId;
    qint64          mLastDetectionTimestamp;
};
