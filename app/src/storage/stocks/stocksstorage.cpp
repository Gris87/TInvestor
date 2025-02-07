#include "stocksstorage.h"

#include <QDebug>



StocksStorage::StocksStorage() :
    IStocksStorage(),
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

QMutex* StocksStorage::getMutex()
{
    return mMutex;
}

QList<Stock>* StocksStorage::getStocks()
{
    return mStocks;
}

void StocksStorage::readFromDatabase(IStocksDatabase *stocksDatabase)
{
    *mStocks = stocksDatabase->readStocksMeta();
    stocksDatabase->readStocksData(mStocks);
}
