#pragma once



#include "src/threads/icleanupthread.h"

#include <gmock/gmock.h>



class CleanupThreadMock : public ICleanupThread
{
public:
    CleanupThreadMock() {}
    ~CleanupThreadMock() override = default;

    CleanupThreadMock(const CleanupThreadMock &another) = delete;
    CleanupThreadMock& operator=(const CleanupThreadMock &another) = delete;

    MOCK_METHOD0(process, void());
};
