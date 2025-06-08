#pragma once



#include "src/threads/follow/ifollowthread.h"

#include <gmock/gmock.h>



class FollowThreadMock : public IFollowThread
{
    Q_OBJECT

public:
    explicit FollowThreadMock(QObject* parent = nullptr) :
        IFollowThread(parent)
    {
    }
    ~FollowThreadMock() override = default;

    FollowThreadMock(const FollowThreadMock& another)            = delete;
    FollowThreadMock& operator=(const FollowThreadMock& another) = delete;

    MOCK_METHOD(void, run, (), (override));
    MOCK_METHOD(void, setAccountIds, (const QString& accountId, const QString& anotherAccountId), (override));
    MOCK_METHOD(void, terminateThread, (), (override));
};
