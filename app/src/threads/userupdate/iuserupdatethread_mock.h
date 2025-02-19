#pragma once



#include "src/threads/userupdate/iuserupdatethread.h"

#include <gmock/gmock.h>



class UserUpdateThreadMock : public IUserUpdateThread
{
public:
    UserUpdateThreadMock() :
        IUserUpdateThread()
    {
    }
    ~UserUpdateThreadMock() override = default;

    UserUpdateThreadMock(const UserUpdateThreadMock& another)            = delete;
    UserUpdateThreadMock& operator=(const UserUpdateThreadMock& another) = delete;

    MOCK_METHOD(void, run, (), (override));
};
