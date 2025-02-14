#pragma once



#include "src/db/user/iuserdatabase.h"



class IUserStorage
{
public:
    explicit IUserStorage()
    {
    }
    virtual ~IUserStorage() = default;

    IUserStorage(const IUserStorage& another)            = delete;
    IUserStorage& operator=(const IUserStorage& another) = delete;

    virtual void           readFromDatabase(IUserDatabase* userDatabase) = 0;
    virtual const QString& getToken()                                    = 0;
};
