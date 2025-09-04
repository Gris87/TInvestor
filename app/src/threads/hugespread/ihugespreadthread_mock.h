#pragma once



#include "src/threads/hugespread/ihugespreadthread.h"

#include <gmock/gmock.h>



class HugeSpreadThreadMock : public IHugeSpreadThread
{
    Q_OBJECT

public:
    explicit HugeSpreadThreadMock(QObject* parent = nullptr) :
        IHugeSpreadThread(parent)
    {
    }
    ~HugeSpreadThreadMock() override = default;

    HugeSpreadThreadMock(const HugeSpreadThreadMock& another)            = delete;
    HugeSpreadThreadMock& operator=(const HugeSpreadThreadMock& another) = delete;

    MOCK_METHOD(void, run, (), (override));
    MOCK_METHOD(void, terminateThread, (), (override));
};
