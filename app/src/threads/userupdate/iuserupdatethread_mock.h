#pragma once



#include "src/threads/userupdate/iuserupdatethread.h"

#include <gmock/gmock.h>



class UserUpdateThreadMock : public IUserUpdateThread
{
    Q_OBJECT

public:
    explicit UserUpdateThreadMock(QObject* parent = nullptr) :
        IUserUpdateThread(parent)
    {
    }
    ~UserUpdateThreadMock() override = default;

    UserUpdateThreadMock(const UserUpdateThreadMock& another)            = delete;
    UserUpdateThreadMock& operator=(const UserUpdateThreadMock& another) = delete;

    MOCK_METHOD(void, run, (), (override));
    MOCK_METHOD(void, terminateThread, (), (override));
};
