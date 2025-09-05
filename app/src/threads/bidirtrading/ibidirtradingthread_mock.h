#pragma once



#include "src/threads/bidirtrading/ibidirtradingthread.h"

#include <gmock/gmock.h>



class BiDirTradingThreadMock : public IBiDirTradingThread
{
    Q_OBJECT

public:
    explicit BiDirTradingThreadMock(QObject* parent = nullptr) :
        IBiDirTradingThread(parent)
    {
    }
    ~BiDirTradingThreadMock() override = default;

    BiDirTradingThreadMock(const BiDirTradingThreadMock& another)            = delete;
    BiDirTradingThreadMock& operator=(const BiDirTradingThreadMock& another) = delete;

    MOCK_METHOD(void, run, (), (override));
    MOCK_METHOD(void, terminateThread, (), (override));
};
