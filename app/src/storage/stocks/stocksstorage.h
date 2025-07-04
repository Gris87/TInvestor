#pragma once



#include "src/storage/stocks/istocksstorage.h"

#include <QReadWriteLock>

#include "src/db/stocks/istocksdatabase.h"
#include "src/storage/user/iuserstorage.h"



class StocksStorage : public IStocksStorage
{
public:
    explicit StocksStorage(IStocksDatabase* stocksDatabase, IUserStorage* userStorage);
    ~StocksStorage() override;

    StocksStorage(const StocksStorage& another)            = delete;
    StocksStorage& operator=(const StocksStorage& another) = delete;

    void                 readFromDatabase() override;
    void                 assignLogos() override;
    void                 readLock() override;
    void                 readUnlock() override;
    void                 writeLock() override;
    void                 writeUnlock() override;
    const QList<Stock*>& getStocks() override;
    bool                 mergeStocksMeta(const QList<StockMeta>& stocksMeta) override;
    void                 appendStockData(Stock* stock, const StockData* dataArray, int dataArraySize) override;
    void                 deleteObsoleteData(qint64 timestamp) override;
    void                 cleanupOperationalData(qint64 timestamp) override;
    void                 obtainStocksDayStartPrice(qint64 timestamp) override;
    void                 obtainStocksDatePrice(qint64 timestamp) override;
    void                 obtainTurnover(qint64 timestamp) override;
    void                 obtainPayback(qint64 timestamp) override;

private:
    IStocksDatabase* mStocksDatabase;
    IUserStorage*    mUserStorage;
    QReadWriteLock*  mRwMutex;
    QList<Stock*>    mStocks;
};
