#pragma once



#include "src/db/user/iuserdatabase.h"

#include <QSqlDatabase>

#include "src/simplecrypt/simplecrypt.h"



class UserDatabase : public IUserDatabase
{
public:
    explicit UserDatabase();
    ~UserDatabase();

    UserDatabase(const UserDatabase& another)            = delete;
    UserDatabase& operator=(const UserDatabase& another) = delete;

    User           readUserInfo() override;
    QList<Account> readAccounts() override;
    void           writeToken(const QString& token) override;
    void           writeUserInfo(const User& user) override;
    void           writeAccounts(const QList<Account>& accounts) override;

    QSqlDatabase db;

private:
    void createUserTable();
    void createAccountsTable();

    SimpleCrypt mSimpleCrypt;
};
