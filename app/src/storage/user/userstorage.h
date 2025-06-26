#pragma once



#include "src/storage/user/iuserstorage.h"

#include <QReadWriteLock>

#include "src/db/user/iuserdatabase.h"



class UserStorage : public IUserStorage
{
public:
    explicit UserStorage(IUserDatabase* userDatabase);
    ~UserStorage() override;

    UserStorage(const UserStorage& another)            = delete;
    UserStorage& operator=(const UserStorage& another) = delete;

    void            readFromDatabase() override;
    void            readLock() override;
    void            readUnlock() override;
    void            writeLock() override;
    void            writeUnlock() override;
    void            setToken(const QString& token) override;
    const QString&  getToken() override;
    void            setUserInfo(const User& user) override;
    bool            isQualified() override;
    float           getCommission() override;
    void            setAccounts(const Accounts& accounts) override;
    const Accounts& getAccounts() override;

private:
    IUserDatabase*  mUserDatabase;
    QReadWriteLock* mRwMutex;
    User            mUser;
    Accounts        mAccounts;
};
