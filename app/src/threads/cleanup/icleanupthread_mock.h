#pragma once



#include "src/threads/cleanup/icleanupthread.h"

#include <gmock/gmock.h>



class CleanupThreadMock : public ICleanupThread
{
public:
    CleanupThreadMock() : ICleanupThread() {}
    ~CleanupThreadMock() override = default;

    CleanupThreadMock(const CleanupThreadMock &another) = delete;
    CleanupThreadMock& operator=(const CleanupThreadMock &another) = delete;

    MOCK_METHOD0(run, void());
};
