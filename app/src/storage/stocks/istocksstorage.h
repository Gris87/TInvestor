#pragma once



#include "src/db/stocks/istocksdatabase.h"

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

    virtual QMutex*       getMutex()                                        = 0;
    virtual QList<Stock>* getStocks()                                       = 0;
    virtual void          readFromDatabase(IStocksDatabase* stocksDatabase) = 0;
};
