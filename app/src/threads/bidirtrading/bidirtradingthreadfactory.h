#pragma once



#include "src/threads/bidirtrading/ibidirtradingthreadfactory.h"



class BiDirTradingThreadFactory : public IBiDirTradingThreadFactory
{
public:
    BiDirTradingThreadFactory();
    ~BiDirTradingThreadFactory() override;

    BiDirTradingThreadFactory(const BiDirTradingThreadFactory& another)            = delete;
    BiDirTradingThreadFactory& operator=(const BiDirTradingThreadFactory& another) = delete;

    IBiDirTradingThread* newInstance(
        IInstrumentsStorage* instrumentsStorage,
        IConfig*             config,
        ITimeUtils*          timeUtils,
        IGrpcClient*         grpcClient,
        ILogsThread*         logsThread,
        const QString&       accountId,
        const QString&       instrumentId,
        const QString&       cause,
        QObject*             parent
    ) const override;
};
