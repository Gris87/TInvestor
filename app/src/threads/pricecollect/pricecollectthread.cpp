#include "src/threads/pricecollect/pricecollectthread.h"

#include <QAtomicInt>
#include <QCoreApplication>
#include <QDebug>
#include <QMutexLocker>
#include <QUrlQuery>

#include "src/grpc/utils.h"
#include "src/threads/parallelhelper/parallelhelperthread.h"



constexpr qint64 MS_IN_SECOND                       = 1000LL;
constexpr qint64 ONE_MINUTE                         = 60LL * MS_IN_SECOND;
constexpr qint64 ONE_HOUR                           = 60LL * ONE_MINUTE;
constexpr qint64 ONE_DAY                            = 24LL * ONE_HOUR;
constexpr qint64 ONE_MONTH                          = 31LL * ONE_DAY;
constexpr qint64 SLEEP_DELAY                        = 5LL * MS_IN_SECOND; // 5 seconds
constexpr qint64 MOSCOW_TIME                        = 3 * ONE_HOUR;       // 3 hours
constexpr qint64 MAX_GRPC_TIME_LIMIT                = ONE_MONTH;          // 1 month
constexpr int    HTTP_STATUS_CODE_OK                = 200;
constexpr int    HTTP_STATUS_CODE_TOO_MANY_REQUESTS = 429;



enum CsvField : qint8
{
    CSV_FIELD_FIGI,
    CSV_FIELD_TIMESTAMP,
    CSV_FIELD_OPEN_PRICE,
    CSV_FIELD_CLOSE_PRICE,
    CSV_FIELD_HIGH_PRICE,
    CSV_FIELD_LOW_PRICE,
    CSV_FIELD_VOLUME,
};



PriceCollectThread::PriceCollectThread(
    IConfig*          config,
    IUserStorage*     userStorage,
    IStocksStorage*   stocksStorage,
    IDirFactory*      dirFactory,
    IFileFactory*     fileFactory,
    IQZipFactory*     qZipFactory,
    IQZipFileFactory* qZipFileFactory,
    ITimeUtils*       timeUtils,
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
    mTimeUtils(timeUtils),
    mHttpClient(httpClient),
    mGrpcClient(grpcClient),
    mDayStartTimestamp()
{
    qDebug() << "Create PriceCollectThread";

    const std::shared_ptr<IDir> dir = dirFactory->newInstance();

    const bool ok = dir->mkpath(qApp->applicationDirPath() + "/cache/stocks");
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

    const std::shared_ptr<tinkoff::SharesResponse> tinkoffStocks = mGrpcClient->findStocks(QThread::currentThread());

    if (tinkoffStocks != nullptr && !QThread::currentThread()->isInterruptionRequested())
    {
        const bool needStocksUpdate = storeNewStocksInfo(tinkoffStocks);
        obtainStocksData();
        cleanupOperationalData();
        const bool needPricesUpdate = obtainStocksDayStartPrice();
        obtainTurnover();
        obtainPayback();

        notifyAboutChanges(needStocksUpdate, needPricesUpdate);
    }

    qDebug() << "Finish PriceCollectThread";
}

void PriceCollectThread::downloadLogo(const QUrl& url, const std::shared_ptr<IFile>& stockLogoFile)
{
    const IHttpClient::Headers headers;

    const HttpResult httpResult = mHttpClient->download(url, headers);

    if (httpResult.statusCode == HTTP_STATUS_CODE_OK && httpResult.body.size() > 0)
    {
        const bool ok = stockLogoFile->open(QIODevice::WriteOnly);
        Q_ASSERT_X(ok, "PriceCollectThread::downloadLogo()", "Failed to open file");

        stockLogoFile->write(httpResult.body);
        stockLogoFile->close();
    }
    else
    {
        const std::shared_ptr<IFile> noImageFile = mFileFactory->newInstance(":/assets/images/no_image.png");
        noImageFile->open(QIODevice::ReadOnly);

        const bool ok = stockLogoFile->open(QIODevice::WriteOnly);
        Q_ASSERT_X(ok, "PriceCollectThread::downloadLogo()", "Failed to open file");

        stockLogoFile->write(noImageFile->readAll());
        stockLogoFile->close();

        noImageFile->close();
    }
}

struct DownloadLogosInfo
{
    explicit DownloadLogosInfo(PriceCollectThread* _thread, IFileFactory* _fileFactory, bool _forceToDownload) :
        thread(_thread),
        fileFactory(_fileFactory),
        forceToDownload(_forceToDownload),
        finished()
    {
    }

    PriceCollectThread* thread;
    IFileFactory*       fileFactory;
    bool                forceToDownload;
    QAtomicInt          finished;
};

static void
downloadLogosForParallel(QThread* parentThread, QList<const tinkoff::Share*>& stocks, int start, int end, void* additionalArgs)
{
    DownloadLogosInfo*  downloadLogosInfo = reinterpret_cast<DownloadLogosInfo*>(additionalArgs);
    PriceCollectThread* thread            = downloadLogosInfo->thread;
    IFileFactory*       fileFactory       = downloadLogosInfo->fileFactory;
    const bool          forceToDownload   = downloadLogosInfo->forceToDownload;

    const QString appDir = qApp->applicationDirPath();

    const tinkoff::Share** stockArray = stocks.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        const tinkoff::Share* stock = stockArray[i];

        const QString uid = QString::fromStdString(stock->uid());

        const std::shared_ptr<IFile> stockLogoFile =
            fileFactory->newInstance(QString("%1/data/stocks/logos/%2.png").arg(appDir, uid));

        if (forceToDownload || !stockLogoFile->exists())
        {
            const QString logoName = QString::fromStdString(stock->brand().logo_name()).replace(".png", "x160.png"); // 160 pixels
            const QUrl    url      = QUrl(QString("https://invest-brands.cdn-tinkoff.ru/%1").arg(logoName));

            thread->downloadLogo(url, stockLogoFile);
        }

        downloadLogosInfo->finished++;

        emit thread->notifyStocksProgress(
            PriceCollectThread::tr("Downloading stocks logos") +
            QString(" (%1 / %2)").arg(QString::number(downloadLogosInfo->finished), QString::number(stocks.size()))
        );
    }
}

bool PriceCollectThread::storeNewStocksInfo(const std::shared_ptr<tinkoff::SharesResponse>& tinkoffStocks)
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

    static int lastDownloadHour = -1;
    const int  currentHour      = QDateTime::currentDateTime().time().hour();

    if (lastDownloadHour < 0)
    {
        lastDownloadHour = currentHour;
    }

    DownloadLogosInfo downloadLogosInfo(this, mFileFactory, lastDownloadHour == currentHour);
    processInParallel(qtTinkoffStocks, downloadLogosForParallel, &downloadLogosInfo);

    const QMutexLocker lock(mStocksStorage->getMutex());
    return mStocksStorage->mergeStocksMeta(stocksMeta);
}

static void getCandlesWithGrpc(
    QThread*        parentThread,
    IStocksStorage* stocksStorage,
    IGrpcClient*    grpcClient,
    Stock*          stock,
    qint64          startTimestamp,
    qint64          endTimestamp
)
{
    // Round to 1 minute
    startTimestamp = (startTimestamp / ONE_MINUTE) * ONE_MINUTE;
    endTimestamp   = (endTimestamp / ONE_MINUTE + 1) * ONE_MINUTE;

    QList<StockData> data;
    data.resize((endTimestamp - startTimestamp) / ONE_MINUTE);
    StockData* dataArray = data.data();

    int lastIndex = data.size() - 1;

    while (true)
    {
        const std::shared_ptr<tinkoff::GetCandlesResponse> tinkoffCandles =
            grpcClient->getCandles(parentThread, stock->meta.uid, startTimestamp, endTimestamp);

        if (parentThread->isInterruptionRequested() || tinkoffCandles == nullptr || tinkoffCandles->candles_size() == 0)
        {
            if (tinkoffCandles != nullptr && tinkoffCandles->candles_size() == 0)
            {
                stocksStorage->appendStockData(stock, &dataArray[lastIndex + 1], data.size() - lastIndex - 1);
            }

            break;
        }

        for (int i = tinkoffCandles->candles_size() - 1; i >= 0; --i)
        {
            const tinkoff::HistoricCandle& candle = tinkoffCandles->candles(i);

            if (candle.is_complete())
            {
                StockData* stockData = &dataArray[lastIndex];

                stockData->timestamp = timeToTimestamp(candle.time());
                stockData->quantity  = candle.volume();
                stockData->price     = quotationToFloat(candle.close());

                --lastIndex;
            }
        }

        endTimestamp = dataArray[lastIndex + 1].timestamp;
    }
}

static int getCandlesFromZipFile(
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

    const std::shared_ptr<IQZip> stockDataFile = qZipFactory->newInstance(zipFilePath);

    if (stockDataFile->open(QuaZip::mdUnzip))
    {
        const std::shared_ptr<IQZipFile> stockZippedFile = qZipFileFactory->newInstance(stockDataFile->getZip());

        QStringList zippedFiles = stockDataFile->getFileNameList();
        zippedFiles.sort();

        for (int i = 0; i < zippedFiles.size() && !parentThread->isInterruptionRequested(); ++i)
        {
            stockDataFile->setCurrentFile(zippedFiles.at(i));

            stockZippedFile->open(QIODevice::ReadOnly);
            const QString content = QString::fromUtf8(stockZippedFile->readAll());
            stockZippedFile->close();

            const QStringList csvLines = content.split('\n');

            for (const QString& csvLine : csvLines)
            {
                const QStringList csvFields = csvLine.split(';');

                if (csvFields.size() > CSV_FIELD_VOLUME)
                {
                    const qint64 timestamp =
                        QDateTime::fromString(csvFields.at(CSV_FIELD_TIMESTAMP), Qt::ISODate).toMSecsSinceEpoch();

                    if (timestamp >= startTimestamp && timestamp < endTimestamp)
                    {
                        StockData* stockData = &dataArray[indexOffset];

                        stockData->timestamp = timestamp;
                        stockData->quantity  = csvFields.at(CSV_FIELD_VOLUME).toLongLong();
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

static void getCandlesWithHttp(
    QThread*          parentThread,
    IUserStorage*     userStorage,
    IStocksStorage*   stocksStorage,
    IFileFactory*     fileFactory,
    IQZipFactory*     qZipFactory,
    IQZipFileFactory* qZipFileFactory,
    ITimeUtils*       timeUtils,
    IHttpClient*      httpClient,
    Stock*            stock,
    qint64            startTimestamp,
    qint64            endTimestamp
)
{
    const QString appDir = qApp->applicationDirPath();

    // Round to 1 minute
    startTimestamp = (startTimestamp / ONE_MINUTE) * ONE_MINUTE;
    endTimestamp   = (endTimestamp / ONE_MINUTE + 1) * ONE_MINUTE;

    QList<StockData> data;
    data.resize((endTimestamp - startTimestamp) / ONE_MINUTE);
    StockData* dataArray = data.data();

    int indexOffset = 0;

    const int startYear = QDateTime::fromMSecsSinceEpoch(startTimestamp).date().year();
    const int endYear   = QDateTime::fromMSecsSinceEpoch(endTimestamp).date().year();

    for (int year = startYear; year <= endYear && !parentThread->isInterruptionRequested(); ++year)
    {
        const QString zipFilePath = QString("%1/cache/stocks/%2_%3.zip").arg(appDir, stock->meta.uid, QString::number(year));

        const std::shared_ptr<IFile> stockDataFile = fileFactory->newInstance(zipFilePath);

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
                const HttpResult httpResult = httpClient->download(url, headers);

                if (parentThread->isInterruptionRequested() ||
                    (httpResult.statusCode != HTTP_STATUS_CODE_OK && httpResult.statusCode != HTTP_STATUS_CODE_TOO_MANY_REQUESTS))
                {
                    break;
                }

                if (httpResult.statusCode == HTTP_STATUS_CODE_OK)
                {
                    const bool ok = stockDataFile->open(QIODevice::WriteOnly);
                    Q_ASSERT_X(ok, "PriceCollectThread::getCandlesWithHttp()", "Failed to open file");

                    stockDataFile->write(httpResult.body);
                    stockDataFile->close();

                    break;
                }

                if (timeUtils->interruptibleSleep(SLEEP_DELAY, parentThread))
                {
                    break;
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
    explicit GetCandlesInfo(
        PriceCollectThread* _thread,
        IConfig*            _config,
        IUserStorage*       _userStorage,
        IStocksStorage*     _stocksStorage,
        IFileFactory*       _fileFactory,
        IQZipFactory*       _qZipFactory,
        IQZipFileFactory*   _qZipFileFactory,
        ITimeUtils*         _timeUtils,
        IHttpClient*        _httpClient,
        IGrpcClient*        _grpcClient,
        qint64              _currentTimestamp
    ) :
        thread(_thread),
        config(_config),
        userStorage(_userStorage),
        stocksStorage(_stocksStorage),
        fileFactory(_fileFactory),
        qZipFactory(_qZipFactory),
        qZipFileFactory(_qZipFileFactory),
        timeUtils(_timeUtils),
        httpClient(_httpClient),
        grpcClient(_grpcClient),
        currentTimestamp(_currentTimestamp),
        finished()
    {
    }

    PriceCollectThread* thread;
    IConfig*            config;
    IUserStorage*       userStorage;
    IStocksStorage*     stocksStorage;
    IFileFactory*       fileFactory;
    IQZipFactory*       qZipFactory;
    IQZipFileFactory*   qZipFileFactory;
    ITimeUtils*         timeUtils;
    IHttpClient*        httpClient;
    IGrpcClient*        grpcClient;
    qint64              currentTimestamp;
    QAtomicInt          finished;
};

static void getCandlesForParallel(QThread* parentThread, QList<Stock*>& stocks, int start, int end, void* additionalArgs)
{
    GetCandlesInfo*     getCandlesInfo   = reinterpret_cast<GetCandlesInfo*>(additionalArgs);
    PriceCollectThread* thread           = getCandlesInfo->thread;
    IConfig*            config           = getCandlesInfo->config;
    IUserStorage*       userStorage      = getCandlesInfo->userStorage;
    IStocksStorage*     stocksStorage    = getCandlesInfo->stocksStorage;
    IFileFactory*       fileFactory      = getCandlesInfo->fileFactory;
    IQZipFactory*       qZipFactory      = getCandlesInfo->qZipFactory;
    IQZipFileFactory*   qZipFileFactory  = getCandlesInfo->qZipFileFactory;
    ITimeUtils*         timeUtils        = getCandlesInfo->timeUtils;
    IHttpClient*        httpClient       = getCandlesInfo->httpClient;
    IGrpcClient*        grpcClient       = getCandlesInfo->grpcClient;
    const qint64        currentTimestamp = getCandlesInfo->currentTimestamp;

    const qint64 storageMonthLimit = static_cast<qint64>(config->getStorageMonthLimit()) * ONE_MONTH;

    Stock** stockArray = stocks.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock*             stock = stockArray[i];
        const QMutexLocker lock(stock->mutex);

        qint64 startTimestamp =
            qBound(currentTimestamp - storageMonthLimit, stock->operational.lastStoredTimestamp + ONE_MINUTE, currentTimestamp);

        if (currentTimestamp - startTimestamp >= MAX_GRPC_TIME_LIMIT)
        {
            getCandlesWithHttp(
                parentThread,
                userStorage,
                stocksStorage,
                fileFactory,
                qZipFactory,
                qZipFileFactory,
                timeUtils,
                httpClient,
                stock,
                startTimestamp,
                currentTimestamp
            );

            startTimestamp = stock->operational.lastStoredTimestamp + ONE_MINUTE;
        }

        getCandlesWithGrpc(parentThread, stocksStorage, grpcClient, stock, startTimestamp, currentTimestamp);

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

    GetCandlesInfo getCandlesInfo(
        this,
        mConfig,
        mUserStorage,
        mStocksStorage,
        mFileFactory,
        mQZipFactory,
        mQZipFileFactory,
        mTimeUtils,
        mHttpClient,
        mGrpcClient,
        QDateTime::currentMSecsSinceEpoch()
    );

    QList<Stock*>& stocks = mStocksStorage->getStocks();
    processInParallel(stocks, getCandlesForParallel, &getCandlesInfo);
}

void PriceCollectThread::cleanupOperationalData()
{
    mStocksStorage->cleanupOperationalData(QDateTime::currentMSecsSinceEpoch() - ONE_HOUR);
}

bool PriceCollectThread::obtainStocksDayStartPrice()
{
    // Round to 1 day
    const qint64 newDayStartTimestamp = ((QDateTime::currentMSecsSinceEpoch() / ONE_DAY) * ONE_DAY) - MOSCOW_TIME;

    if (mDayStartTimestamp != newDayStartTimestamp)
    {
        mDayStartTimestamp = newDayStartTimestamp;

        mStocksStorage->obtainStocksDayStartPrice(mDayStartTimestamp);

        return true;
    }

    return false;
}

void PriceCollectThread::obtainTurnover()
{
    mStocksStorage->obtainTurnover(QDateTime::currentMSecsSinceEpoch() - ONE_MONTH);
}

void PriceCollectThread::obtainPayback()
{
    mStocksStorage->obtainPayback(QDateTime::currentMSecsSinceEpoch() - ONE_DAY);
}

void PriceCollectThread::notifyAboutChanges(bool needStocksUpdate, bool needPricesUpdate)
{
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

        emit periodicDataChanged();
    }
}
