#pragma once



#include "src/threads/bidirtradingcontrol/ibidirtradingcontrolthread.h"

#include <gmock/gmock.h>



class BiDirTradingControlThreadMock : public IBiDirTradingControlThread
{
    Q_OBJECT

public:
    explicit BiDirTradingControlThreadMock(QObject* parent = nullptr) :
        IBiDirTradingControlThread(parent)
    {
    }
    ~BiDirTradingControlThreadMock() override = default;

    BiDirTradingControlThreadMock(const BiDirTradingControlThreadMock& another)            = delete;
    BiDirTradingControlThreadMock& operator=(const BiDirTradingControlThreadMock& another) = delete;

    MOCK_METHOD(void, run, (), (override));
    MOCK_METHOD(void, setAccountId, (const QString& accountId), (override));
    MOCK_METHOD(void, terminateThread, (), (override));
};
