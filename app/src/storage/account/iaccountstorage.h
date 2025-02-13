#pragma once



class IAccountStorage
{
public:
    explicit IAccountStorage()
    {
    }
    virtual ~IAccountStorage() = default;

    IAccountStorage(const IAccountStorage& another)            = delete;
    IAccountStorage& operator=(const IAccountStorage& another) = delete;
};
