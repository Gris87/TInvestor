#pragma once



#include "src/db/account/iaccountdatabase.h"



class IAccountStorage
{
public:
    explicit IAccountStorage()
    {
    }
    virtual ~IAccountStorage() = default;

    IAccountStorage(const IAccountStorage& another)            = delete;
    IAccountStorage& operator=(const IAccountStorage& another) = delete;

    virtual void           readFromDatabase(IAccountDatabase* accountDatabase) = 0;
    virtual const QString& getToken()                                          = 0;
};
