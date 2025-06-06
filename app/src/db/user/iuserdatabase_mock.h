#pragma once



#include "src/db/user/iuserdatabase.h"

#include <gmock/gmock.h>



class UserDatabaseMock : public IUserDatabase
{
public:
    explicit UserDatabaseMock() :
        IUserDatabase()
    {
    }
    ~UserDatabaseMock() override = default;

    UserDatabaseMock(const UserDatabaseMock& another)            = delete;
    UserDatabaseMock& operator=(const UserDatabaseMock& another) = delete;

    MOCK_METHOD(User, readUserInfo, (), (override));
    MOCK_METHOD(Accounts, readAccounts, (), (override));
    MOCK_METHOD(void, writeToken, (const QString& token), (override));
    MOCK_METHOD(void, writeUserInfo, (const User& user), (override));
    MOCK_METHOD(void, writeAccounts, (const Accounts& accounts), (override));
};
