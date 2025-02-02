#include "stockstorage.h"

#include <QDebug>



StockStorage::StockStorage() :
    IStockStorage()
{
    qDebug() << "Create StockStorage";
}

StockStorage::~StockStorage()
{
    qDebug() << "Destroy StockStorage";
}
