#pragma once



#include "src/db/account/iaccountdatabase.h"

#include <gmock/gmock.h>



class AccountDatabaseMock : public IAccountDatabase
{
public:
    explicit AccountDatabaseMock() :
        IAccountDatabase()
    {
    }
    ~AccountDatabaseMock() override = default;

    AccountDatabaseMock(const AccountDatabaseMock& another)            = delete;
    AccountDatabaseMock& operator=(const AccountDatabaseMock& another) = delete;

    MOCK_METHOD(Account, readAccountInfo, (), (override));
    MOCK_METHOD(void, writeToken, (const QString token), (override));
};
