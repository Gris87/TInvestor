#include "src/storage/stocks/stocksstorage.h"

#include <QDebug>



StocksStorage::StocksStorage(IStocksDatabase* stocksDatabase) :
    IStocksStorage(),
    mStocksDatabase(stocksDatabase),
    mMutex(new QMutex()),
    mStocks(new QList<Stock>())
{
    qDebug() << "Create StocksStorage";
}

StocksStorage::~StocksStorage()
{
    qDebug() << "Destroy StocksStorage";

    delete mMutex;
    delete mStocks;
}

void StocksStorage::readFromDatabase()
{
    *mStocks = mStocksDatabase->readStocksMeta();
    mStocksDatabase->readStocksData(mStocks);
}

QMutex* StocksStorage::getMutex()
{
    return mMutex;
}

QList<Stock>* StocksStorage::getStocks()
{
    return mStocks;
}
