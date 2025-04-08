#pragma once



#include "src/domain/user/account.h"
#include "src/domain/user/user.h"



class IUserDatabase
{
public:
    explicit IUserDatabase() = default;
    virtual ~IUserDatabase() = default;

    IUserDatabase(const IUserDatabase& another)            = delete;
    IUserDatabase& operator=(const IUserDatabase& another) = delete;

    virtual User           readUserInfo()                                = 0;
    virtual QList<Account> readAccounts()                                = 0;
    virtual void           writeToken(const QString& token)              = 0;
    virtual void           writeUserInfo(const User& user)               = 0;
    virtual void           writeAccounts(const QList<Account>& accounts) = 0;
};
