#pragma once



#include <QMutex>

#include "src/domain/user/account.h"
#include "src/domain/user/user.h"



class IUserStorage
{
public:
    explicit IUserStorage()
    {
    }
    virtual ~IUserStorage() = default;

    IUserStorage(const IUserStorage& another)            = delete;
    IUserStorage& operator=(const IUserStorage& another) = delete;

    virtual void                  readFromDatabase()                          = 0;
    virtual QMutex*               getMutex()                                  = 0;
    virtual void                  setToken(const QString& token)              = 0;
    virtual const QString&        getToken()                                  = 0;
    virtual void                  setUserInfo(const User& user)               = 0;
    virtual bool                  isQualified()                               = 0;
    virtual float                 getCommission()                             = 0;
    virtual void                  setAccounts(const QList<Account>& accounts) = 0;
    virtual const QList<Account>& getAccounts()                               = 0;
};
