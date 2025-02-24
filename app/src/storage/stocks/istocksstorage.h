#pragma once



#include "src/domain/stocks/stocks.h"

#include <QMutex>



class IStocksStorage
{
public:
    IStocksStorage()
    {
    }
    virtual ~IStocksStorage() = default;

    IStocksStorage(const IStocksStorage& another)            = delete;
    IStocksStorage& operator=(const IStocksStorage& another) = delete;

    virtual void          readFromDatabase() = 0;
    virtual QMutex*       getMutex()         = 0;
    virtual QList<Stock>* getStocks()        = 0;
    virtual void          mergeStocksMeta(const QList<StockMeta>& stocksMeta) = 0;
};
