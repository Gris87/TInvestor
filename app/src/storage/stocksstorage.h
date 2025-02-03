#pragma once



#include "src/storage/istocksstorage.h"



class StocksStorage : public IStocksStorage
{
public:
    explicit StocksStorage();
    ~StocksStorage();

    StocksStorage(const StocksStorage &another) = delete;
    StocksStorage& operator=(const StocksStorage &another) = delete;

    QList<Stock>* getStocks() override;
    void readFromDatabase(IStocksDatabase *stocksDatabase) override;

private:
    QList<Stock> mStocks;
};
