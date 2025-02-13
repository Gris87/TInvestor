#pragma once



#include "src/domain/account/account.h"



class IAccountDatabase
{
public:
    explicit IAccountDatabase()
    {
    }
    virtual ~IAccountDatabase() = default;

    IAccountDatabase(const IAccountDatabase& another)            = delete;
    IAccountDatabase& operator=(const IAccountDatabase& another) = delete;

    virtual Account readAccountInfo()               = 0;
    virtual void    writeToken(const QString token) = 0;
};
