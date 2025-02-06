#pragma once



#include "src/threads/refresh/irefreshthread.h"

#include <gmock/gmock.h>



class RefreshThreadMock : public IRefreshThread
{
public:
    RefreshThreadMock() : IRefreshThread() {}
    ~RefreshThreadMock() override = default;

    RefreshThreadMock(const RefreshThreadMock &another) = delete;
    RefreshThreadMock& operator=(const RefreshThreadMock &another) = delete;

    MOCK_METHOD0(run, void());
};
