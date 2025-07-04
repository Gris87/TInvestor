#pragma once



#include "src/storage/user/iuserstorage.h"

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
    MOCK_METHOD(void, readLock, (), (override));
    MOCK_METHOD(void, readUnlock, (), (override));
    MOCK_METHOD(void, writeLock, (), (override));
    MOCK_METHOD(void, writeUnlock, (), (override));
    MOCK_METHOD(void, setToken, (const QString& token), (override));
    MOCK_METHOD(const QString&, getToken, (), (override));
    MOCK_METHOD(void, setUserInfo, (const User& user), (override));
    MOCK_METHOD(bool, isQualified, (), (override));
    MOCK_METHOD(float, getCommission, (), (override));
    MOCK_METHOD(void, setAccounts, (const Accounts& accounts), (override));
    MOCK_METHOD(const Accounts&, getAccounts, (), (override));
};
