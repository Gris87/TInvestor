#pragma once



#include "src/threads/cleanup/icleanupthread.h"

#include <gmock/gmock.h>



class CleanupThreadMock : public ICleanupThread
{
    Q_OBJECT

public:
    explicit CleanupThreadMock(QObject* parent = nullptr) :
        ICleanupThread(parent)
    {
    }
    ~CleanupThreadMock() override = default;

    CleanupThreadMock(const CleanupThreadMock& another)            = delete;
    CleanupThreadMock& operator=(const CleanupThreadMock& another) = delete;

    MOCK_METHOD(void, run, (), (override));
};
