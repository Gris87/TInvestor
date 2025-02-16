#pragma once



#include <QString>



class IUserStorage
{
public:
    explicit IUserStorage()
    {
    }
    virtual ~IUserStorage() = default;

    IUserStorage(const IUserStorage& another)            = delete;
    IUserStorage& operator=(const IUserStorage& another) = delete;

    virtual void           readFromDatabase()             = 0;
    virtual const QString& getToken()                     = 0;
    virtual void           setToken(const QString& token) = 0;
};
