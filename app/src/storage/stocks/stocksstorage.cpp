#include "stocksstorage.h"

#include <QDebug>



StocksStorage::StocksStorage() :
    IStocksStorage()
{
    qDebug() << "Create StocksStorage";
}

StocksStorage::~StocksStorage()
{
    qDebug() << "Destroy StocksStorage";
}

QList<Stock>* StocksStorage::getStocks()
{
    return &mStocks;
}

void StocksStorage::readFromDatabase(IStocksDatabase *stocksDatabase)
{
    mStocks = stocksDatabase->readStocksMeta();
}
