#pragma once



#include "src/storage/stocks/istocksstorage.h"



class StocksStorage : public IStocksStorage
{
public:
    explicit StocksStorage(IStocksDatabase* stocksDatabase);
    ~StocksStorage();

    StocksStorage(const StocksStorage& another)            = delete;
    StocksStorage& operator=(const StocksStorage& another) = delete;

    QMutex*       getMutex() override;
    QList<Stock>* getStocks() override;
    void          readFromDatabase() override;

private:
    IStocksDatabase* mStocksDatabase;
    QMutex*          mMutex;
    QList<Stock>*    mStocks;
};
