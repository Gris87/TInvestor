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

    void           readFromDatabase() override;
    QMutex*        getMutex() override;
    const QString& getToken() override;
    void           setToken(const QString& token) override;
    void           setUserInfo(const User& user) override;
    const QList<Account>& getAccounts() override;
    void                  setAccounts(const QList<Account>& accounts) override;

private:
    IUserDatabase* mUserDatabase;
    QMutex*        mMutex;
    User           mUser;
    QList<Account> mAccounts;
};
