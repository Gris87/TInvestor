#pragma once



#include "src/storage/istockstorage.h"



class StockStorage : public IStockStorage
{
public:
    explicit StockStorage();
    ~StockStorage();

    StockStorage(const StockStorage &another) = delete;
    StockStorage& operator=(const StockStorage &another) = delete;
};
