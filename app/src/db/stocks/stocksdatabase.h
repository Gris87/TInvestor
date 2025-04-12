#pragma once



#include "src/db/stocks/istocksdatabase.h"

#include "src/utils/fs/dir/idirfactory.h"
#include "src/utils/fs/file/ifilefactory.h"



class StocksDatabase : public IStocksDatabase
{
public:
    explicit StocksDatabase(IDirFactory* dirFactory, IFileFactory* fileFactory);
    ~StocksDatabase() override;

    StocksDatabase(const StocksDatabase& another)            = delete;
    StocksDatabase& operator=(const StocksDatabase& another) = delete;

    QList<Stock*> readStocksMeta() override;
    void          readStocksData(QList<Stock*>& stocks) override;
    void          writeStocksMeta(const QList<Stock*>& stocks) override;
    void          appendStockData(Stock* stock, const StockData* dataArray, int dataArraySize) override;
    void          writeStockData(const Stock& stock) override;

private:
    IFileFactory* mFileFactory;
};
