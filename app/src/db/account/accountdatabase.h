#pragma once



#include "src/db/account/iaccountdatabase.h"

#include <QtSql>



class AccountDatabase : public IAccountDatabase
{
public:
    explicit AccountDatabase();
    ~AccountDatabase();

    AccountDatabase(const AccountDatabase& another)            = delete;
    AccountDatabase& operator=(const AccountDatabase& another) = delete;

    QSqlDatabase db;

private:
    void createAccountTable();
};
