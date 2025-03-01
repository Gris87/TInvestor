#include "src/threads/pricecollect/pricecollectthread.h"

#include <QCoreApplication>
#include <QDebug>
#include <QMutexLocker>

#include "src/grpc/utils.h"
#include "src/threads/parallelhelper/parallelhelperthread.h"



#define MAX_GRPC_TIME_LIMIT 2678400000LL // 31 * 24 * 60 * 60 * 1000 // 31 days



PriceCollectThread::PriceCollectThread(
    IConfig*        config,
    IUserStorage*   userStorage,
    IStocksStorage* stocksStorage,
    IFileFactory*   fileFactory,
    IHttpClient*    httpClient,
    IGrpcClient*    grpcClient,
    QObject*        parent
) :
    IPriceCollectThread(parent),
    mConfig(config),
    mUserStorage(userStorage),
    mStocksStorage(stocksStorage),
    mFileFactory(fileFactory),
    mHttpClient(httpClient),
    mGrpcClient(grpcClient)
{
    qDebug() << "Create PriceCollectThread";
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
            QString url      = QString("https://invest-brands.cdn-tinkoff.ru/%1").arg(logoName);

            IHttpClient::Headers headers;

            std::shared_ptr<QByteArray> data = httpClient->download(url, headers);

            if (data)
            {
                stockLogoFile->open(QIODevice::WriteOnly);
                stockLogoFile->write(*data);
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

void getCandlesWithHttp(
    QThread*        parentThread,
    IUserStorage*   userStorage,
    IStocksStorage* stocksStorage,
    IHttpClient*    httpClient,
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

    int startYear = QDateTime::fromMSecsSinceEpoch(startTimestamp).date().year();
    int endYear   = QDateTime::fromMSecsSinceEpoch(endTimestamp).date().year();

    for (int year = startYear; year <= endYear; ++year)
    {
        qInfo() << year;
    }

    Q_UNUSED(dataArray);
    Q_UNUSED(parentThread);
    Q_UNUSED(userStorage);
    Q_UNUSED(stocksStorage);
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
                getCandlesWithHttp(parentThread, userStorage, stocksStorage, httpClient, stock, startTimestamp, currentTimestamp);
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
    getCandlesInfo.httpClient       = mHttpClient;
    getCandlesInfo.grpcClient       = mGrpcClient;
    getCandlesInfo.currentTimestamp = QDateTime::currentMSecsSinceEpoch();

    QList<Stock>* stocks = mStocksStorage->getStocks();
    processInParallel(stocks, getCandlesForParallel, &getCandlesInfo);
}
