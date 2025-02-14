#pragma once



#include "src/domain/user/user.h"



class IUserDatabase
{
public:
    explicit IUserDatabase()
    {
    }
    virtual ~IUserDatabase() = default;

    IUserDatabase(const IUserDatabase& another)            = delete;
    IUserDatabase& operator=(const IUserDatabase& another) = delete;

    virtual User readUserInfo()                   = 0;
    virtual void writeToken(const QString& token) = 0;
    virtual void writeUserInfo(const User& user)  = 0;
};
