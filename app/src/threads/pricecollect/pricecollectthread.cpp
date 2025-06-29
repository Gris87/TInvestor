#include "src/threads/pricecollect/pricecollectthread.h"

#include <QAtomicInt>
#include <QCoreApplication>
#include <QDebug>
#include <QUrlQuery>

#include "src/grpc/utils.h"
#include "src/threads/parallelhelper/parallelhelperthread.h"



const char* const RUBLE_UID                          = "a92e2e25-a698-45cc-a781-167cf465257c";
constexpr qint64  MS_IN_SECOND                       = 1000LL;
constexpr qint64  ONE_MINUTE                         = 60LL * MS_IN_SECOND;
constexpr qint64  ONE_HOUR                           = 60LL * ONE_MINUTE;
constexpr qint64  ONE_DAY                            = 24LL * ONE_HOUR;
constexpr qint64  ONE_MONTH                          = 31LL * ONE_DAY;
constexpr qint64  SLEEP_DELAY                        = 5LL * MS_IN_SECOND; // 5 seconds
constexpr qint64  MOSCOW_TIME                        = 3 * ONE_HOUR;       // 3 hours
constexpr qint64  MAX_GRPC_TIME_LIMIT                = ONE_MONTH;          // 1 month
constexpr int     LOGO_SIZE                          = 24;
constexpr int     HTTP_STATUS_CODE_OK                = 200;
constexpr int     HTTP_STATUS_CODE_TOO_MANY_REQUESTS = 429;



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
    IConfig*             config,
    IUserStorage*        userStorage,
    IStocksStorage*      stocksStorage,
    IInstrumentsStorage* instrumentsStorage,
    ILogosStorage*       logosStorage,
    IDirFactory*         dirFactory,
    IFileFactory*        fileFactory,
    IQZipFactory*        qZipFactory,
    IQZipFileFactory*    qZipFileFactory,
    ITimeUtils*          timeUtils,
    IHttpClient*         httpClient,
    IGrpcClient*         grpcClient,
    QObject*             parent
) :
    IPriceCollectThread(parent),
    mConfig(config),
    mUserStorage(userStorage),
    mStocksStorage(stocksStorage),
    mInstrumentsStorage(instrumentsStorage),
    mLogosStorage(logosStorage),
    mDirFactory(dirFactory),
    mFileFactory(fileFactory),
    mQZipFactory(qZipFactory),
    mQZipFileFactory(qZipFileFactory),
    mTimeUtils(timeUtils),
    mHttpClient(httpClient),
    mGrpcClient(grpcClient),
    mDayStartTimestamp()
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

    blockSignals(false);
    emit notifyInstrumentsProgress(tr("Downloading metadata"));

    const std::shared_ptr<tinkoff::SharesResponse> tinkoffStocks =
        mGrpcClient->findStocks(QThread::currentThread(), tinkoff::INSTRUMENT_STATUS_BASE);

    if (!QThread::currentThread()->isInterruptionRequested() && tinkoffStocks != nullptr)
    {
        storeNewInstrumentsInfo();
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

void PriceCollectThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}

bool PriceCollectThread::storeNewStocksInfo(const std::shared_ptr<tinkoff::SharesResponse>& tinkoffStocks)
{
    QList<StockMeta> stocksMeta;

    stocksMeta.reserve(tinkoffStocks->instruments_size());

    mLogosStorage->readLock();

    for (int i = 0; i < tinkoffStocks->instruments_size(); ++i)
    {
        const tinkoff::Share& tinkoffStock = tinkoffStocks->instruments(i);

        if (tinkoffStock.currency() == "rub" && tinkoffStock.api_trade_available_flag())
        {
            StockMeta stockMeta;

            stockMeta.instrumentId        = QString::fromStdString(tinkoffStock.uid());
            stockMeta.instrumentLogo      = mLogosStorage->getLogo(stockMeta.instrumentId);
            stockMeta.instrumentTicker    = QString::fromStdString(tinkoffStock.ticker());
            stockMeta.instrumentName      = QString::fromStdString(tinkoffStock.name());
            stockMeta.forQualInvestorFlag = tinkoffStock.for_qual_investor_flag();
            stockMeta.minPriceIncrement   = quotationToFloat(tinkoffStock.min_price_increment());
            stockMeta.pricePrecision      = quotationPrecision(tinkoffStock.min_price_increment());

            stocksMeta.append(stockMeta);
        }
    }

    mLogosStorage->readUnlock();

    mStocksStorage->writeLock();
    const bool res = mStocksStorage->mergeStocksMeta(stocksMeta);
    mStocksStorage->writeUnlock();

    return res;
}

static void
obtainInstrumentsFromShares(QThread* parentThread, IGrpcClient* grpcClient, Instruments& res, QList<InstrumentIdAndLogo>& logos)
{
    const std::shared_ptr<tinkoff::SharesResponse> tinkoffShares =
        grpcClient->findStocks(parentThread, tinkoff::INSTRUMENT_STATUS_ALL);

    if (!parentThread->isInterruptionRequested() && tinkoffShares != nullptr)
    {
        for (int i = 0; i < tinkoffShares->instruments_size(); ++i)
        {
            const tinkoff::Share& tinkoffShare = tinkoffShares->instruments(i);

            if (tinkoffShare.currency() == "rub")
            {
                const QString instrumentId = QString::fromStdString(tinkoffShare.uid());

                Instrument instrument;
                instrument.ticker         = QString::fromStdString(tinkoffShare.ticker());
                instrument.name           = QString::fromStdString(tinkoffShare.name());
                instrument.lot            = tinkoffShare.lot();
                instrument.pricePrecision = quotationPrecision(tinkoffShare.min_price_increment());

                res[instrumentId] = instrument;
                logos.append(InstrumentIdAndLogo(instrumentId, QString::fromStdString(tinkoffShare.brand().logo_name())));
            }
        }
    }
}

static void
obtainInstrumentsFromBonds(QThread* parentThread, IGrpcClient* grpcClient, Instruments& res, QList<InstrumentIdAndLogo>& logos)
{
    const std::shared_ptr<tinkoff::BondsResponse> tinkoffBonds = grpcClient->findBonds(parentThread);

    if (!parentThread->isInterruptionRequested() && tinkoffBonds != nullptr)
    {
        for (int i = 0; i < tinkoffBonds->instruments_size(); ++i)
        {
            const tinkoff::Bond& tinkoffBond = tinkoffBonds->instruments(i);

            if (tinkoffBond.currency() == "rub")
            {
                const QString instrumentId = QString::fromStdString(tinkoffBond.uid());

                Instrument instrument;
                instrument.ticker         = QString::fromStdString(tinkoffBond.ticker());
                instrument.name           = QString::fromStdString(tinkoffBond.name());
                instrument.lot            = tinkoffBond.lot();
                instrument.pricePrecision = quotationPrecision(tinkoffBond.min_price_increment());

                res[instrumentId] = instrument;
                logos.append(InstrumentIdAndLogo(instrumentId, QString::fromStdString(tinkoffBond.brand().logo_name())));
            }
        }
    }
}

static void obtainInstrumentsFromCurrencies(
    QThread* parentThread, IGrpcClient* grpcClient, Instruments& res, QList<InstrumentIdAndLogo>& logos
)
{
    const std::shared_ptr<tinkoff::CurrenciesResponse> tinkoffCurrencies = grpcClient->findCurrencies(parentThread);

    if (!parentThread->isInterruptionRequested() && tinkoffCurrencies != nullptr)
    {
        for (int i = 0; i < tinkoffCurrencies->instruments_size(); ++i)
        {
            const tinkoff::Currency& tinkoffCurrency = tinkoffCurrencies->instruments(i);

            if (tinkoffCurrency.currency() == "rub")
            {
                const QString instrumentId = QString::fromStdString(tinkoffCurrency.uid());

                Instrument instrument;
                instrument.ticker         = QString::fromStdString(tinkoffCurrency.ticker());
                instrument.name           = QString::fromStdString(tinkoffCurrency.name());
                instrument.lot            = tinkoffCurrency.lot();
                instrument.pricePrecision = quotationPrecision(tinkoffCurrency.min_price_increment());

                res[instrumentId] = instrument;
                logos.append(InstrumentIdAndLogo(instrumentId, QString::fromStdString(tinkoffCurrency.brand().logo_name())));
            }
        }
    }

    // Use ruble name instead of ruble ticker
    Instrument& rubleInstrument = res[RUBLE_UID];
    rubleInstrument.ticker      = rubleInstrument.name;
}

static void
obtainInstrumentsFromEtfs(QThread* parentThread, IGrpcClient* grpcClient, Instruments& res, QList<InstrumentIdAndLogo>& logos)
{
    const std::shared_ptr<tinkoff::EtfsResponse> tinkoffEtfs = grpcClient->findEtfs(parentThread);

    if (!parentThread->isInterruptionRequested() && tinkoffEtfs != nullptr)
    {
        for (int i = 0; i < tinkoffEtfs->instruments_size(); ++i)
        {
            const tinkoff::Etf& tinkoffEtf = tinkoffEtfs->instruments(i);

            if (tinkoffEtf.currency() == "rub")
            {
                const QString instrumentId = QString::fromStdString(tinkoffEtf.uid());

                Instrument instrument;
                instrument.ticker         = QString::fromStdString(tinkoffEtf.ticker());
                instrument.name           = QString::fromStdString(tinkoffEtf.name());
                instrument.lot            = tinkoffEtf.lot();
                instrument.pricePrecision = quotationPrecision(tinkoffEtf.min_price_increment());

                res[instrumentId] = instrument;
                logos.append(InstrumentIdAndLogo(instrumentId, QString::fromStdString(tinkoffEtf.brand().logo_name())));
            }
        }
    }
}

static void
obtainInstrumentsFromFutures(QThread* parentThread, IGrpcClient* grpcClient, Instruments& res, QList<InstrumentIdAndLogo>& logos)
{
    const std::shared_ptr<tinkoff::FuturesResponse> tinkoffFutures = grpcClient->findFutures(parentThread);

    if (!parentThread->isInterruptionRequested() && tinkoffFutures != nullptr)
    {
        for (int i = 0; i < tinkoffFutures->instruments_size(); ++i)
        {
            const tinkoff::Future& tinkoffFuture = tinkoffFutures->instruments(i);

            if (tinkoffFuture.currency() == "rub")
            {
                const QString instrumentId = QString::fromStdString(tinkoffFuture.uid());

                Instrument instrument;
                instrument.ticker         = QString::fromStdString(tinkoffFuture.ticker());
                instrument.name           = QString::fromStdString(tinkoffFuture.name());
                instrument.lot            = tinkoffFuture.lot();
                instrument.pricePrecision = quotationPrecision(tinkoffFuture.min_price_increment());

                res[instrumentId] = instrument;
                logos.append(InstrumentIdAndLogo(instrumentId, QString::fromStdString(tinkoffFuture.brand().logo_name())));
            }
        }
    }
}

using InstrumentHandler =
    void (*)(QThread* parentThread, IGrpcClient* grpcClient, Instruments& res, QList<InstrumentIdAndLogo>& logos);

// clang-format off
static const QMap<tinkoff::InstrumentType, InstrumentHandler> INSTRUMENT_TYPE_TO_HANDLER{ // clazy:exclude=non-pod-global-static
    {tinkoff::INSTRUMENT_TYPE_SHARE,    obtainInstrumentsFromShares},
    {tinkoff::INSTRUMENT_TYPE_BOND,     obtainInstrumentsFromBonds},
    {tinkoff::INSTRUMENT_TYPE_CURRENCY, obtainInstrumentsFromCurrencies},
    {tinkoff::INSTRUMENT_TYPE_ETF,      obtainInstrumentsFromEtfs},
    {tinkoff::INSTRUMENT_TYPE_FUTURES,  obtainInstrumentsFromFutures}
};
// clang-format on

struct ObtainInstrumentsInfo
{
    explicit ObtainInstrumentsInfo(IGrpcClient* _grpcClient, const QList<tinkoff::InstrumentType>& instrumentTypes) :
        grpcClient(_grpcClient)
    {
        results.resize(instrumentTypes.size());
        logos.resize(instrumentTypes.size());
    }

    IGrpcClient*                      grpcClient;
    QList<Instruments>                results; // UID => Instrument
    QList<QList<InstrumentIdAndLogo>> logos;   // UID => Logo
};

static void obtainInstrumentsForParallel(
    QThread* parentThread,
    int /*threadId*/,
    QList<tinkoff::InstrumentType>& instrumentTypes,
    int                             start,
    int                             end,
    void*                           additionalArgs
)
{
    ObtainInstrumentsInfo* obtainInstrumentsInfo = reinterpret_cast<ObtainInstrumentsInfo*>(additionalArgs);

    IGrpcClient*                grpcClient   = obtainInstrumentsInfo->grpcClient;
    Instruments*                resultsArray = obtainInstrumentsInfo->results.data();
    QList<InstrumentIdAndLogo>* logosArray   = obtainInstrumentsInfo->logos.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        InstrumentHandler instrumentHandler = INSTRUMENT_TYPE_TO_HANDLER.value(instrumentTypes.at(i));
        instrumentHandler(parentThread, grpcClient, resultsArray[i], logosArray[i]);
    }
}

void PriceCollectThread::downloadLogo(const QString& instrumentId, const QUrl& url)
{
    const IHttpClient::Headers headers;
    const HttpResult           httpResult = mHttpClient->download(url, headers);

    QPixmap logo;
    bool    good = false;

    if (httpResult.statusCode == HTTP_STATUS_CODE_OK && httpResult.body.size() > 0)
    {
        good = logo.loadFromData(httpResult.body, "PNG");

        if (good)
        {
            logo = logo.scaled(LOGO_SIZE, LOGO_SIZE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        }
    }

    if (!good)
    {
        const std::shared_ptr<IFile> noImageFile = mFileFactory->newInstance(":/assets/images/no_image.png");

        bool ok = noImageFile->open(QIODevice::ReadOnly);
        Q_ASSERT_X(ok, __FUNCTION__, "Failed to open file");

        const QByteArray content = noImageFile->readAll();
        noImageFile->close();

        ok = logo.loadFromData(content, "PNG");
        Q_ASSERT_X(ok, __FUNCTION__, "Failed to open file");
    }

    mLogosStorage->writeLock();
    mLogosStorage->setLogo(instrumentId, logo);
    mLogosStorage->writeUnlock();
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

static void downloadLogosForParallel(
    QThread* parentThread, int /*threadId*/, QList<InstrumentIdAndLogo>& logos, int start, int end, void* additionalArgs
)
{
    DownloadLogosInfo*  downloadLogosInfo = reinterpret_cast<DownloadLogosInfo*>(additionalArgs);
    PriceCollectThread* thread            = downloadLogosInfo->thread;
    IFileFactory*       fileFactory       = downloadLogosInfo->fileFactory;
    const bool          forceToDownload   = downloadLogosInfo->forceToDownload;

    const QString appDir = qApp->applicationDirPath();

    InstrumentIdAndLogo* logosArray = logos.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        const std::shared_ptr<IFile> logoFile =
            fileFactory->newInstance(QString("%1/data/instruments/logos/%2.png").arg(appDir, logosArray[i].instrumentId));

        if (forceToDownload || !logoFile->exists())
        {
            const QString logoName = logosArray[i].logo.replace(".png", "x160.png"); // 160 pixels
            const QUrl    url      = QUrl(QString("https://invest-brands.cdn-tinkoff.ru/%1").arg(logoName));

            thread->downloadLogo(logosArray[i].instrumentId, url);
        }

        downloadLogosInfo->finished++;

        emit thread->notifyInstrumentsProgress(
            PriceCollectThread::tr("Downloading logos") +
            QString(" (%1 / %2)").arg(QString::number(downloadLogosInfo->finished), QString::number(logos.size()))
        );
    }
}

void PriceCollectThread::storeNewInstrumentsInfo()
{
    QList<tinkoff::InstrumentType> instrumentTypes{
        tinkoff::INSTRUMENT_TYPE_SHARE,
        tinkoff::INSTRUMENT_TYPE_BOND,
        tinkoff::INSTRUMENT_TYPE_CURRENCY,
        tinkoff::INSTRUMENT_TYPE_ETF,
        tinkoff::INSTRUMENT_TYPE_FUTURES,
    };

    ObtainInstrumentsInfo obtainInstrumentsInfo(mGrpcClient, instrumentTypes);
    processInParallel(instrumentTypes, obtainInstrumentsForParallel, &obtainInstrumentsInfo);

    Instruments                instruments; // UID => Instrument
    QList<InstrumentIdAndLogo> logos;

    for (int i = 0; i < instrumentTypes.size(); ++i)
    {
        instruments.insert(obtainInstrumentsInfo.results.at(i));
        logos.append(obtainInstrumentsInfo.logos.at(i));
    }

    emit notifyInstrumentsProgress(tr("Downloading logos"));

    static int lastDownloadHour = -1;
    const int  currentHour      = QDateTime::currentDateTime().time().hour();

    if (lastDownloadHour < 0)
    {
        lastDownloadHour = currentHour;
    }

    DownloadLogosInfo downloadLogosInfo(this, mFileFactory, lastDownloadHour == currentHour);
    processInParallel(logos, downloadLogosForParallel, &downloadLogosInfo);

    mInstrumentsStorage->writeLock();
    mInstrumentsStorage->mergeInstruments(instruments);
    mInstrumentsStorage->writeUnlock();
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
            grpcClient->getCandles(parentThread, stock->meta.instrumentId, startTimestamp, endTimestamp);

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
    QThread*                      parentThread,
    IQZipFactory*                 qZipFactory,
    IQZipFileFactory*             qZipFileFactory,
    qint64                        startTimestamp,
    qint64                        endTimestamp,
    const std::shared_ptr<IFile>& stockDataFile,
    StockData*                    dataArray
)
{
    int indexOffset = 0;

    const std::shared_ptr<IQZip> stockDataZipFile = qZipFactory->newInstance(stockDataFile->getDevice());

    if (stockDataZipFile->open(QuaZip::mdUnzip))
    {
        const std::shared_ptr<IQZipFile> stockZippedFile = qZipFileFactory->newInstance(stockDataZipFile->getZip());

        QStringList zippedFiles = stockDataZipFile->getFileNameList();
        zippedFiles.sort();

        for (int i = 0; i < zippedFiles.size() && !parentThread->isInterruptionRequested(); ++i)
        {
            stockDataZipFile->setCurrentFile(zippedFiles.at(i));

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

        stockDataZipFile->close();
    }

    return indexOffset;
}

// NOLINTBEGIN(readability-function-cognitive-complexity)
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
        const QString zipFilePath =
            QString("%1/cache/stocks/%2_%3.zip").arg(appDir, stock->meta.instrumentId, QString::number(year));

        const std::shared_ptr<IFile> stockDataFile = fileFactory->newInstance(zipFilePath);

        if (year == endYear || !stockDataFile->exists())
        {
            QUrl url = QUrl("https://invest-public-api.tinkoff.ru/history-data");

            QUrlQuery query;

            query.addQueryItem("instrumentId", stock->meta.instrumentId);
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
                    Q_ASSERT_X(ok, __FUNCTION__, "Failed to open file");

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
            parentThread, qZipFactory, qZipFileFactory, startTimestamp, endTimestamp, stockDataFile, &dataArray[indexOffset]
        );
    }

    stocksStorage->appendStockData(stock, dataArray, indexOffset);
}
// NOLINTEND(readability-function-cognitive-complexity)

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

static void
getCandlesForParallel(QThread* parentThread, int /*threadId*/, QList<Stock*>& stocks, int start, int end, void* additionalArgs)
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
        Stock* stock = stockArray[i];

        stock->writeLock();

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

        stock->writeUnlock();

        getCandlesInfo->finished++;

        emit thread->notifyInstrumentsProgress(
            PriceCollectThread::tr("Obtain stocks data") +
            QString(" (%1 / %2)").arg(QString::number(getCandlesInfo->finished), QString::number(stocks.size()))
        );
    }
}

void PriceCollectThread::obtainStocksData()
{
    emit notifyInstrumentsProgress(tr("Obtain stocks data"));

    const std::shared_ptr<IDir> createDir = mDirFactory->newInstance();

    bool ok = createDir->mkpath(qApp->applicationDirPath() + "/cache/stocks");
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to create dir");

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

    QList<Stock*> stocks = mStocksStorage->getStocks();
    processInParallel(stocks, getCandlesForParallel, &getCandlesInfo);

    const std::shared_ptr<IDir> deleteDir = mDirFactory->newInstance(qApp->applicationDirPath() + "/cache/stocks");

    ok = deleteDir->removeRecursively();
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to delete dir");
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
