#pragma once



#include "src/domain/user/account.h"
#include "src/domain/user/user.h"



class IUserStorage
{
public:
    explicit IUserStorage() = default;
    virtual ~IUserStorage() = default;

    IUserStorage(const IUserStorage& another)            = delete;
    IUserStorage& operator=(const IUserStorage& another) = delete;

    virtual void            readFromDatabase()                    = 0;
    virtual void            readLock()                            = 0;
    virtual void            readUnlock()                          = 0;
    virtual void            writeLock()                           = 0;
    virtual void            writeUnlock()                         = 0;
    virtual void            setToken(const QString& token)        = 0;
    virtual const QString&  getToken()                            = 0;
    virtual void            setUserInfo(const User& user)         = 0;
    virtual bool            isQualified()                         = 0;
    virtual float           getCommission()                       = 0;
    virtual void            setAccounts(const Accounts& accounts) = 0;
    virtual const Accounts& getAccounts()                         = 0;
};
