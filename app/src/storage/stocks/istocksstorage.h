#pragma once



#include "src/domain/stocks/stock.h"

#include <QMutex>



class IStocksStorage
{
public:
    IStocksStorage()          = default;
    virtual ~IStocksStorage() = default;

    IStocksStorage(const IStocksStorage& another)            = delete;
    IStocksStorage& operator=(const IStocksStorage& another) = delete;

    virtual void           readFromDatabase()                                                           = 0;
    virtual QMutex*        getMutex()                                                                   = 0;
    virtual QList<Stock*>& getStocks()                                                                  = 0;
    virtual bool           mergeStocksMeta(const QList<StockMeta>& stocksMeta)                          = 0;
    virtual void           appendStockData(Stock* stock, const StockData* dataArray, int dataArraySize) = 0;
    virtual void           deleteObsoleteData(qint64 timestamp)                                         = 0;
    virtual void           cleanupOperationalData(qint64 timestamp)                                     = 0;
    virtual void           obtainStocksDayStartPrice(qint64 timestamp)                                  = 0;
    virtual void           obtainStocksDatePrice(qint64 timestamp)                                      = 0;
    virtual void           obtainTurnover(qint64 timestamp)                                             = 0;
    virtual void           obtainPayback(qint64 timestamp)                                              = 0;
};
