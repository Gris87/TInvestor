#pragma once



#include "src/db/stocks/istocksdatabase.h"



class IStocksStorage
{
public:
    IStocksStorage() {}
    virtual ~IStocksStorage() = default;

    IStocksStorage(const IStocksStorage &another) = delete;
    IStocksStorage& operator=(const IStocksStorage &another) = delete;

    virtual QList<Stock>* getStocks() = 0;
    virtual void readFromDatabase(IStocksDatabase *stocksDatabase) = 0;
};
