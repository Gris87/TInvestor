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

void StocksStorage::mergeStocksMeta(const QList<StockMeta>& stocksMeta)
{
    bool changed = false;

    QMap<QString, StockMeta*> existingMetas; // uid => meta
    QList<const StockMeta*>   newMetas;      // uid => meta

    for (int i = 0; i < mStocks->size(); ++i)
    {
        StockMeta* existingMeta          = &(*mStocks)[i].meta;
        existingMetas[existingMeta->uid] = existingMeta;
    }

    for (int i = 0; i < stocksMeta.size(); ++i)
    {
        const StockMeta* newMeta = &stocksMeta.at(i);

        if (existingMetas.contains(newMeta->uid))
        {
            StockMeta* existingMeta = existingMetas[newMeta->uid];

            if (*existingMeta != *newMeta)
            {
                changed = true;

                *existingMeta = *newMeta;
            }
        }
        else
        {
            changed = true;

            newMetas.append(newMeta);
        }
    }

    for (int i = 0; i < newMetas.size(); ++i)
    {
        Stock stock;

        stock.meta = *newMetas.at(i);

        mStocks->append(stock);
    }

    if (changed)
    {
        mStocksDatabase->writeStocksMeta(mStocks);
    }
}
