#pragma once



#include "src/storage/stocks/istocksstorage.h"

#include "src/db/stocks/istocksdatabase.h"



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
    void          mergeStocksMeta(const QList<StockMeta>& stocksMeta) override;
    void          appendStockData(Stock* stock, const StockData* dataArray, int dataArraySize) override;
    void          deleteObsoleteData(qint64 obsoleteTimestamp, QList<Stock>* stocks) override;

private:
    IStocksDatabase* mStocksDatabase;
    QMutex*          mMutex;
    QList<Stock>*    mStocks;
};
