#pragma once



#include "src/threads/trading/itradingthreadfactory.h"

#include <gmock/gmock.h>



class TradingThreadFactoryMock : public ITradingThreadFactory
{
public:
    TradingThreadFactoryMock() :
        ITradingThreadFactory()
    {
    }
    ~TradingThreadFactoryMock() override = default;

    TradingThreadFactoryMock(const TradingThreadFactoryMock& another)            = delete;
    TradingThreadFactoryMock& operator=(const TradingThreadFactoryMock& another) = delete;

    MOCK_METHOD(
        ITradingThread*,
        newInstance,
        (IInstrumentsStorage * instrumentsStorage,
         ITimeUtils*    timeUtils,
         IGrpcClient*   grpcClient,
         ILogsThread*   logsThread,
         const QString& accountId,
         const QString& instrumentId,
         double         expectedCost,
         const QString& cause,
         QObject*       parent),
        (const, override)
    );
};
