#pragma once



#include "src/db/stocks/istocksdatabase.h"



class StocksDatabase : public IStocksDatabase
{
public:
    explicit StocksDatabase();
    ~StocksDatabase();

    StocksDatabase(const StocksDatabase &another) = delete;
    StocksDatabase& operator=(const StocksDatabase &another) = delete;

    QList<Stock> readStocksMeta() override;
    void readStocksData(QList<Stock> *stocks) override;
    void writeStocksMeta(QList<Stock> *stocks) override;
    void appendStockData(Stock *stock) override;
    void deleteObsoleteData(qint64 obsoleteTimestamp, QList<Stock> *stocks) override;

private:
    void writeStockData(Stock *stock);
    void fillWithTestData();
};
