#pragma once



#include "src/config/iconfig.h"
#include "src/grpc/igrpcclient.h"
#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/threads/bidirtrading/ibidirtradingthread.h"
#include "src/threads/logs/ilogsthread.h"
#include "src/utils/timeutils/itimeutils.h"



class IBiDirTradingThreadFactory
{
public:
    IBiDirTradingThreadFactory()          = default;
    virtual ~IBiDirTradingThreadFactory() = default;

    IBiDirTradingThreadFactory(const IBiDirTradingThreadFactory& another)            = delete;
    IBiDirTradingThreadFactory& operator=(const IBiDirTradingThreadFactory& another) = delete;

    virtual IBiDirTradingThread* newInstance(
        IInstrumentsStorage* instrumentsStorage,
        IConfig*             config,
        ITimeUtils*          timeUtils,
        IGrpcClient*         grpcClient,
        ILogsThread*         logsThread,
        const QString&       accountId,
        const QString&       instrumentId,
        const QString&       cause,
        QObject*             parent
    ) const = 0;
};
