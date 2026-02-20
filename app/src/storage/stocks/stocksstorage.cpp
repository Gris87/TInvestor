#include "src/storage/stocks/stocksstorage.h"

#include <QDebug>

#include "src/threads/parallelhelper/parallelhelperthread.h"



constexpr double ONE_DAY_DOUBLE             = 86400000.0; // 24 * 60 * 60 * 1000 // 1 day
constexpr float  TWO_COMMISSIONS_IN_PERCENT = 2 / 100.0f;
constexpr qint64 MS_IN_SECOND               = 1000LL;
constexpr qint64 ONE_MINUTE                 = 60LL * MS_IN_SECOND;
constexpr qint64 ONE_HOUR                   = 60LL * ONE_MINUTE;
constexpr qint64 NIGHT_DELAY                = 2LL * ONE_HOUR; // 2 hours



StocksStorage::StocksStorage(IStocksDatabase* stocksDatabase, IUserStorage* userStorage) :
    IStocksStorage(),
    mRwMutex(new QReadWriteLock()),
    mStocksDatabase(stocksDatabase),
    mUserStorage(userStorage),
    mStocks()
{
    qDebug() << "Create StocksStorage";
}

StocksStorage::~StocksStorage()
{
    qDebug() << "Destroy StocksStorage";

    delete mRwMutex;

    for (const Stock* stock : std::as_const(mStocks))
    {
        delete stock;
    }
}

void StocksStorage::readFromDatabase()
{
    mStocks = mStocksDatabase->readStocksMeta();
    mStocksDatabase->readStocksData(mStocks);
}

void StocksStorage::assignLogos()
{
    mStocksDatabase->assignLogos(mStocks);
}

void StocksStorage::readLock()
{
    mRwMutex->lockForRead();
}

void StocksStorage::readUnlock()
{
    mRwMutex->unlock();
}

void StocksStorage::writeLock()
{
    mRwMutex->lockForWrite();
}

void StocksStorage::writeUnlock()
{
    mRwMutex->unlock();
}

const QList<Stock*>& StocksStorage::getStocks()
{
    return mStocks;
}

void StocksStorage::writeStocksMeta()
{
    mStocksDatabase->writeStocksMeta(mStocks);
}

bool StocksStorage::mergeStocksMeta(const QList<StockMeta>& stocksMeta)
{
    bool changed = false;

    QMap<QString, Stock*>   existingStocks; // UID => Stock
    QList<const StockMeta*> newMetas;

    newMetas.reserve(stocksMeta.size());

    for (Stock* stock : std::as_const(mStocks))
    {
        stock->readLock();
        existingStocks[stock->meta.instrumentId] = stock;
        stock->readUnlock();
    }

    for (const StockMeta& newMeta : stocksMeta)
    {
        if (existingStocks.contains(newMeta.instrumentId))
        {
            Stock* stock = existingStocks[newMeta.instrumentId];
            stock->writeLock();

            if (!stock->meta.compareForMerge(newMeta))
            {
                changed = true;

                stock->meta.merge(newMeta);
            }

            stock->writeUnlock();
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
        stock->meta  = *newMeta;

        mStocks.append(stock);
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

static void deleteObsoleteDataForParallel(
    QThread* parentThread, int /*threadId*/, Stock** stocks, int /*size*/, int start, int end, void* additionalArgs
)
{
    const DeleteObsoleteDataInfo* deleteObsoleteDataInfo = reinterpret_cast<DeleteObsoleteDataInfo*>(additionalArgs);
    IStocksDatabase*              stocksDatabase         = deleteObsoleteDataInfo->stocksDatabase;
    const qint64                  obsoleteTimestamp      = deleteObsoleteDataInfo->obsoleteTimestamp;

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock* stock = stocks[i];

        stock->writeLock();

        Q_ASSERT_X(
            std::is_sorted(
                stock->data.constBegin(),
                stock->data.constEnd(),
                [](const StockData& l, const StockData& r) { return l.timestamp < r.timestamp; }
            ),
            __FUNCTION__,
            "Stock data is unsorted"
        );

        const int index = std::distance(
            stock->data.constBegin(),
            std::lower_bound(
                stock->data.constBegin(),
                stock->data.constEnd(),
                obsoleteTimestamp,
                [](const StockData& stockData, qint64 value) { return stockData.timestamp < value; }
            )
        );

        if (index > 0)
        {
            stock->data.remove(0, index);

            stocksDatabase->writeStockData(*stock);
        }

        stock->writeUnlock();
    }
}

void StocksStorage::deleteObsoleteData(qint64 timestamp)
{
    qDebug() << "Deleting obsolete stocks data";

    DeleteObsoleteDataInfo deleteObsoleteDataInfo(mStocksDatabase, timestamp);
    processInParallel(QThread::currentThread(), mStocks, deleteObsoleteDataForParallel, &deleteObsoleteDataInfo);
}

struct CopyDataToOperationalInfo
{
    explicit CopyDataToOperationalInfo(qint64 _limitTimestamp) :
        limitTimestamp(_limitTimestamp)
    {
    }

    qint64 limitTimestamp;
};

static void copyDataToOperationalForParallel(
    QThread* parentThread, int /*threadId*/, Stock** stocks, int /*size*/, int start, int end, void* additionalArgs
)
{
    const CopyDataToOperationalInfo* copyDataToOperationalInfo = reinterpret_cast<CopyDataToOperationalInfo*>(additionalArgs);
    const qint64                     limitTimestamp            = copyDataToOperationalInfo->limitTimestamp;

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock* stock = stocks[i];

        stock->writeLock();

        for (int j = stock->data.size() - 1; j >= 0; --j)
        {
            const StockData& stockData = stock->data.at(j);

            if (stockData.timestamp < limitTimestamp)
            {
                break;
            }

            const int index = std::distance(
                stock->operational.detailedData.constBegin(),
                std::lower_bound(
                    stock->operational.detailedData.constBegin(),
                    stock->operational.detailedData.constEnd(),
                    stockData.timestamp,
                    [](const StockOperationalData& stockData, qint64 value) { return stockData.timestamp < value; }
                )
            );

            StockOperationalData stockOperationalData; // NOLINT(cppcoreguidelines-pro-type-member-init)

            stockOperationalData.timestamp = stockData.timestamp;
            stockOperationalData.price     = stockData.price;

            stock->operational.detailedData.insert(index, stockOperationalData);
        }

        Q_ASSERT_X(
            std::is_sorted(
                stock->operational.detailedData.constBegin(),
                stock->operational.detailedData.constEnd(),
                [](const StockOperationalData& l, const StockOperationalData& r) { return l.timestamp < r.timestamp; }
            ),
            __FUNCTION__,
            "Stock data is unsorted"
        );

        for (int i = 0; i < stock->operational.detailedData.size() - 1; ++i)
        {
            const qint64 prevMinute = stock->operational.detailedData.at(i).timestamp / ONE_MINUTE;
            const qint64 nextMinute = stock->operational.detailedData.at(i + 1).timestamp / ONE_MINUTE;

            if (nextMinute == prevMinute)
            {
                stock->operational.detailedData.removeAt(i);
                --i;
            }
        }

        stock->writeUnlock();
    }
}

void StocksStorage::copyDataToOperational(qint64 timestamp)
{
    qDebug() << "Copy data to operational";

    CopyDataToOperationalInfo copyDataToOperationalInfo(timestamp);
    processInParallel(QThread::currentThread(), mStocks, copyDataToOperationalForParallel, &copyDataToOperationalInfo);
}

struct CleanupOperationalDataInfo
{
    explicit CleanupOperationalDataInfo(qint64 _obsoleteTimestamp) :
        obsoleteTimestamp(_obsoleteTimestamp)
    {
    }

    qint64 obsoleteTimestamp;
};

static void cleanupOperationalDataForParallel(
    QThread* parentThread, int /*threadId*/, Stock** stocks, int /*size*/, int start, int end, void* additionalArgs
)
{
    const CleanupOperationalDataInfo* cleanupOperationalDataInfo = reinterpret_cast<CleanupOperationalDataInfo*>(additionalArgs);
    const qint64                      obsoleteTimestamp          = cleanupOperationalDataInfo->obsoleteTimestamp;

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock* stock = stocks[i];

        stock->writeLock();

        Q_ASSERT_X(
            std::is_sorted(
                stock->operational.detailedData.constBegin(),
                stock->operational.detailedData.constEnd(),
                [](const StockOperationalData& l, const StockOperationalData& r) { return l.timestamp < r.timestamp; }
            ),
            __FUNCTION__,
            "Stock data is unsorted"
        );

        const int index = std::distance(
            stock->operational.detailedData.constBegin(),
            std::lower_bound(
                stock->operational.detailedData.constBegin(),
                stock->operational.detailedData.constEnd(),
                obsoleteTimestamp,
                [](const StockOperationalData& stockData, qint64 value) { return stockData.timestamp < value; }
            )
        );

        if (index > 0)
        {
            stock->operational.detailedData.remove(0, index);
        }

        stock->writeUnlock();
    }
}

void StocksStorage::cleanupOperationalData(qint64 timestamp)
{
    qDebug() << "Cleanup operational data";

    CleanupOperationalDataInfo cleanupOperationalDataInfo(timestamp);
    processInParallel(QThread::currentThread(), mStocks, cleanupOperationalDataForParallel, &cleanupOperationalDataInfo);
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

static void getDatePriceForParallel(
    QThread* parentThread, int /*threadId*/, Stock** stocks, int /*size*/, int start, int end, void* additionalArgs
)
{
    const GetDatePriceInfo* getDatePriceInfo = reinterpret_cast<GetDatePriceInfo*>(additionalArgs);
    const qint64            startTimestamp   = getDatePriceInfo->startTimestamp;
    const bool              isDayStartNeeded = getDatePriceInfo->isDayStartNeeded;

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock* stock = stocks[i];

        stock->writeLock();

        Q_ASSERT_X(
            std::is_sorted(
                stock->data.constBegin(),
                stock->data.constEnd(),
                [](const StockData& l, const StockData& r) { return l.timestamp < r.timestamp; }
            ),
            __FUNCTION__,
            "Stock data is unsorted"
        );

        int index = std::distance(
            stock->data.constBegin(),
            std::lower_bound(
                stock->data.constBegin(), stock->data.constEnd(), startTimestamp, [](const StockData& stockData, qint64 value) {
                    return stockData.timestamp <= value;
                }
            )
        );

        if (index > 0)
        {
            --index;
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

        stock->writeUnlock();
    }
}

void StocksStorage::obtainStocksDayStartPrice(qint64 timestamp)
{
    GetDatePriceInfo getDatePriceInfo(timestamp, true);
    processInParallel(QThread::currentThread(), mStocks, getDatePriceForParallel, &getDatePriceInfo);
}

void StocksStorage::obtainStocksDatePrice(qint64 timestamp)
{
    GetDatePriceInfo getDatePriceInfo(timestamp, false);
    processInParallel(QThread::currentThread(), mStocks, getDatePriceForParallel, &getDatePriceInfo);
}

struct GetLastTradeTimeInfo
{
    explicit GetLastTradeTimeInfo(qint64 _startTimestamp) :
        startTimestamp(_startTimestamp)
    {
    }

    qint64 startTimestamp;
};

static void getLastTradeTimeForParallel(
    QThread* parentThread, int /*threadId*/, Stock** stocks, int /*size*/, int start, int end, void* additionalArgs
)
{
    const GetLastTradeTimeInfo* getLastTradeTimeInfo = reinterpret_cast<GetLastTradeTimeInfo*>(additionalArgs);
    const qint64                startTimestamp       = getLastTradeTimeInfo->startTimestamp;

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock* stock = stocks[i];

        stock->writeLock();

        const StockData* stockData = stock->data.constData();

        QTime lastTradeTime(0, 0);

        for (int j = stock->data.size() - 2; j >= 0 && !parentThread->isInterruptionRequested(); --j)
        {
            const qint64 timestamp = stockData[j].timestamp;

            if (timestamp < startTimestamp)
            {
                break;
            }

            const qint64 nextTimestamp = stockData[j + 1].timestamp;

            if (nextTimestamp - timestamp >= NIGHT_DELAY)
            {
                lastTradeTime = qMax(lastTradeTime, QDateTime::fromMSecsSinceEpoch(timestamp).time());
            }
        }

        stock->meta.lastTradeTime = lastTradeTime;
        stock->writeUnlock();
    }
}

void StocksStorage::obtainLastTradeTime(qint64 timestamp)
{
    GetLastTradeTimeInfo getLastTradeTimeInfo(timestamp);
    processInParallel(QThread::currentThread(), mStocks, getLastTradeTimeForParallel, &getLastTradeTimeInfo);
}

struct GetTurnoverInfo
{
    explicit GetTurnoverInfo(qint64 _startTimestamp) :
        startTimestamp(_startTimestamp)
    {
    }

    qint64 startTimestamp;
};

static void getTurnoverForParallel(
    QThread* parentThread, int /*threadId*/, Stock** stocks, int /*size*/, int start, int end, void* additionalArgs
)
{
    const GetTurnoverInfo* getTurnoverInfo = reinterpret_cast<GetTurnoverInfo*>(additionalArgs);
    const qint64           startTimestamp  = getTurnoverInfo->startTimestamp;

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock* stock = stocks[i];

        stock->writeLock();

        Q_ASSERT_X(
            std::is_sorted(
                stock->data.constBegin(),
                stock->data.constEnd(),
                [](const StockData& l, const StockData& r) { return l.timestamp < r.timestamp; }
            ),
            __FUNCTION__,
            "Stock data is unsorted"
        );

        int index = std::distance(
            stock->data.constBegin(),
            std::lower_bound(
                stock->data.constBegin(), stock->data.constEnd(), startTimestamp, [](const StockData& stockData, qint64 value) {
                    return stockData.timestamp <= value;
                }
            )
        );

        if (index > 0)
        {
            --index;
        }

        if (index < stock->data.size())
        {
            qint64       totalTurnover = 0;
            const qint64 lot           = stock->meta.lot;

            for (int i = index; i < stock->data.size(); ++i)
            {
                totalTurnover += qRound64(
                    static_cast<double>(lot * stock->data.at(i).quantity) * static_cast<double>(stock->data.at(i).price)
                );
            }

            const qint64 deltaTimestamp = stock->data.constLast().timestamp - stock->data.at(index).timestamp;

            stock->meta.turnover = deltaTimestamp > 0 ? qRound64(totalTurnover * (ONE_DAY_DOUBLE / deltaTimestamp)) : 0;
        }

        stock->writeUnlock();
    }
}

void StocksStorage::obtainTurnover(qint64 timestamp)
{
    GetTurnoverInfo getTurnoverInfo(timestamp);
    processInParallel(QThread::currentThread(), mStocks, getTurnoverForParallel, &getTurnoverInfo);
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

static void getPaybackForParallel(
    QThread* parentThread, int /*threadId*/, Stock** stocks, int /*size*/, int start, int end, void* additionalArgs
)
{
    const GetPaybackInfo* getPaybackInfo = reinterpret_cast<GetPaybackInfo*>(additionalArgs);
    IUserStorage*         userStorage    = getPaybackInfo->userStorage;
    const qint64          startTimestamp = getPaybackInfo->startTimestamp;

    userStorage->readLock();
    const float commission = userStorage->getCommission();
    userStorage->readUnlock();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock* stock = stocks[i];

        stock->writeLock();

        Q_ASSERT_X(
            std::is_sorted(
                stock->data.constBegin(),
                stock->data.constEnd(),
                [](const StockData& l, const StockData& r) { return l.timestamp < r.timestamp; }
            ),
            __FUNCTION__,
            "Stock data is unsorted"
        );

        int index = std::distance(
            stock->data.constBegin(),
            std::lower_bound(
                stock->data.constBegin(), stock->data.constEnd(), startTimestamp, [](const StockData& stockData, qint64 value) {
                    return stockData.timestamp <= value;
                }
            )
        );

        if (index > 0)
        {
            --index;
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

        stock->writeUnlock();
    }
}

void StocksStorage::obtainPayback(qint64 timestamp)
{
    GetPaybackInfo getPaybackInfo(mUserStorage, timestamp);
    processInParallel(QThread::currentThread(), mStocks, getPaybackForParallel, &getPaybackInfo);
}
