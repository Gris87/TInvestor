#include "src/storage/stocks/stocksstorage.h"

#include <QDebug>

#include "src/threads/parallelhelper/parallelhelperthread.h"



StocksStorage::StocksStorage(IStocksDatabase* stocksDatabase, IUserStorage* userStorage) :
    IStocksStorage(),
    mStocksDatabase(stocksDatabase),
    mUserStorage(userStorage),
    mMutex(new QMutex()),
    mStocks()
{
    qDebug() << "Create StocksStorage";
}

StocksStorage::~StocksStorage()
{
    qDebug() << "Destroy StocksStorage";

    delete mMutex;

    for (int i = 0; i < mStocks.size(); ++i)
    {
        delete mStocks.at(i);
    }
}

void StocksStorage::readFromDatabase()
{
    mStocks = mStocksDatabase->readStocksMeta();
    mStocksDatabase->readStocksData(mStocks);
}

QMutex* StocksStorage::getMutex()
{
    return mMutex;
}

QList<Stock*>& StocksStorage::getStocks()
{
    return mStocks;
}

bool StocksStorage::mergeStocksMeta(const QList<StockMeta>& stocksMeta)
{
    bool changed = false;

    QMap<QString, StockMeta*> existingMetas; // uid => meta
    QList<const StockMeta*>   newMetas;

    for (int i = 0; i < mStocks.size(); ++i)
    {
        StockMeta* existingMeta          = &mStocks[i]->meta;
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
        Stock* stock = new Stock(); // StocksStorage will take ownership

        stock->meta = *newMetas.at(i);

        mStocks.append(stock);
    }

    if (changed)
    {
        mStocksDatabase->writeStocksMeta(mStocks);
    }

    return changed;
}

void StocksStorage::appendStockData(Stock* stock, const StockData* dataArray, int dataArraySize)
{
    mStocksDatabase->appendStockData(stock, dataArray, dataArraySize);
}

struct DeleteObsoleteDataInfo
{
    IStocksDatabase* stocksDatabase;
    qint64           obsoleteTimestamp;
};

void deleteObsoleteDataForParallel(QThread* parentThread, QList<Stock*>& stocks, int start, int end, void* additionalArgs)
{
    DeleteObsoleteDataInfo* deleteObsoleteDataInfo = reinterpret_cast<DeleteObsoleteDataInfo*>(additionalArgs);
    IStocksDatabase*        stocksDatabase         = deleteObsoleteDataInfo->stocksDatabase;
    qint64                  obsoleteTimestamp      = deleteObsoleteDataInfo->obsoleteTimestamp;

    Stock** stockArray = stocks.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock*       stock = stockArray[i];
        QMutexLocker lock(stock->mutex);

        qint64 index = 0; // TODO: Use binary search (from start to end with binary steps (1 2 4 8)

        while (index < stock->data.size() && stock->data.at(index).timestamp < obsoleteTimestamp)
        {
            ++index;
        }

        if (index > 0)
        {
            stock->data.remove(0, index);

            stocksDatabase->writeStockData(*stock);
        }
    }
}

void StocksStorage::deleteObsoleteData(qint64 timestamp)
{
    qDebug() << "Deleting obsolete stocks data";

    DeleteObsoleteDataInfo deleteObsoleteDataInfo;
    deleteObsoleteDataInfo.stocksDatabase    = mStocksDatabase;
    deleteObsoleteDataInfo.obsoleteTimestamp = timestamp;

    processInParallel(mStocks, deleteObsoleteDataForParallel, &deleteObsoleteDataInfo);
}

struct CleanupOperationalDataInfo
{
    qint64 obsoleteTimestamp;
};

void cleanupOperationalDataForParallel(QThread* parentThread, QList<Stock*>& stocks, int start, int end, void* additionalArgs)
{
    CleanupOperationalDataInfo* cleanupOperationalDataInfo = reinterpret_cast<CleanupOperationalDataInfo*>(additionalArgs);
    qint64                      obsoleteTimestamp          = cleanupOperationalDataInfo->obsoleteTimestamp;

    Stock** stockArray = stocks.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock*       stock = stockArray[i];
        QMutexLocker lock(stock->mutex);

        qint64 index = 0; // TODO: Use binary search (from start to end with binary steps (1 2 4 8)

        while (index < stock->operational.detailedData.size() &&
               stock->operational.detailedData.at(index).timestamp < obsoleteTimestamp)
        {
            ++index;
        }

        if (index > 0)
        {
            stock->operational.detailedData.remove(0, index);
        }
    }
}

void StocksStorage::cleanupOperationalData(qint64 timestamp)
{
    qDebug() << "Cleanup operational data";

    CleanupOperationalDataInfo cleanupOperationalDataInfo;
    cleanupOperationalDataInfo.obsoleteTimestamp = timestamp;

    processInParallel(mStocks, cleanupOperationalDataForParallel, &cleanupOperationalDataInfo);
}

struct GetDatePriceInfo
{
    qint64 startTimestamp;
    bool   isDayStartNeeded;
};

void getDatePriceForParallel(QThread* parentThread, QList<Stock*>& stocks, int start, int end, void* additionalArgs)
{
    GetDatePriceInfo* getDatePriceInfo = reinterpret_cast<GetDatePriceInfo*>(additionalArgs);
    qint64            startTimestamp   = getDatePriceInfo->startTimestamp;
    bool              isDayStartNeeded = getDatePriceInfo->isDayStartNeeded;

    Stock** stockArray = stocks.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock*       stock = stockArray[i];
        QMutexLocker lock(stock->mutex);

        // TODO: Use binary search (from end to start with binary steps (1 2 4 8)
        int index = 0;

        for (int i = stock->data.size() - 1; i >= 0; --i)
        {
            if (stock->data.at(i).timestamp <= startTimestamp)
            {
                index = i;

                break;
            }
        }

        if (index < stock->data.size())
        {
            if (isDayStartNeeded)
            {
                stock->operational.dayStartPrice = stock->data.at(index).price;
            }
            else
            {
                stock->operational.specifiedDatePrice = stock->data.at(index).price;
            }
        }
    }
}

void StocksStorage::obtainStocksDayStartPrice(qint64 timestamp)
{
    GetDatePriceInfo getDatePriceInfo;
    getDatePriceInfo.startTimestamp   = timestamp;
    getDatePriceInfo.isDayStartNeeded = true;

    processInParallel(mStocks, getDatePriceForParallel, &getDatePriceInfo);
}

void StocksStorage::obtainStocksDatePrice(qint64 timestamp)
{
    GetDatePriceInfo getDatePriceInfo;
    getDatePriceInfo.startTimestamp   = timestamp;
    getDatePriceInfo.isDayStartNeeded = false;

    processInParallel(mStocks, getDatePriceForParallel, &getDatePriceInfo);
}

struct GetPaybackInfo
{
    IUserStorage* userStorage;
    qint64        startTimestamp;
};

void getPaybackForParallel(QThread* parentThread, QList<Stock*>& stocks, int start, int end, void* additionalArgs)
{
    GetPaybackInfo* getPaybackInfo = reinterpret_cast<GetPaybackInfo*>(additionalArgs);
    IUserStorage*   userStorage    = getPaybackInfo->userStorage;
    qint64          startTimestamp = getPaybackInfo->startTimestamp;

    userStorage->getMutex()->lock();
    float commission = userStorage->getCommission();
    userStorage->getMutex()->unlock();

    Stock** stockArray = stocks.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock*       stock = stockArray[i];
        QMutexLocker lock(stock->mutex);

        // TODO: Use binary search (from end to start with binary steps (1 2 4 8)
        int index = 0;

        for (int i = stock->data.size() - 1; i >= 0; --i)
        {
            if (stock->data.at(i).timestamp <= startTimestamp)
            {
                index = i;

                break;
            }
        }

        if (index < stock->data.size() - 1)
        {
            qint64 goodDeals  = 0;
            qint64 totalDeals = 0;

            for (int i = index; i < stock->data.size() - 1; ++i)
            {
                float expectedPrice = stock->data.at(i).price * (1 + commission * 0.02f); // 2 / 100.0 (2 commissions)

                for (int j = i + 1; j < stock->data.size(); ++j)
                {
                    if (stock->data.at(j).price > expectedPrice)
                    {
                        ++goodDeals;
                    }
                }

                totalDeals += stock->data.size() - i - 1;
            }

            stock->operational.payback = (goodDeals * 100.0f) / totalDeals;
        }
    }
}

void StocksStorage::obtainPayback(qint64 timestamp)
{
    GetPaybackInfo getPaybackInfo;
    getPaybackInfo.userStorage    = mUserStorage;
    getPaybackInfo.startTimestamp = timestamp;

    processInParallel(mStocks, getPaybackForParallel, &getPaybackInfo);
}
