#pragma once



#include "src/storage/stocks/istocksstorage.h"

#include "src/db/stocks/istocksdatabase.h"
#include "src/storage/user/iuserstorage.h"



class StocksStorage : public IStocksStorage
{
public:
    explicit StocksStorage(IStocksDatabase* stocksDatabase, IUserStorage* userStorage);
    ~StocksStorage();

    StocksStorage(const StocksStorage& another)            = delete;
    StocksStorage& operator=(const StocksStorage& another) = delete;

    void           readFromDatabase() override;
    QMutex*        getMutex() override;
    QList<Stock*>& getStocks() override;
    bool           mergeStocksMeta(const QList<StockMeta>& stocksMeta) override;
    void           appendStockData(Stock* stock, const StockData* dataArray, int dataArraySize) override;
    void           deleteObsoleteData(qint64 obsoleteTimestamp, QList<Stock*>& stocks) override;
    void           obtainStocksDayStartPrice(qint64 timestamp) override;
    void           obtainStocksDatePrice(qint64 timestamp) override;
    void           obtainPayback(qint64 timestamp) override;

private:
    IStocksDatabase* mStocksDatabase;
    IUserStorage*    mUserStorage;
    QMutex*          mMutex;
    QList<Stock*>    mStocks;
};
