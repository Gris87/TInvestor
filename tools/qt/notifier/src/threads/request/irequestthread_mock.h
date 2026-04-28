#pragma once



#include "src/threads/request/irequestthread.h"

#include <gmock/gmock.h>



class RequestThreadMock : public IRequestThread
{
    Q_OBJECT

public:
    explicit RequestThreadMock(QObject* parent = nullptr) :
        IRequestThread(parent)
    {
    }
    ~RequestThreadMock() override = default;

    RequestThreadMock(const RequestThreadMock& another)            = delete;
    RequestThreadMock& operator=(const RequestThreadMock& another) = delete;

    MOCK_METHOD(void, run, (), (override));
    MOCK_METHOD(void, terminateThread, (), (override));
};
