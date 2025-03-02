#include "src/threads/pricecollect/pricecollectthread.h"

#include <QCoreApplication>
#include <QDebug>
#include <QMutexLocker>
#include <QUrlQuery>

#include "src/grpc/utils.h"
#include "src/threads/parallelhelper/parallelhelperthread.h"



#define MAX_GRPC_TIME_LIMIT 2678400000LL // 31 * 24 * 60 * 60 * 1000 // 31 days



PriceCollectThread::PriceCollectThread(
    IConfig*        config,
    IUserStorage*   userStorage,
    IStocksStorage* stocksStorage,
    IDirFactory*    dirFactory,
    IFileFactory*   fileFactory,
    IQZipFactory*   qZipFactory,
    IHttpClient*    httpClient,
    IGrpcClient*    grpcClient,
    QObject*        parent
) :
    IPriceCollectThread(parent),
    mConfig(config),
    mUserStorage(userStorage),
    mStocksStorage(stocksStorage),
    mFileFactory(fileFactory),
    mQZipFactory(qZipFactory),
    mHttpClient(httpClient),
    mGrpcClient(grpcClient)
{
    qDebug() << "Create PriceCollectThread";

    std::shared_ptr<IDir> dir = dirFactory->newInstance();

    bool ok = dir->mkpath(qApp->applicationDirPath() + "/cache/stocks");
    Q_ASSERT_X(ok, "PriceCollectThread::obtainStocksData()", "Failed to create dir");
}

PriceCollectThread::~PriceCollectThread()
{
    qDebug() << "Destroy PriceCollectThread";
}

void PriceCollectThread::run()
{
    qDebug() << "Running PriceCollectThread";

    std::shared_ptr<tinkoff::SharesResponse> tinkoffStocks = mGrpcClient->findStocks(QThread::currentThread());

    if (tinkoffStocks != nullptr && !QThread::currentThread()->isInterruptionRequested())
    {
        storeNewStocksInfo(tinkoffStocks);
        obtainStocksData();
    }

    qDebug() << "Finish PriceCollectThread";
}

struct DownloadLogosInfo
{
    IFileFactory* fileFactory;
    IHttpClient*  httpClient;
};

void
downloadLogosForParallel(QThread* parentThread, QList<const tinkoff::Share*>* stocks, int start, int end, void* additionalArgs)
{
    DownloadLogosInfo* downloadLogosInfo = reinterpret_cast<DownloadLogosInfo*>(additionalArgs);
    IFileFactory*      fileFactory       = downloadLogosInfo->fileFactory;
    IHttpClient*       httpClient        = downloadLogosInfo->httpClient;

    QString appDir = qApp->applicationDirPath();

    const tinkoff::Share** stockArray = stocks->data();

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
    }
}

void PriceCollectThread::storeNewStocksInfo(std::shared_ptr<tinkoff::SharesResponse> tinkoffStocks)
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

    DownloadLogosInfo downloadLogosInfo;
    downloadLogosInfo.fileFactory = mFileFactory;
    downloadLogosInfo.httpClient  = mHttpClient;

    processInParallel(&qtTinkoffStocks, downloadLogosForParallel, &downloadLogosInfo);

    QMutexLocker lock(mStocksStorage->getMutex());
    mStocksStorage->mergeStocksMeta(stocksMeta);
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
                stockData->value     = quotationToFloat(candle.close());

                --lastIndex;
            }
        }

        endTimestamp = dataArray[lastIndex + 1].timestamp;
    }
}

void getCandlesFromZipFile(
    QThread*        parentThread,
    IUserStorage*   userStorage,
    IStocksStorage* stocksStorage,
    IFileFactory*   fileFactory,
    IQZipFactory*   qZipFactory,
    IHttpClient*    httpClient,
    Stock*          stock,
    qint64          startTimestamp,
    qint64          endTimestamp,
    const QString&  zipFilePath
)
{
    std::shared_ptr<IQZip> stockDataFile = qZipFactory->newInstance(zipFilePath);

    if (stockDataFile->open(QuaZip::mdUnzip))
    {
        qInfo() << zipFilePath;

        stockDataFile->close();
    }

    Q_UNUSED(stockDataFile);
    Q_UNUSED(parentThread);
    Q_UNUSED(userStorage);
    Q_UNUSED(stocksStorage);
    Q_UNUSED(fileFactory);
    Q_UNUSED(qZipFactory);
    Q_UNUSED(httpClient);
    Q_UNUSED(stock);
    Q_UNUSED(startTimestamp);
    Q_UNUSED(endTimestamp);
}

void getCandlesWithHttp(
    QThread*        parentThread,
    IUserStorage*   userStorage,
    IStocksStorage* stocksStorage,
    IFileFactory*   fileFactory,
    IQZipFactory*   qZipFactory,
    IHttpClient*    httpClient,
    Stock*          stock,
    qint64          startTimestamp,
    qint64          endTimestamp
)
{
    QString appDir = qApp->applicationDirPath();

    // Round to 1 minute
    startTimestamp = (startTimestamp / 60000) * 60000;
    endTimestamp   = (endTimestamp / 60000 + 1) * 60000;

    QList<StockData> data;
    data.resize((endTimestamp - startTimestamp) / 60000);
    StockData* dataArray = data.data();

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

        getCandlesFromZipFile(
            parentThread,
            userStorage,
            stocksStorage,
            fileFactory,
            qZipFactory,
            httpClient,
            stock,
            startTimestamp,
            endTimestamp,
            zipFilePath
        );
    }

    Q_UNUSED(dataArray);
    Q_UNUSED(parentThread);
    Q_UNUSED(userStorage);
    Q_UNUSED(stocksStorage);
    Q_UNUSED(fileFactory);
    Q_UNUSED(qZipFactory);
    Q_UNUSED(httpClient);
    Q_UNUSED(stock);
    Q_UNUSED(startTimestamp);
    Q_UNUSED(endTimestamp);
}

struct GetCandlesInfo
{
    IConfig*        config;
    IUserStorage*   userStorage;
    IStocksStorage* stocksStorage;
    IFileFactory*   fileFactory;
    IQZipFactory*   qZipFactory;
    IHttpClient*    httpClient;
    IGrpcClient*    grpcClient;
    qint64          currentTimestamp;
};

void getCandlesForParallel(QThread* parentThread, QList<Stock>* stocks, int start, int end, void* additionalArgs)
{
    GetCandlesInfo* getCandlesInfo   = reinterpret_cast<GetCandlesInfo*>(additionalArgs);
    IConfig*        config           = getCandlesInfo->config;
    IUserStorage*   userStorage      = getCandlesInfo->userStorage;
    IStocksStorage* stocksStorage    = getCandlesInfo->stocksStorage;
    IFileFactory*   fileFactory      = getCandlesInfo->fileFactory;
    IQZipFactory*   qZipFactory      = getCandlesInfo->qZipFactory;
    IHttpClient*    httpClient       = getCandlesInfo->httpClient;
    IGrpcClient*    grpcClient       = getCandlesInfo->grpcClient;
    qint64          currentTimestamp = getCandlesInfo->currentTimestamp;

    qint64 storageMonthLimit = qint64(config->getStorageMonthLimit()) * 2678400000LL; // 31 * 24 * 60 * 60 * 1000 // 31 days

    Stock* stockArray = stocks->data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock* stock = &stockArray[i];
        QMutexLocker lock(stock->mutex);

        if (stock->meta.ticker == "SPBE")
        {
            qint64 startTimestamp = stock->operational.lastStoredTimestamp;

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
                    httpClient,
                    stock,
                    startTimestamp,
                    currentTimestamp
                );
            }
        }
    }
}

void PriceCollectThread::obtainStocksData()
{
    GetCandlesInfo getCandlesInfo;
    getCandlesInfo.config           = mConfig;
    getCandlesInfo.userStorage      = mUserStorage;
    getCandlesInfo.stocksStorage    = mStocksStorage;
    getCandlesInfo.fileFactory      = mFileFactory;
    getCandlesInfo.qZipFactory      = mQZipFactory;
    getCandlesInfo.httpClient       = mHttpClient;
    getCandlesInfo.grpcClient       = mGrpcClient;
    getCandlesInfo.currentTimestamp = QDateTime::currentMSecsSinceEpoch();

    QList<Stock>* stocks = mStocksStorage->getStocks();
    processInParallel(stocks, getCandlesForParallel, &getCandlesInfo);
}
