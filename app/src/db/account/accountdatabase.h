#pragma once



#include "src/db/account/iaccountdatabase.h"



class AccountDatabase : public IAccountDatabase
{
public:
    explicit AccountDatabase();
    ~AccountDatabase();

    AccountDatabase(const AccountDatabase& another)            = delete;
    AccountDatabase& operator=(const AccountDatabase& another) = delete;
};
