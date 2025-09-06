#pragma once



#include "src/threads/bidirtrading/ibidirtradingthreadfactory.h"

#include <gmock/gmock.h>



class BiDirTradingThreadFactoryMock : public IBiDirTradingThreadFactory
{
public:
    BiDirTradingThreadFactoryMock() :
        IBiDirTradingThreadFactory()
    {
    }
    ~BiDirTradingThreadFactoryMock() override = default;

    BiDirTradingThreadFactoryMock(const BiDirTradingThreadFactoryMock& another)            = delete;
    BiDirTradingThreadFactoryMock& operator=(const BiDirTradingThreadFactoryMock& another) = delete;

    MOCK_METHOD(
        IBiDirTradingThread*,
        newInstance,
        (IInstrumentsStorage * instrumentsStorage,
         ITimeUtils*    timeUtils,
         IGrpcClient*   grpcClient,
         ILogsThread*   logsThread,
         const QString& accountId,
         const QString& instrumentId,
         const QString& cause,
         QObject*       parent),
        (const, override)
    );
};
