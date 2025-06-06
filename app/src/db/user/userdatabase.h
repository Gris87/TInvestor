#pragma once



#include "src/db/user/iuserdatabase.h"

#include <QSqlDatabase>

#include "src/simplecrypt/simplecrypt.h"



class UserDatabase : public IUserDatabase
{
public:
    explicit UserDatabase();
    ~UserDatabase() override;

    UserDatabase(const UserDatabase& another)            = delete;
    UserDatabase& operator=(const UserDatabase& another) = delete;

    User     readUserInfo() override;
    Accounts readAccounts() override;
    void     writeToken(const QString& token) override;
    void     writeUserInfo(const User& user) override;
    void     writeAccounts(const Accounts& accounts) override;

    QSqlDatabase db;

private:
    void initSimpleCrypt();
    void createUserTable() const;
    void createAccountsTable() const;

    SimpleCrypt mSimpleCrypt;
};
