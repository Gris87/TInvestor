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
        IBiDirInfosStorage*  biDirInfosStorage,
        IUserStorage*        userStorage,
        IConfig*             config,
        ITimeUtils*          timeUtils,
        ITradeUtils*         tradeUtils,
        IGrpcClient*         grpcClient,
        IGrpcRetryClient*    grpcRetryClient,
        ILogsThread*         logsThread,
        const QString&       accountId,
        Stock*               stock,
        BiDirMode            bidirMode,
        const QString&       cause,
        QObject*             parent
    ) const override;
};
