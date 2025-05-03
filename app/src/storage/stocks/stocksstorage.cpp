#include "src/storage/stocks/stocksstorage.h"

#include <QDebug>

#include "src/threads/parallelhelper/parallelhelperthread.h"



constexpr double ONE_DAY_DOUBLE             = 86400000.0; // 24 * 60 * 60 * 1000 // 1 day
constexpr float  TWO_COMMISSIONS_IN_PERCENT = 2 / 100.0f;



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

    for (Stock* stock : std::as_const(mStocks))
    {
        delete stock;
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

    QMap<QString, StockMeta*> existingMetas; // UID => Stock meta
    QList<const StockMeta*>   newMetas;

    newMetas.reserve(stocksMeta.size());

    for (Stock* stock : std::as_const(mStocks))
    {
        StockMeta* existingMeta          = &stock->meta;
        existingMetas[existingMeta->uid] = existingMeta;
    }

    for (const StockMeta& newMeta : stocksMeta)
    {
        if (existingMetas.contains(newMeta.uid))
        {
            StockMeta* existingMeta = existingMetas[newMeta.uid];

            if (*existingMeta != newMeta)
            {
                changed = true;

                *existingMeta = newMeta;
            }
        }
        else
        {
            changed = true;

            newMetas.append(&newMeta);
        }
    }

    for (const StockMeta* newMeta : newMetas)
    {
        Stock* stock = new Stock(); // StocksStorage will take ownership

        stock->meta = *newMeta;

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
    explicit DeleteObsoleteDataInfo(IStocksDatabase* _stocksDatabase, qint64 _obsoleteTimestamp) :
        stocksDatabase(_stocksDatabase),
        obsoleteTimestamp(_obsoleteTimestamp)
    {
    }

    IStocksDatabase* stocksDatabase;
    qint64           obsoleteTimestamp;
};

static void deleteObsoleteDataForParallel(QThread* parentThread, QList<Stock*>& stocks, int start, int end, void* additionalArgs)
{
    DeleteObsoleteDataInfo* deleteObsoleteDataInfo = reinterpret_cast<DeleteObsoleteDataInfo*>(additionalArgs);
    IStocksDatabase*        stocksDatabase         = deleteObsoleteDataInfo->stocksDatabase;
    const qint64            obsoleteTimestamp      = deleteObsoleteDataInfo->obsoleteTimestamp;

    Stock** stockArray = stocks.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock*             stock = stockArray[i];
        const QMutexLocker lock(stock->mutex);

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

    DeleteObsoleteDataInfo deleteObsoleteDataInfo(mStocksDatabase, timestamp);
    processInParallel(mStocks, deleteObsoleteDataForParallel, &deleteObsoleteDataInfo);
}

struct CleanupOperationalDataInfo
{
    explicit CleanupOperationalDataInfo(qint64 _obsoleteTimestamp) :
        obsoleteTimestamp(_obsoleteTimestamp)
    {
    }

    qint64 obsoleteTimestamp;
};

static void
cleanupOperationalDataForParallel(QThread* parentThread, QList<Stock*>& stocks, int start, int end, void* additionalArgs)
{
    CleanupOperationalDataInfo* cleanupOperationalDataInfo = reinterpret_cast<CleanupOperationalDataInfo*>(additionalArgs);
    const qint64                obsoleteTimestamp          = cleanupOperationalDataInfo->obsoleteTimestamp;

    Stock** stockArray = stocks.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock*             stock = stockArray[i];
        const QMutexLocker lock(stock->mutex);

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

    CleanupOperationalDataInfo cleanupOperationalDataInfo(timestamp);
    processInParallel(mStocks, cleanupOperationalDataForParallel, &cleanupOperationalDataInfo);
}

struct GetDatePriceInfo
{
    explicit GetDatePriceInfo(qint64 _startTimestamp, bool _isDayStartNeeded) :
        startTimestamp(_startTimestamp),
        isDayStartNeeded(_isDayStartNeeded)
    {
    }

    qint64 startTimestamp;
    bool   isDayStartNeeded;
};

static void getDatePriceForParallel(QThread* parentThread, QList<Stock*>& stocks, int start, int end, void* additionalArgs)
{
    GetDatePriceInfo* getDatePriceInfo = reinterpret_cast<GetDatePriceInfo*>(additionalArgs);
    const qint64      startTimestamp   = getDatePriceInfo->startTimestamp;
    const bool        isDayStartNeeded = getDatePriceInfo->isDayStartNeeded;

    Stock** stockArray = stocks.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock*             stock = stockArray[i];
        const QMutexLocker lock(stock->mutex);

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
    GetDatePriceInfo getDatePriceInfo(timestamp, true);
    processInParallel(mStocks, getDatePriceForParallel, &getDatePriceInfo);
}

void StocksStorage::obtainStocksDatePrice(qint64 timestamp)
{
    GetDatePriceInfo getDatePriceInfo(timestamp, false);
    processInParallel(mStocks, getDatePriceForParallel, &getDatePriceInfo);
}

struct GetTurnoverInfo
{
    explicit GetTurnoverInfo(qint64 _startTimestamp) :
        startTimestamp(_startTimestamp)
    {
    }

    qint64 startTimestamp;
};

static void getTurnoverForParallel(QThread* parentThread, QList<Stock*>& stocks, int start, int end, void* additionalArgs)
{
    GetTurnoverInfo* getTurnoverInfo = reinterpret_cast<GetTurnoverInfo*>(additionalArgs);
    const qint64     startTimestamp  = getTurnoverInfo->startTimestamp;

    Stock** stockArray = stocks.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock*             stock = stockArray[i];
        const QMutexLocker lock(stock->mutex);

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
            qint64 totalTurnover = 0;

            for (int i = index; i < stock->data.size(); ++i)
            {
                totalTurnover += qRound64(stock->data.at(i).quantity * stock->data.at(i).price);
            }

            const qint64 deltaTimestamp = stock->data.constLast().timestamp - stock->data.at(index).timestamp;

            stock->operational.turnover = deltaTimestamp > 0 ? qRound64(totalTurnover * (ONE_DAY_DOUBLE / deltaTimestamp)) : 0;
        }
    }
}

void StocksStorage::obtainTurnover(qint64 timestamp)
{
    GetTurnoverInfo getTurnoverInfo(timestamp);
    processInParallel(mStocks, getTurnoverForParallel, &getTurnoverInfo);
}

struct GetPaybackInfo
{
    explicit GetPaybackInfo(IUserStorage* _userStorage, qint64 _startTimestamp) :
        userStorage(_userStorage),
        startTimestamp(_startTimestamp)
    {
    }

    IUserStorage* userStorage;
    qint64        startTimestamp;
};

static void getPaybackForParallel(QThread* parentThread, QList<Stock*>& stocks, int start, int end, void* additionalArgs)
{
    GetPaybackInfo* getPaybackInfo = reinterpret_cast<GetPaybackInfo*>(additionalArgs);
    IUserStorage*   userStorage    = getPaybackInfo->userStorage;
    const qint64    startTimestamp = getPaybackInfo->startTimestamp;

    userStorage->getMutex()->lock();
    const float commission = userStorage->getCommission();
    userStorage->getMutex()->unlock();

    Stock** stockArray = stocks.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock*             stock = stockArray[i];
        const QMutexLocker lock(stock->mutex);

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
                const float expectedPrice = stock->data.at(i).price * (1 + commission * TWO_COMMISSIONS_IN_PERCENT);

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
    GetPaybackInfo getPaybackInfo(mUserStorage, timestamp);
    processInParallel(mStocks, getPaybackForParallel, &getPaybackInfo);
}
