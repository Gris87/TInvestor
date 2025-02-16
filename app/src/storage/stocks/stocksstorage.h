#pragma once



#include "src/storage/stocks/istocksstorage.h"



class StocksStorage : public IStocksStorage
{
public:
    explicit StocksStorage(IStocksDatabase* stocksDatabase);
    ~StocksStorage();

    StocksStorage(const StocksStorage& another)            = delete;
    StocksStorage& operator=(const StocksStorage& another) = delete;

    void          readFromDatabase() override;
    QMutex*       getMutex() override;
    QList<Stock>* getStocks() override;

private:
    IStocksDatabase* mStocksDatabase;
    QMutex*          mMutex;
    QList<Stock>*    mStocks;
};
