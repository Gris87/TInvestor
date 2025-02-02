#pragma once



class IStockStorage
{
public:
    IStockStorage() {}
    virtual ~IStockStorage() = default;

    IStockStorage(const IStockStorage &another) = delete;
    IStockStorage& operator=(const IStockStorage &another) = delete;
};
