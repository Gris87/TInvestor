#pragma once



#include "src/storage/user/iuserstorage.h"

#include "src/db/user/iuserdatabase.h"



class UserStorage : public IUserStorage
{
public:
    explicit UserStorage(IUserDatabase* userDatabase);
    ~UserStorage();

    UserStorage(const UserStorage& another)            = delete;
    UserStorage& operator=(const UserStorage& another) = delete;

    void                  readFromDatabase() override;
    QMutex*               getMutex() override;
    void                  setToken(const QString& token) override;
    const QString&        getToken() override;
    void                  setUserInfo(const User& user) override;
    bool                  isQualified() override;
    float                 getCommission() override;
    void                  setAccounts(const QList<Account>& accounts) override;
    const QList<Account>& getAccounts() override;

private:
    IUserDatabase* mUserDatabase;
    QMutex*        mMutex;
    User           mUser;
    QList<Account> mAccounts;
};
