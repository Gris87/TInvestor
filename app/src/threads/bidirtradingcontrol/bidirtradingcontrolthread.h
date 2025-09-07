#pragma once



#include "src/threads/bidirtradingcontrol/ibidirtradingcontrolthread.h"

#include <QTimeZone>

#include "src/config/iconfig.h"
#include "src/grpc/igrpcclient.h"
#include "src/storage/stocks/istocksstorage.h"
#include "src/storage/user/iuserstorage.h"
#include "src/utils/timeutils/itimeutils.h"



class BiDirTradingControlThread : public IBiDirTradingControlThread
{
    Q_OBJECT

public:
    explicit BiDirTradingControlThread(
        IStocksStorage* stocksStorage,
        IUserStorage*   userStorage,
        IConfig*        config,
        ITimeUtils*     timeUtils,
        IGrpcClient*    grpcClient,
        QObject*        parent = nullptr
    );
    ~BiDirTradingControlThread() override;

    BiDirTradingControlThread(const BiDirTradingControlThread& another)            = delete;
    BiDirTradingControlThread& operator=(const BiDirTradingControlThread& another) = delete;

    void run() override;

    void detectHugeSpreadStocks(qint64 timestamp, bool tradeHugeSpread, bool tradeLiquidityEtfDaily);

    void setAccountId(const QString& accountId) override;

    void terminateThread() override;

private:
    IStocksStorage* mStocksStorage;
    IUserStorage*   mUserStorage;
    IConfig*        mConfig;
    ITimeUtils*     mTimeUtils;
    IGrpcClient*    mGrpcClient;
    QTimeZone       mMoscowTimezone;
    QString         mAccountId;
    qint64          mLastDetectionTimestamp;
    bool            mLastTradeHugeSpread;
    bool            mLastTradeLiquidityEtfDaily;
};
