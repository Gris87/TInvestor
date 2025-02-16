#pragma once



#include "src/storage/user/iUserstorage.h"

#include <gmock/gmock.h>



class UserStorageMock : public IUserStorage
{
public:
    explicit UserStorageMock() :
        IUserStorage()
    {
    }
    ~UserStorageMock() override = default;

    UserStorageMock(const UserStorageMock& another)            = delete;
    UserStorageMock& operator=(const UserStorageMock& another) = delete;

    MOCK_METHOD(void, readFromDatabase, (), (override));
    MOCK_METHOD(const QString&, getToken, (), (override));
    MOCK_METHOD(void, setToken, (const QString& token), (override));
};
