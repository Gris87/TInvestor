#pragma once



#include "src/threads/trading/itradingthreadfactory.h"



class TradingThreadFactory : public ITradingThreadFactory
{
public:
    TradingThreadFactory();
    ~TradingThreadFactory() override;

    TradingThreadFactory(const TradingThreadFactory& another)            = delete;
    TradingThreadFactory& operator=(const TradingThreadFactory& another) = delete;

    ITradingThread* newInstance(
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
    ) const override;
};
