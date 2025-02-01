#pragma once



#include <QList>

#include "src/domain/stocks/stock.h"



class IStocksDatabase
{
public:
    explicit IStocksDatabase() {}
    virtual ~IStocksDatabase() = default;

    IStocksDatabase(const IStocksDatabase &another) = delete;
    IStocksDatabase& operator=(const IStocksDatabase &another) = delete;

    virtual void createStockTable(const QString &name) = 0;
    virtual QList<Stock> readStocks() = 0;
    virtual void readStocksData(QList<Stock> &stocks) = 0;
    virtual void deleteObsoleteData(qint64 obsoleteTimestamp, const QList<Stock> &stocks) = 0;
};
