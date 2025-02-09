#include "src/db/stocks/stocksdatabase.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>

#include "src/threads/parallelhelper/parallelhelperthread.h"



const int amountOfDataPerDay = 24 * 60;



StocksDatabase::StocksDatabase(IDirFactory* dirFactory, IFileFactory* fileFactory) :
    IStocksDatabase(),
    mFileFactory(fileFactory)
{
    qDebug() << "Create StocksDatabase";

    IDir* dir = dirFactory->newInstance();

    bool ok = dir->mkpath(qApp->applicationDirPath() + "/data/db/stocks");
    Q_ASSERT_X(ok, "StocksDatabase::StocksDatabase()", "Failed to create dir");

    // fillWithTestData();

    delete dir;
}

StocksDatabase::~StocksDatabase()
{
    qDebug() << "Destroy StocksDatabase";
}

QList<Stock> StocksDatabase::readStocksMeta()
{
    qDebug() << "Reading stocks from database";

    QList<Stock> res;

    IFile* stocksFile = mFileFactory->newInstance(qApp->applicationDirPath() + "/data/db/stocks/stocks.lst");

    if (stocksFile->open(QIODevice::ReadOnly))
    {
        QString stockStr = QString::fromUtf8(stocksFile->readAll());
        stocksFile->close();

        QStringList stockLines = stockStr.split('\n');

        for (int i = 0; i < stockLines.size() - 1; i += 2)
        {
            Stock stock;

            stock.name     = stockLines.at(i);
            stock.fullname = stockLines.at(i + 1);

            res.append(stock);

            qDebug() << "Read stock" << stock.name << ":" << stock.fullname;
        }
    }

    delete stocksFile;

    return res;
}

struct ReadStocksDataInfo
{
    IFileFactory* fileFactory;
};

void readStocksDataForParallel(QList<Stock>* stocks, int start, int end, void* additionalArgs)
{
    ReadStocksDataInfo* readStocksDataInfo = reinterpret_cast<ReadStocksDataInfo*>(additionalArgs);
    IFileFactory*       fileFactory        = readStocksDataInfo->fileFactory;

    QString appDir = qApp->applicationDirPath();

    Stock* stockArray = stocks->data();

    for (int i = start; i < end; ++i)
    {
        Stock& stock = stockArray[i];

        IFile* stockDataFile = fileFactory->newInstance(QString("%1/data/db/stocks/%2.dat").arg(appDir).arg(stock.name));

        if (stockDataFile->open(QIODevice::ReadOnly))
        {
            qint64    fileSize = stockDataFile->size();
            qsizetype dataSize = fileSize / sizeof(StockData);

            stock.data.reserve(dataSize + amountOfDataPerDay);
            stock.data.resizeForOverwrite(dataSize);

            stockDataFile->read(reinterpret_cast<char*>(stock.data.data()), fileSize);
            stockDataFile->close();

            qDebug() << "Read stock data" << stock.name;
        }
        else
        {
            stock.data.clear();

            qWarning() << "Failed to read stock data" << stock.name;
        }

        delete stockDataFile;
    }
}

void StocksDatabase::readStocksData(QList<Stock>* stocks)
{
    qDebug() << "Reading stocks data from database";

    ReadStocksDataInfo readStocksDataInfo;
    readStocksDataInfo.fileFactory = mFileFactory;

    processInParallel(stocks, readStocksDataForParallel, &readStocksDataInfo);
}

void StocksDatabase::writeStocksMeta(QList<Stock>* stocks)
{
    qDebug() << "Writing stocks to database";

    QString stocksStr;

    for (int i = 0; i < stocks->size(); ++i)
    {
        const Stock& stock = stocks->at(i);

        stocksStr += stock.name + "\n";
        stocksStr += stock.fullname + "\n";
    }

    IFile* stocksFile = mFileFactory->newInstance(qApp->applicationDirPath() + "/data/db/stocks/stocks.lst");

    bool ok = stocksFile->open(QIODevice::WriteOnly);
    Q_ASSERT_X(ok, "StocksDatabase::writeStocksMeta()", "Failed to open file");

    stocksFile->write(stocksStr.toUtf8());
    stocksFile->close();

    delete stocksFile;
}

void StocksDatabase::appendStockData(Stock* stock)
{
    QString stockDataFilePath = QString("%1/data/db/stocks/%2.dat").arg(qApp->applicationDirPath()).arg(stock->name);
    IFile*  stockDataFile     = mFileFactory->newInstance(stockDataFilePath);

    bool ok = stockDataFile->open(QIODevice::Append);
    Q_ASSERT_X(ok, "StocksDatabase::appendStockData()", "Failed to open file");

    stockDataFile->write(reinterpret_cast<const char*>(stock->data.constData()), stock->data.size() * sizeof(StockData));
    stockDataFile->close();

    delete stockDataFile;
}

void writeStockData(IFileFactory* fileFactory, const Stock& stock)
{
    QString stockDataFilePath = QString("%1/data/db/stocks/%2.dat").arg(qApp->applicationDirPath()).arg(stock.name);
    IFile*  stockDataFile     = fileFactory->newInstance(stockDataFilePath);

    bool ok = stockDataFile->open(QIODevice::WriteOnly);
    Q_ASSERT_X(ok, "StocksDatabase::writeStockData()", "Failed to open file");

    stockDataFile->write(reinterpret_cast<const char*>(stock.data.constData()), stock.data.size() * sizeof(StockData));
    stockDataFile->close();

    delete stockDataFile;
}

struct DeleteObsoleteDataInfo
{
    IFileFactory* fileFactory;
    qint64        obsoleteTimestamp;
};

void deleteObsoleteDataForParallel(QList<Stock>* stocks, int start, int end, void* additionalArgs)
{
    DeleteObsoleteDataInfo* deleteObsoleteDataInfo = reinterpret_cast<DeleteObsoleteDataInfo*>(additionalArgs);
    IFileFactory*           fileFactory            = deleteObsoleteDataInfo->fileFactory;
    qint64                  obsoleteTimestamp      = deleteObsoleteDataInfo->obsoleteTimestamp;

    Stock* stockArray = stocks->data();

    for (int i = start; i < end; ++i)
    {
        Stock& stock = stockArray[i];
        qint64 index = 0;

        while (index < stock.data.size() && stock.data.at(index).timestamp < obsoleteTimestamp)
        {
            ++index;
        }

        if (index > 0)
        {
            stock.data.remove(0, index);

            writeStockData(fileFactory, stock);
        }
    }
}

void StocksDatabase::deleteObsoleteData(qint64 obsoleteTimestamp, QList<Stock>* stocks)
{
    qDebug() << "Deleting obsolete stocks data";

    DeleteObsoleteDataInfo deleteObsoleteDataInfo;
    deleteObsoleteDataInfo.fileFactory       = mFileFactory;
    deleteObsoleteDataInfo.obsoleteTimestamp = obsoleteTimestamp;

    processInParallel(stocks, deleteObsoleteDataForParallel, &deleteObsoleteDataInfo);
}

void StocksDatabase::fillWithTestData() // TODO: Remove me
{
    QString stocksStr;

    for (int i = 0; i < 100; ++i)
    {
        stocksStr += QString("AZAZ%1\n").arg(i);
        stocksStr += "BLAH\n";
    }

    QString appDir = qApp->applicationDirPath();

    IFile* stocksFile = mFileFactory->newInstance(appDir + "/data/db/stocks/stocks.lst");

    bool ok = stocksFile->open(QIODevice::WriteOnly);
    Q_ASSERT_X(ok, "StocksDatabase::fillWithTestData()", "Failed to open file");

    stocksFile->write(stocksStr.toUtf8());
    stocksFile->close();

    delete stocksFile;



    int              dataSize = 2 * 365 * 24 * 60;
    QList<StockData> stockDataList(dataSize);

    StockData* stockDataArray = stockDataList.data();

    for (int i = 0; i < 100; ++i)
    {
        qInfo() << i;

        for (int j = 0; j < dataSize; ++j)
        {
            StockData* stockData = &stockDataArray[j];

            stockData->timestamp = 1672520400 + j * 60;
            stockData->value     = i * j;
        }

        IFile* stockDataFile = mFileFactory->newInstance(QString("%1/data/db/stocks/AZAZ%2.dat").arg(appDir).arg(i));

        bool ok = stockDataFile->open(QIODevice::WriteOnly);
        Q_ASSERT_X(ok, "StocksDatabase::fillWithTestData()", "Failed to open file");

        stockDataFile->write(reinterpret_cast<const char*>(stockDataArray), dataSize * sizeof(StockData));
        stockDataFile->close();

        delete stockDataFile;
    }
}
