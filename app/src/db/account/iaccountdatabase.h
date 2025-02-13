#pragma once



class IAccountDatabase
{
public:
    explicit IAccountDatabase()
    {
    }
    virtual ~IAccountDatabase() = default;

    IAccountDatabase(const IAccountDatabase& another)            = delete;
    IAccountDatabase& operator=(const IAccountDatabase& another) = delete;
};
