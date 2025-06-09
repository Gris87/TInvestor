#pragma once



#include "src/grpc/igrpcclient.h"
#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/threads/logs/ilogsthread.h"
#include "src/threads/trading/itradingthread.h"



class ITradingThreadFactory
{
public:
    ITradingThreadFactory()          = default;
    virtual ~ITradingThreadFactory() = default;

    ITradingThreadFactory(const ITradingThreadFactory& another)            = delete;
    ITradingThreadFactory& operator=(const ITradingThreadFactory& another) = delete;

    virtual ITradingThread* newInstance(
        IInstrumentsStorage* instrumentsStorage,
        IGrpcClient*         grpcClient,
        ILogsThread*         logsThread,
        const QString&       accountId,
        const QString&       instrumentId,
        double               expectedCost,
        QObject*             parent
    ) const = 0;
};
