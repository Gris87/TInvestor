#include "src/db/stocks/stocksdatabase.h"

#include <QCoreApplication>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "src/threads/parallelhelper/parallelhelperthread.h"



constexpr qsizetype AMOUNT_OF_DATA_PER_DAY = 24LL * 60LL; // Amount of minutes in day



StocksDatabase::StocksDatabase(IDirFactory* dirFactory, IFileFactory* fileFactory, ILogosStorage* logosStorage) :
    IStocksDatabase(),
    mFileFactory(fileFactory),
    mLogosStorage(logosStorage)
{
    qDebug() << "Create StocksDatabase";

    const std::shared_ptr<IDir> dir = dirFactory->newInstance();

    const bool ok = dir->mkpath(qApp->applicationDirPath() + "/data/stocks");
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to create dir");
}

StocksDatabase::~StocksDatabase()
{
    qDebug() << "Destroy StocksDatabase";
}

QList<Stock*> StocksDatabase::readStocksMeta()
{
    qDebug() << "Reading stocks from database";

    QList<Stock*> res;

    const std::shared_ptr<IFile> stocksFile = mFileFactory->newInstance(qApp->applicationDirPath() + "/data/stocks/stocks.json");

    if (stocksFile->open(QIODevice::ReadOnly))
    {
        const QByteArray content = stocksFile->readAll();
        stocksFile->close();

        const simdjson::padded_string jsonData(content.toStdString());

        simdjson::ondemand::parser parser;

        try
        {
            simdjson::ondemand::document doc = parser.iterate(jsonData);

            simdjson::ondemand::array jsonStocks = doc.get_array();
            res.resizeForOverwrite(jsonStocks.count_elements());

            int i = 0;

            for (const simdjson::ondemand::object jsonObject : jsonStocks)
            {
                Stock* stock = new Stock();
                stock->meta.fromJsonObject(jsonObject);
                res[i] = stock;

                qDebug() << "Read stock" << stock->meta.instrumentTicker << ":" << stock->meta.instrumentName;

                ++i;
            }
        }
        catch (...)
        {
            qWarning() << "Failed to parse stocks";
        }
    }

    return res;
}

struct ReadStocksDataInfo
{
    explicit ReadStocksDataInfo(IFileFactory* _fileFactory) :
        fileFactory(_fileFactory)
    {
    }

    IFileFactory* fileFactory;
};

static void readStocksDataForParallel(
    QThread* parentThread, int /*threadId*/, QList<Stock*>& stocks, int start, int end, void* additionalArgs
)
{
    ReadStocksDataInfo* readStocksDataInfo = reinterpret_cast<ReadStocksDataInfo*>(additionalArgs);
    IFileFactory*       fileFactory        = readStocksDataInfo->fileFactory;

    const QString appDir = qApp->applicationDirPath();

    Stock** stockArray = stocks.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock* stock = stockArray[i];

        const std::shared_ptr<IFile> stockDataFile =
            fileFactory->newInstance(QString("%1/data/stocks/%2.dat").arg(appDir, stock->meta.instrumentId));

        if (stockDataFile->open(QIODevice::ReadOnly))
        {
            const qint64    fileSize = stockDataFile->size();
            const qsizetype dataSize = fileSize / sizeof(StockData);

            stock->data.reserve(dataSize + AMOUNT_OF_DATA_PER_DAY);
            stock->data.resizeForOverwrite(dataSize);

            stockDataFile->read(reinterpret_cast<char*>(stock->data.data()), fileSize);
            stockDataFile->close();

            Q_ASSERT_X(
                std::is_sorted(
                    stock->data.constBegin(),
                    stock->data.constEnd(),
                    [](const StockData& l, const StockData& r) { return l.timestamp < r.timestamp; }
                ),
                __FUNCTION__,
                "Stock data is unsorted"
            );

            qDebug() << "Read stock data" << stock->meta.instrumentTicker;
        }
        else
        {
            stock->data.clear();

            qWarning() << "Failed to read stock data" << stock->meta.instrumentTicker;
        }

        stock->operational.lastStoredTimestamp = !stock->data.isEmpty() ? stock->data.constLast().timestamp : 0;
    }
}

void StocksDatabase::readStocksData(QList<Stock*>& stocks)
{
    qDebug() << "Reading stocks data from database";

    ReadStocksDataInfo readStocksDataInfo(mFileFactory);
    processInParallel(stocks, readStocksDataForParallel, &readStocksDataInfo);
}

struct AssignLogosInfo
{
    explicit AssignLogosInfo(ILogosStorage* _logosStorage) :
        logosStorage(_logosStorage)
    {
    }

    ILogosStorage* logosStorage;
};

static void
assignLogosForParallel(QThread* parentThread, int /*threadId*/, QList<Stock*>& stocks, int start, int end, void* additionalArgs)
{
    AssignLogosInfo* assignLogosInfo = reinterpret_cast<AssignLogosInfo*>(additionalArgs);
    ILogosStorage*   logosStorage    = assignLogosInfo->logosStorage;

    Stock** stockArray = stocks.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock* stock = stockArray[i];

        stock->meta.instrumentLogo = logosStorage->getLogo(stock->meta.instrumentId);
    }
}

void StocksDatabase::assignLogos(QList<Stock*>& stocks)
{
    mLogosStorage->readLock();

    AssignLogosInfo assignLogosInfo(mLogosStorage);
    processInParallel(stocks, assignLogosForParallel, &assignLogosInfo);

    mLogosStorage->readUnlock();
}

void StocksDatabase::writeStocksMeta(const QList<Stock*>& stocks)
{
    qDebug() << "Writing stocks to database";

    QJsonArray jsonStocks;

    for (Stock* stock : stocks)
    {
        jsonStocks.append(stock->meta.toJsonObject());
    }

    const QJsonDocument jsonDoc(jsonStocks);

    const std::shared_ptr<IFile> stocksFile = mFileFactory->newInstance(qApp->applicationDirPath() + "/data/stocks/stocks.json");

    const bool ok = stocksFile->open(QIODevice::WriteOnly);
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to open file");

    stocksFile->write(jsonDoc.toJson(QJsonDocument::Compact));
    stocksFile->close();
}

void StocksDatabase::appendStockData(Stock* stock, const StockData* dataArray, int dataArraySize)
{
    const QString stockDataFilePath = QString("%1/data/stocks/%2.dat").arg(qApp->applicationDirPath(), stock->meta.instrumentId);
    const std::shared_ptr<IFile> stockDataFile = mFileFactory->newInstance(stockDataFilePath);

    const bool ok = stockDataFile->open(QIODevice::Append);
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to open file");

    stockDataFile->write(reinterpret_cast<const char*>(dataArray), dataArraySize * sizeof(StockData));
    stockDataFile->close();

    for (int i = 0; i < dataArraySize; ++i)
    {
        stock->data.append(dataArray[i]);
    }

    Q_ASSERT_X(
        std::is_sorted(
            stock->data.constBegin(),
            stock->data.constEnd(),
            [](const StockData& l, const StockData& r) { return l.timestamp < r.timestamp; }
        ),
        __FUNCTION__,
        "Stock data is unsorted"
    );

    stock->operational.lastStoredTimestamp = !stock->data.isEmpty() ? stock->data.constLast().timestamp : 0;
}

void StocksDatabase::writeStockData(const Stock& stock)
{
    Q_ASSERT_X(
        std::is_sorted(
            stock.data.constBegin(),
            stock.data.constEnd(),
            [](const StockData& l, const StockData& r) { return l.timestamp < r.timestamp; }
        ),
        __FUNCTION__,
        "Stock data is unsorted"
    );

    const QString stockDataFilePath = QString("%1/data/stocks/%2.dat").arg(qApp->applicationDirPath(), stock.meta.instrumentId);
    const std::shared_ptr<IFile> stockDataFile = mFileFactory->newInstance(stockDataFilePath);

    const bool ok = stockDataFile->open(QIODevice::WriteOnly);
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to open file");

    stockDataFile->write(reinterpret_cast<const char*>(stock.data.constData()), stock.data.size() * sizeof(StockData));
    stockDataFile->close();
}
