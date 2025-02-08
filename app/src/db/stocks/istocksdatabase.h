#pragma once



#include <QList>

#include "src/domain/stocks/stock.h"



class IStocksDatabase
{
public:
    explicit IStocksDatabase()
    {
    }
    virtual ~IStocksDatabase() = default;

    IStocksDatabase(const IStocksDatabase& another)            = delete;
    IStocksDatabase& operator=(const IStocksDatabase& another) = delete;

    virtual QList<Stock> readStocksMeta()                                                   = 0;
    virtual void         readStocksData(QList<Stock>* stocks)                               = 0;
    virtual void         writeStocksMeta(QList<Stock>* stocks)                              = 0;
    virtual void         appendStockData(Stock* stock)                                      = 0;
    virtual void         deleteObsoleteData(qint64 obsoleteTimestamp, QList<Stock>* stocks) = 0;
};
