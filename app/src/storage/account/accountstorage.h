#pragma once



#include "src/storage/account/iaccountstorage.h"



class AccountStorage : public IAccountStorage
{
public:
    explicit AccountStorage();
    ~AccountStorage();

    AccountStorage(const AccountStorage& another)            = delete;
    AccountStorage& operator=(const AccountStorage& another) = delete;

    void readFromDatabase(IAccountDatabase* accountDatabase) override;
};
