#include "src/threads/pricecollect/pricecollectthread.h"

#include <QAtomicInt>
#include <QCoreApplication>
#include <QDebug>
#include <QMutexLocker>
#include <QUrlQuery>

#include "src/grpc/utils.h"
#include "src/threads/parallelhelper/parallelhelperthread.h"



#define MAX_GRPC_TIME_LIMIT 2678400000LL // 31 * 24 * 60 * 60 * 1000 // 31 days
#define ONE_DAY             86400000LL   // 24 * 60 * 60 * 1000 // 1 day
#define MOSCOW_TIME         10800000LL   // 3 * 60 * 60 * 1000 // 3 hours

#define CSV_FIELD_TIMESTAMP   1
#define CSV_FIELD_CLOSE_PRICE 3



PriceCollectThread::PriceCollectThread(
    IConfig*          config,
    IUserStorage*     userStorage,
    IStocksStorage*   stocksStorage,
    IDirFactory*      dirFactory,
    IFileFactory*     fileFactory,
    IQZipFactory*     qZipFactory,
    IQZipFileFactory* qZipFileFactory,
    IHttpClient*      httpClient,
    IGrpcClient*      grpcClient,
    QObject*          parent
) :
    IPriceCollectThread(parent),
    mConfig(config),
    mUserStorage(userStorage),
    mStocksStorage(stocksStorage),
    mFileFactory(fileFactory),
    mQZipFactory(qZipFactory),
    mQZipFileFactory(qZipFileFactory),
    mHttpClient(httpClient),
    mGrpcClient(grpcClient),
    mDayStartTimestamp()
{
    qDebug() << "Create PriceCollectThread";

    std::shared_ptr<IDir> dir = dirFactory->newInstance();

    bool ok = dir->mkpath(qApp->applicationDirPath() + "/cache/stocks");
    Q_ASSERT_X(ok, "PriceCollectThread::PriceCollectThread()", "Failed to create dir");
}

PriceCollectThread::~PriceCollectThread()
{
    qDebug() << "Destroy PriceCollectThread";
}

void PriceCollectThread::run()
{
    qDebug() << "Running PriceCollectThread";

    emit notifyStocksProgress(tr("Downloading stocks metadata"));

    std::shared_ptr<tinkoff::SharesResponse> tinkoffStocks = mGrpcClient->findStocks(QThread::currentThread());

    if (tinkoffStocks != nullptr && !QThread::currentThread()->isInterruptionRequested())
    {
        bool needStocksUpdate = storeNewStocksInfo(tinkoffStocks);
        obtainStocksData();
        bool needPricesUpdate = obtainStocksDayStartPrice();
        obtainPayback();

        if (needStocksUpdate)
        {
            emit stocksChanged();
        }
        else
        {
            if (needPricesUpdate)
            {
                emit pricesChanged();
            }

            emit paybackChanged();
        }
    }

    qDebug() << "Finish PriceCollectThread";
}

struct DownloadLogosInfo
{
    PriceCollectThread* thread;
    IFileFactory*       fileFactory;
    IHttpClient*        httpClient;
    QAtomicInt          finished;
};

void
downloadLogosForParallel(QThread* parentThread, QList<const tinkoff::Share*>& stocks, int start, int end, void* additionalArgs)
{
    DownloadLogosInfo*  downloadLogosInfo = reinterpret_cast<DownloadLogosInfo*>(additionalArgs);
    PriceCollectThread* thread            = downloadLogosInfo->thread;
    IFileFactory*       fileFactory       = downloadLogosInfo->fileFactory;
    IHttpClient*        httpClient        = downloadLogosInfo->httpClient;

    QString appDir = qApp->applicationDirPath();

    const tinkoff::Share** stockArray = stocks.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        const tinkoff::Share* stock = stockArray[i];

        QString uid = QString::fromStdString(stock->uid());

        std::shared_ptr<IFile> stockLogoFile = fileFactory->newInstance(QString("%1/data/stocks/logos/%2.png").arg(appDir, uid));

        if (!stockLogoFile->exists())
        {
            QString logoName = QString::fromStdString(stock->brand().logo_name()).replace(".png", "x160.png"); // 160 pixels
            QUrl    url      = QUrl(QString("https://invest-brands.cdn-tinkoff.ru/%1").arg(logoName));

            IHttpClient::Headers headers;

            HttpResult httpResult = httpClient->download(url, headers);

            if (httpResult.statusCode == 200)
            {
                stockLogoFile->open(QIODevice::WriteOnly);
                stockLogoFile->write(httpResult.body);
                stockLogoFile->close();
            }
        }

        downloadLogosInfo->finished++;

        emit thread->notifyStocksProgress(
            PriceCollectThread::tr("Downloading stocks logos") +
            QString(" (%1 / %2)").arg(QString::number(downloadLogosInfo->finished), QString::number(stocks.size()))
        );
    }
}

bool PriceCollectThread::storeNewStocksInfo(std::shared_ptr<tinkoff::SharesResponse> tinkoffStocks)
{
    QList<const tinkoff::Share*> qtTinkoffStocks;
    QList<StockMeta>             stocksMeta;

    for (int i = 0; i < tinkoffStocks->instruments_size(); ++i)
    {
        const tinkoff::Share& tinkoffStock = tinkoffStocks->instruments(i);

        if (QString::fromStdString(tinkoffStock.currency()) == "rub" &&
            QString::fromStdString(tinkoffStock.country_of_risk()) == "RU" && tinkoffStock.api_trade_available_flag())
        {
            StockMeta stockMeta;

            stockMeta.uid                     = QString::fromStdString(tinkoffStock.uid());
            stockMeta.ticker                  = QString::fromStdString(tinkoffStock.ticker());
            stockMeta.name                    = QString::fromStdString(tinkoffStock.name());
            stockMeta.forQualInvestorFlag     = tinkoffStock.for_qual_investor_flag();
            stockMeta.lot                     = tinkoffStock.lot();
            stockMeta.minPriceIncrement.units = tinkoffStock.min_price_increment().units();
            stockMeta.minPriceIncrement.nano  = tinkoffStock.min_price_increment().nano();

            qtTinkoffStocks.append(&tinkoffStock);
            stocksMeta.append(stockMeta);
        }
    }

    emit notifyStocksProgress(tr("Downloading stocks logos"));

    DownloadLogosInfo downloadLogosInfo;
    downloadLogosInfo.thread      = this;
    downloadLogosInfo.fileFactory = mFileFactory;
    downloadLogosInfo.httpClient  = mHttpClient;
    downloadLogosInfo.finished    = 0;

    processInParallel(qtTinkoffStocks, downloadLogosForParallel, &downloadLogosInfo);

    QMutexLocker lock(mStocksStorage->getMutex());
    return mStocksStorage->mergeStocksMeta(stocksMeta);
}

void getCandlesWithGrpc(
    QThread*        parentThread,
    IStocksStorage* stocksStorage,
    IGrpcClient*    grpcClient,
    Stock*          stock,
    qint64          startTimestamp,
    qint64          endTimestamp
)
{
    // Round to 1 minute
    startTimestamp = (startTimestamp / 60000) * 60000;
    endTimestamp   = (endTimestamp / 60000 + 1) * 60000;

    QList<StockData> data;
    data.resize((endTimestamp - startTimestamp) / 60000);
    StockData* dataArray = data.data();

    int lastIndex = data.size() - 1;

    while (true)
    {
        std::shared_ptr<tinkoff::GetCandlesResponse> tinkoffCandles =
            grpcClient->getCandles(parentThread, stock->meta.uid, startTimestamp / 1000, endTimestamp / 1000);

        if (parentThread->isInterruptionRequested() || tinkoffCandles == nullptr || tinkoffCandles->candles_size() == 0)
        {
            if (tinkoffCandles->candles_size() == 0)
            {
                stocksStorage->appendStockData(stock, &dataArray[lastIndex + 1], data.size() - lastIndex - 1);
            }

            return;
        }

        for (int i = tinkoffCandles->candles_size() - 1; i >= 0; --i)
        {
            const tinkoff::HistoricCandle& candle = tinkoffCandles->candles(i);

            if (candle.is_complete())
            {
                StockData* stockData = &dataArray[lastIndex];

                stockData->timestamp = candle.time().seconds() * 1000;
                stockData->price     = quotationToFloat(candle.close());

                --lastIndex;
            }
        }

        endTimestamp = dataArray[lastIndex + 1].timestamp;
    }
}

int getCandlesFromZipFile(
    QThread*          parentThread,
    IQZipFactory*     qZipFactory,
    IQZipFileFactory* qZipFileFactory,
    qint64            startTimestamp,
    qint64            endTimestamp,
    const QString&    zipFilePath,
    StockData*        dataArray
)
{
    int indexOffset = 0;

    std::shared_ptr<IQZip> stockDataFile = qZipFactory->newInstance(zipFilePath);

    if (stockDataFile->open(QuaZip::mdUnzip))
    {
        std::shared_ptr<IQZipFile> stockZippedFile = qZipFileFactory->newInstance(stockDataFile->getZip());

        QStringList zippedFiles = stockDataFile->getFileNameList();
        zippedFiles.sort();

        for (int i = 0; i < zippedFiles.size() && !parentThread->isInterruptionRequested(); ++i)
        {
            stockDataFile->setCurrentFile(zippedFiles.at(i));

            stockZippedFile->open(QIODevice::ReadOnly);
            QString content = QString::fromUtf8(stockZippedFile->readAll());
            stockZippedFile->close();

            QStringList csvLines = content.split('\n');

            for (int j = 0; j < csvLines.size(); ++j)
            {
                QStringList csvFields = csvLines.at(j).split(';');

                if (csvFields.size() > CSV_FIELD_CLOSE_PRICE)
                {
                    qint64 timestamp = QDateTime::fromString(csvFields.at(CSV_FIELD_TIMESTAMP), Qt::ISODate).toMSecsSinceEpoch();

                    if (timestamp >= startTimestamp && timestamp < endTimestamp)
                    {
                        StockData* stockData = &dataArray[indexOffset];

                        stockData->timestamp = timestamp;
                        stockData->price     = csvFields.at(CSV_FIELD_CLOSE_PRICE).toFloat();

                        ++indexOffset;
                    }
                }
            }
        }

        stockDataFile->close();
    }

    return indexOffset;
}

void getCandlesWithHttp(
    QThread*          parentThread,
    IUserStorage*     userStorage,
    IStocksStorage*   stocksStorage,
    IFileFactory*     fileFactory,
    IQZipFactory*     qZipFactory,
    IQZipFileFactory* qZipFileFactory,
    IHttpClient*      httpClient,
    Stock*            stock,
    qint64            startTimestamp,
    qint64            endTimestamp
)
{
    QString appDir = qApp->applicationDirPath();

    // Round to 1 minute
    startTimestamp = (startTimestamp / 60000) * 60000;
    endTimestamp   = (endTimestamp / 60000 + 1) * 60000;

    QList<StockData> data;
    data.resize((endTimestamp - startTimestamp) / 60000);
    StockData* dataArray = data.data();

    int indexOffset = 0;

    int startYear = QDateTime::fromMSecsSinceEpoch(startTimestamp).date().year();
    int endYear   = QDateTime::fromMSecsSinceEpoch(endTimestamp).date().year();

    for (int year = startYear; year <= endYear && !parentThread->isInterruptionRequested(); ++year)
    {
        QString zipFilePath = QString("%1/cache/stocks/%2_%3.zip").arg(appDir, stock->meta.uid, QString::number(year));

        std::shared_ptr<IFile> stockDataFile = fileFactory->newInstance(zipFilePath);

        if (year == endYear || !stockDataFile->exists())
        {
            QUrl url = QUrl("https://invest-public-api.tinkoff.ru/history-data");

            QUrlQuery query;

            query.addQueryItem("instrumentId", stock->meta.uid);
            query.addQueryItem("year", QString::number(year));

            url.setQuery(query.query());

            IHttpClient::Headers headers;
            headers["Authorization"] = QString("Bearer %1").arg(userStorage->getToken());

            while (true)
            {
                HttpResult httpResult = httpClient->download(url, headers);

                if (parentThread->isInterruptionRequested() || (httpResult.statusCode != 200 && httpResult.statusCode != 429))
                {
                    break;
                }

                if (httpResult.statusCode == 200)
                {
                    stockDataFile->open(QIODevice::WriteOnly);
                    stockDataFile->write(httpResult.body);
                    stockDataFile->close();

                    break;
                }

                for (int t = 0; t < 10 && !parentThread->isInterruptionRequested(); ++t)
                {
                    QThread::msleep(500);
                }
            }
        }

        indexOffset += getCandlesFromZipFile(
            parentThread, qZipFactory, qZipFileFactory, startTimestamp, endTimestamp, zipFilePath, &dataArray[indexOffset]
        );
    }

    stocksStorage->appendStockData(stock, dataArray, indexOffset);
}

struct GetCandlesInfo
{
    PriceCollectThread* thread;
    IConfig*            config;
    IUserStorage*       userStorage;
    IStocksStorage*     stocksStorage;
    IFileFactory*       fileFactory;
    IQZipFactory*       qZipFactory;
    IQZipFileFactory*   qZipFileFactory;
    IHttpClient*        httpClient;
    IGrpcClient*        grpcClient;
    qint64              currentTimestamp;
    QAtomicInt          finished;
};

void getCandlesForParallel(QThread* parentThread, QList<Stock*>& stocks, int start, int end, void* additionalArgs)
{
    GetCandlesInfo*     getCandlesInfo   = reinterpret_cast<GetCandlesInfo*>(additionalArgs);
    PriceCollectThread* thread           = getCandlesInfo->thread;
    IConfig*            config           = getCandlesInfo->config;
    IUserStorage*       userStorage      = getCandlesInfo->userStorage;
    IStocksStorage*     stocksStorage    = getCandlesInfo->stocksStorage;
    IFileFactory*       fileFactory      = getCandlesInfo->fileFactory;
    IQZipFactory*       qZipFactory      = getCandlesInfo->qZipFactory;
    IQZipFileFactory*   qZipFileFactory  = getCandlesInfo->qZipFileFactory;
    IHttpClient*        httpClient       = getCandlesInfo->httpClient;
    IGrpcClient*        grpcClient       = getCandlesInfo->grpcClient;
    qint64              currentTimestamp = getCandlesInfo->currentTimestamp;

    qint64 storageMonthLimit = qint64(config->getStorageMonthLimit()) * 2678400000LL; // 31 * 24 * 60 * 60 * 1000 // 31 days

    Stock** stockArray = stocks.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock*       stock = stockArray[i];
        QMutexLocker lock(stock->mutex);

        qint64 startTimestamp = stock->operational.lastStoredTimestamp + 60000;

        if (startTimestamp < currentTimestamp - storageMonthLimit)
        {
            startTimestamp = currentTimestamp - storageMonthLimit;
        }

        if (currentTimestamp - startTimestamp < MAX_GRPC_TIME_LIMIT)
        {
            getCandlesWithGrpc(parentThread, stocksStorage, grpcClient, stock, startTimestamp, currentTimestamp);
        }
        else
        {
            getCandlesWithHttp(
                parentThread,
                userStorage,
                stocksStorage,
                fileFactory,
                qZipFactory,
                qZipFileFactory,
                httpClient,
                stock,
                startTimestamp,
                currentTimestamp
            );
            getCandlesWithGrpc(
                parentThread, stocksStorage, grpcClient, stock, stock->operational.lastStoredTimestamp + 60000, currentTimestamp
            );
        }

        getCandlesInfo->finished++;

        emit thread->notifyStocksProgress(
            PriceCollectThread::tr("Obtain stocks data") +
            QString(" (%1 / %2)").arg(QString::number(getCandlesInfo->finished), QString::number(stocks.size()))
        );
    }
}

void PriceCollectThread::obtainStocksData()
{
    emit notifyStocksProgress(tr("Obtain stocks data"));

    GetCandlesInfo getCandlesInfo;
    getCandlesInfo.thread           = this;
    getCandlesInfo.config           = mConfig;
    getCandlesInfo.userStorage      = mUserStorage;
    getCandlesInfo.stocksStorage    = mStocksStorage;
    getCandlesInfo.fileFactory      = mFileFactory;
    getCandlesInfo.qZipFactory      = mQZipFactory;
    getCandlesInfo.qZipFileFactory  = mQZipFileFactory;
    getCandlesInfo.httpClient       = mHttpClient;
    getCandlesInfo.grpcClient       = mGrpcClient;
    getCandlesInfo.currentTimestamp = QDateTime::currentMSecsSinceEpoch();
    getCandlesInfo.finished         = 0;

    QList<Stock*>& stocks = mStocksStorage->getStocks();
    processInParallel(stocks, getCandlesForParallel, &getCandlesInfo);
}

bool PriceCollectThread::obtainStocksDayStartPrice()
{
    // Round to 1 day
    qint64 newDayStartTimestamp = (QDateTime::currentMSecsSinceEpoch() / ONE_DAY) * ONE_DAY - MOSCOW_TIME;

    if (mDayStartTimestamp != newDayStartTimestamp)
    {
        mDayStartTimestamp = newDayStartTimestamp;

        mStocksStorage->obtainStocksDayStartPrice(mDayStartTimestamp);

        return true;
    }

    return false;
}

void PriceCollectThread::obtainPayback()
{
    mStocksStorage->obtainPayback(QDateTime::currentMSecsSinceEpoch() - ONE_DAY);
}
