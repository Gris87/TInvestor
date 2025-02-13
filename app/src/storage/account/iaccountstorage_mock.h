#pragma once



#include "src/storage/account/iAccountstorage.h"

#include <gmock/gmock.h>



class AccountStorageMock : public IAccountStorage
{
public:
    explicit AccountStorageMock() :
        IAccountStorage()
    {
    }
    ~AccountStorageMock() override = default;

    AccountStorageMock(const AccountStorageMock& another)            = delete;
    AccountStorageMock& operator=(const AccountStorageMock& another) = delete;

    MOCK_METHOD(void, readFromDatabase, (IAccountDatabase * accountDatabase), (override));
    MOCK_METHOD(const QString&, getToken, (), (override));
};
