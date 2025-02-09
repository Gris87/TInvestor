#pragma once



#include "src/db/stocks/istocksdatabase.h"

#include "src/fs/dir/idirfactory.h"
#include "src/fs/file/ifilefactory.h"



class StocksDatabase : public IStocksDatabase
{
public:
    explicit StocksDatabase(IDirFactory* dirFactory, IFileFactory* fileFactory);
    ~StocksDatabase();

    StocksDatabase(const StocksDatabase& another)            = delete;
    StocksDatabase& operator=(const StocksDatabase& another) = delete;

    QList<Stock> readStocksMeta() override;
    void         readStocksData(QList<Stock>* stocks) override;
    void         writeStocksMeta(QList<Stock>* stocks) override;
    void         appendStockData(Stock* stock) override;
    void         deleteObsoleteData(qint64 obsoleteTimestamp, QList<Stock>* stocks) override;

private:
    void fillWithTestData();

    IFileFactory* mFileFactory;
};
