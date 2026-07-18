#pragma once



#include "src/threads/bidirtradingcontrol/ibidirtradingcontrolthread.h"

#include <QTimeZone>

#include "src/config/iconfig.h"
#include "src/grpc/igrpcclient.h"
#include "src/storage/bidirinfos/ibidirinfosstorage.h"
#include "src/storage/stocks/istocksstorage.h"
#include "src/storage/user/iuserstorage.h"
#include "src/utils/timeutils/itimeutils.h"



class BiDirTradingControlThread : public IBiDirTradingControlThread
{
    Q_OBJECT

public:
    explicit BiDirTradingControlThread(
        IStocksStorage*     stocksStorage,
        IBiDirInfosStorage* biDirInfosStorage,
        IUserStorage*       userStorage,
        IConfig*            config,
        ITimeUtils*         timeUtils,
        IGrpcClient*        grpcClient,
        QObject*            parent = nullptr
    );
    ~BiDirTradingControlThread() override;

    BiDirTradingControlThread(const BiDirTradingControlThread& another)            = delete;
    BiDirTradingControlThread& operator=(const BiDirTradingControlThread& another) = delete;

    void run() override;

    void detectStocksForBiDirTrading(qint64 timestamp, bool tradeHugeBid, bool tradeHugeSpread);

    void terminateThread() override;

private:
    IStocksStorage*                   mStocksStorage;
    IBiDirInfosStorage*               mBiDirInfosStorage;
    IUserStorage*                     mUserStorage;
    IConfig*                          mConfig;
    ITimeUtils*                       mTimeUtils;
    IGrpcClient*                      mGrpcClient;
    qint64                            mLastDetectionTimestamp;
    bool                              mLastTradeHugeBid;
    bool                              mLastTradeHugeSpread;
    QList<InstrumentsForBiDirTrading> mLastInstrumentsForBiDirTrading;
    int                               mLastInstrumentsId;
};
