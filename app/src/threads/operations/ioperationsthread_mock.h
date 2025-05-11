#pragma once



#include "src/threads/operations/ioperationsthread.h"

#include <gmock/gmock.h>



class OperationsThreadMock : public IOperationsThread
{
    Q_OBJECT

public:
    explicit OperationsThreadMock(QObject* parent = nullptr) :
        IOperationsThread(parent)
    {
    }
    ~OperationsThreadMock() override = default;

    OperationsThreadMock(const OperationsThreadMock& another)            = delete;
    OperationsThreadMock& operator=(const OperationsThreadMock& another) = delete;

    MOCK_METHOD(void, run, (), (override));
    MOCK_METHOD(void, setAccount, (const QString& account), (override));
    MOCK_METHOD(void, terminateThread, (), (override));
};
