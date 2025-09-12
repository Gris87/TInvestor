#pragma once



#include "src/grpc/igrpcclient.h"
#include "src/grpc/igrpcretryclient.h"
#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/storage/user/iuserstorage.h"
#include "src/threads/logs/ilogsthread.h"
#include "src/threads/trading/itradingthread.h"
#include "src/utils/timeutils/itimeutils.h"



class ITradingThreadFactory
{
public:
    ITradingThreadFactory()          = default;
    virtual ~ITradingThreadFactory() = default;

    ITradingThreadFactory(const ITradingThreadFactory& another)            = delete;
    ITradingThreadFactory& operator=(const ITradingThreadFactory& another) = delete;

    virtual ITradingThread* newInstance(
        IInstrumentsStorage* instrumentsStorage,
        IUserStorage*        userStorage,
        ITimeUtils*          timeUtils,
        IGrpcClient*         grpcClient,
        IGrpcRetryClient*    grpcRetryClient,
        ILogsThread*         logsThread,
        const QString&       accountId,
        const QString&       instrumentId,
        AsapMode             asapMode,
        float                avgPrice,
        float                price,
        double               expectedCost,
        const QString&       cause,
        QObject*             parent
    ) const = 0;
};
