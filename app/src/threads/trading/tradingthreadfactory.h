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
        IGrpcClient*         grpcClient,
        ILogsThread*         logsThread,
        ITimeUtils*          timeUtils,
        const QString&       accountId,
        const QString&       instrumentId,
        double               expectedCost,
        QObject*             parent
    ) const override;
};
