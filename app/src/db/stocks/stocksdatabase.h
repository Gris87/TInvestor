#pragma once



#include "src/db/stocks/istocksdatabase.h"

#include <QtSql>



class StocksDatabase : public IStocksDatabase
{
public:
    explicit StocksDatabase();
    ~StocksDatabase();

    StocksDatabase(const StocksDatabase &another) = delete;
    StocksDatabase& operator=(const StocksDatabase &another) = delete;

    void createStockTable(const QString &name) override;
    void readStocks(QList<Stock> *stocks) override;
    void deleteObsoleteData(qint64 obsoleteTimestamp, const QList<Stock> &stocks) override;

private:
    void createListTable();
    void fillWithTestData();

    QSqlDatabase mDB;
};
