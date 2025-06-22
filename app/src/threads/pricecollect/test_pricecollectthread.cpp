#include "src/threads/pricecollect/pricecollectthread.h"

#include <QBuffer>
#include <QCoreApplication>
#include <QPixmap>
#include <gtest/gtest.h>

#include "src/config/iconfig_mock.h"
#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/logos/ilogosstorage_mock.h"
#include "src/storage/stocks/istocksstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/utils/fs/dir/idir_mock.h"
#include "src/utils/fs/dir/idirfactory_mock.h"
#include "src/utils/fs/file/ifile_mock.h"
#include "src/utils/fs/file/ifilefactory_mock.h"
#include "src/utils/fs/zip/qzip/iqzip_mock.h"
#include "src/utils/fs/zip/qzip/iqzipfactory_mock.h"
#include "src/utils/fs/zip/qzipfile/iqzipfile_mock.h"
#include "src/utils/fs/zip/qzipfile/iqzipfilefactory_mock.h"
#include "src/utils/http/ihttpclient_mock.h"
#include "src/utils/timeutils/itimeutils_mock.h"



using ::testing::_;
using ::testing::Gt;
using ::testing::InSequence;
using ::testing::Ne;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_PriceCollectThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        appDir = qApp->applicationDirPath();

        configMock             = new StrictMock<ConfigMock>();
        userStorageMock        = new StrictMock<UserStorageMock>();
        stocksStorageMock      = new StrictMock<StocksStorageMock>();
        instrumentsStorageMock = new StrictMock<InstrumentsStorageMock>();
        logosStorageMock       = new StrictMock<LogosStorageMock>();
        dirFactoryMock         = new StrictMock<DirFactoryMock>();
        fileFactoryMock        = new StrictMock<FileFactoryMock>();
        qZipFactoryMock        = new StrictMock<QZipFactoryMock>();
        qZipFileFactoryMock    = new StrictMock<QZipFileFactoryMock>();
        timeUtilsMock          = new StrictMock<TimeUtilsMock>();
        httpClientMock         = new StrictMock<HttpClientMock>();
        grpcClientMock         = new StrictMock<GrpcClientMock>();

        thread = new PriceCollectThread(
            configMock,
            userStorageMock,
            stocksStorageMock,
            instrumentsStorageMock,
            logosStorageMock,
            dirFactoryMock,
            fileFactoryMock,
            qZipFactoryMock,
            qZipFileFactoryMock,
            timeUtilsMock,
            httpClientMock,
            grpcClientMock
        );
    }

    void TearDown() override
    {
        delete thread;
        delete configMock;
        delete userStorageMock;
        delete stocksStorageMock;
        delete instrumentsStorageMock;
        delete logosStorageMock;
        delete dirFactoryMock;
        delete fileFactoryMock;
        delete qZipFactoryMock;
        delete qZipFileFactoryMock;
        delete timeUtilsMock;
        delete httpClientMock;
        delete grpcClientMock;
    }

    PriceCollectThread*                 thread;
    StrictMock<ConfigMock>*             configMock;
    StrictMock<UserStorageMock>*        userStorageMock;
    StrictMock<StocksStorageMock>*      stocksStorageMock;
    StrictMock<InstrumentsStorageMock>* instrumentsStorageMock;
    StrictMock<LogosStorageMock>*       logosStorageMock;
    StrictMock<DirFactoryMock>*         dirFactoryMock;
    StrictMock<FileFactoryMock>*        fileFactoryMock;
    StrictMock<QZipFactoryMock>*        qZipFactoryMock;
    StrictMock<QZipFileFactoryMock>*    qZipFileFactoryMock;
    StrictMock<TimeUtilsMock>*          timeUtilsMock;
    StrictMock<HttpClientMock>*         httpClientMock;
    StrictMock<GrpcClientMock>*         grpcClientMock;
    QString                             appDir;
};



TEST_F(Test_PriceCollectThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_PriceCollectThread, Test_run)
{
    const InSequence seq;

    StrictMock<FileMock>*     logoFileMock1   = new StrictMock<FileMock>();     // Will be deleted in downloadLogosForParallel
    StrictMock<FileMock>*     logoFileMock2   = new StrictMock<FileMock>();     // Will be deleted in downloadLogosForParallel
    StrictMock<FileMock>*     logoFileMock3   = new StrictMock<FileMock>();     // Will be deleted in downloadLogosForParallel
    StrictMock<FileMock>*     logoFileMock4   = new StrictMock<FileMock>();     // Will be deleted in downloadLogosForParallel
    StrictMock<FileMock>*     logoFileMock5   = new StrictMock<FileMock>();     // Will be deleted in downloadLogosForParallel
    StrictMock<FileMock>*     noImageFileMock = new StrictMock<FileMock>();     // Will be deleted in downloadLogo
    StrictMock<DirMock>*      dirMock1        = new StrictMock<DirMock>();      // Will be deleted in obtainStocksData
    StrictMock<DirMock>*      dirMock2        = new StrictMock<DirMock>();      // Will be deleted in obtainStocksData
    StrictMock<FileMock>*     zipFileMock1    = new StrictMock<FileMock>();     // Will be deleted in getCandlesWithHttp
    StrictMock<FileMock>*     zipFileMock2    = new StrictMock<FileMock>();     // Will be deleted in getCandlesWithHttp
    StrictMock<FileMock>*     zipFileMock3    = new StrictMock<FileMock>();     // Will be deleted in getCandlesWithHttp
    StrictMock<QZipMock>*     qZipMock1       = new StrictMock<QZipMock>();     // Will be deleted in getCandlesFromZipFile
    StrictMock<QZipMock>*     qZipMock2       = new StrictMock<QZipMock>();     // Will be deleted in getCandlesFromZipFile
    StrictMock<QZipMock>*     qZipMock3       = new StrictMock<QZipMock>();     // Will be deleted in getCandlesFromZipFile
    StrictMock<QZipFileMock>* qZipFileMock1   = new StrictMock<QZipFileMock>(); // Will be deleted in getCandlesFromZipFile

    const std::shared_ptr<tinkoff::SharesResponse> stocksResponse(new tinkoff::SharesResponse());
    tinkoff::Share*                                stockShare = stocksResponse->add_instruments();

    tinkoff::Quotation* minPriceIncrement = new tinkoff::Quotation(); // stockShare will take ownership
    minPriceIncrement->set_units(1);
    minPriceIncrement->set_nano(500000000);

    tinkoff::BrandData* stockBrand = new tinkoff::BrandData(); // stockShare will take ownership
    stockBrand->set_logo_name("WAGA.png");

    stockShare->set_currency("rub");
    stockShare->set_country_of_risk("RU");
    stockShare->set_api_trade_available_flag(true);
    stockShare->set_uid("aaaaa");
    stockShare->set_ticker("WAGA");
    stockShare->set_name("Wagadaka too");
    stockShare->set_for_qual_investor_flag(true);
    stockShare->set_lot(100);
    stockShare->set_allocated_min_price_increment(minPriceIncrement);
    stockShare->set_allocated_brand(stockBrand);

    const std::shared_ptr<tinkoff::SharesResponse> sharesResponse(new tinkoff::SharesResponse());
    tinkoff::Share*                                share = sharesResponse->add_instruments();

    tinkoff::BrandData* shareBrand = new tinkoff::BrandData(); // share will take ownership
    shareBrand->set_logo_name("ZARA.png");

    share->set_currency("rub");
    share->set_country_of_risk("RU");
    share->set_api_trade_available_flag(true);
    share->set_uid("bbbbb");
    share->set_ticker("ZARA");
    share->set_name("Zaraza");
    share->set_allocated_brand(shareBrand);

    const std::shared_ptr<tinkoff::BondsResponse> bondsResponse(new tinkoff::BondsResponse());
    tinkoff::Bond*                                bond = bondsResponse->add_instruments();

    tinkoff::BrandData* bondBrand = new tinkoff::BrandData(); // bond will take ownership
    bondBrand->set_logo_name("LOTO.png");

    bond->set_currency("rub");
    bond->set_country_of_risk("RU");
    bond->set_api_trade_available_flag(true);
    bond->set_uid("ccccc");
    bond->set_ticker("LOTO");
    bond->set_name("Sport loto");
    bond->set_allocated_brand(bondBrand);

    const std::shared_ptr<tinkoff::CurrenciesResponse> currenciesResponse(new tinkoff::CurrenciesResponse());
    tinkoff::Currency*                                 currency = currenciesResponse->add_instruments();

    tinkoff::BrandData* currencyBrand = new tinkoff::BrandData(); // currency will take ownership
    currencyBrand->set_logo_name("USD.png");

    currency->set_currency("rub");
    currency->set_country_of_risk("RU");
    currency->set_api_trade_available_flag(true);
    currency->set_uid("ddddd");
    currency->set_ticker("USDRUB");
    currency->set_name("USD to RUB");
    currency->set_allocated_brand(currencyBrand);

    const std::shared_ptr<tinkoff::EtfsResponse> etfsResponse(new tinkoff::EtfsResponse());
    tinkoff::Etf*                                etf = etfsResponse->add_instruments();

    tinkoff::BrandData* etfBrand = new tinkoff::BrandData(); // etf will take ownership
    etfBrand->set_logo_name("DOGS.png");

    etf->set_currency("rub");
    etf->set_country_of_risk("RU");
    etf->set_api_trade_available_flag(true);
    etf->set_uid("eeeee");
    etf->set_ticker("Dogs");
    etf->set_name("Save dogs");
    etf->set_allocated_brand(etfBrand);

    const std::shared_ptr<tinkoff::FuturesResponse> futuresResponse(new tinkoff::FuturesResponse());
    tinkoff::Future*                                future = futuresResponse->add_instruments();

    tinkoff::BrandData* futureBrand = new tinkoff::BrandData(); // future will take ownership
    futureBrand->set_logo_name("GOLD.png");

    future->set_currency("rub");
    future->set_country_of_risk("RU");
    future->set_api_trade_available_flag(true);
    future->set_uid("fffff");
    future->set_ticker("GOLD");
    future->set_name("Man, I am rich");
    future->set_allocated_brand(futureBrand);

    Stock         stock;
    QList<Stock*> stocks;

    stock.meta.instrumentId        = "aaaaa";
    stock.meta.instrumentTicker    = "WAGA";
    stock.meta.instrumentName      = "Wagadaka too";
    stock.meta.forQualInvestorFlag = true;
    stock.meta.minPriceIncrement   = 0.001f;
    stock.meta.pricePrecision      = 3;

    stocks << &stock;

    QBuffer logoBuffer;

    QPixmap logoImage(1, 1);
    logoImage.save(&logoBuffer, "PNG");

    HttpResult httpResult;
    httpResult.statusCode = 200;
    httpResult.body       = logoBuffer.data();

    HttpResult tooManyRequestsHttpResult;
    tooManyRequestsHttpResult.statusCode = 429;

    HttpResult internalServerErrorHttpResult;
    internalServerErrorHttpResult.statusCode = 500;
    internalServerErrorHttpResult.body       = QString("Something uninteresting").toUtf8();

    QString token = "SomeToken";

    IHttpClient::Headers headers;
    headers["Authorization"] = "Bearer SomeToken";

    QuaZip zip;

    const std::shared_ptr<tinkoff::GetCandlesResponse> getCandlesResponse(new tinkoff::GetCandlesResponse());
    tinkoff::HistoricCandle*                           candle = getCandlesResponse->add_candles();

    tinkoff::Quotation* price = new tinkoff::Quotation(); // marketDataResponse will take ownership
    price->set_units(125);
    price->set_nano(500000000);

    google::protobuf::Timestamp* time = new google::protobuf::Timestamp(); // marketDataResponse will take ownership
    time->set_seconds(1000);
    time->set_nanos(0);

    candle->set_is_complete(true);
    candle->set_allocated_time(time);
    candle->set_allocated_close(price);

    const std::shared_ptr<tinkoff::GetCandlesResponse> emptyCandlesResponse(new tinkoff::GetCandlesResponse());

    EXPECT_CALL(*grpcClientMock, findStocks(QThread::currentThread(), tinkoff::INSTRUMENT_STATUS_BASE))
        .WillOnce(Return(stocksResponse));
    EXPECT_CALL(*stocksStorageMock, lock());
    EXPECT_CALL(*stocksStorageMock, mergeStocksMeta(Ne(QList<StockMeta>()))).WillOnce(Return(true));
    EXPECT_CALL(*stocksStorageMock, unlock());

    EXPECT_CALL(*grpcClientMock, findStocks(QThread::currentThread(), tinkoff::INSTRUMENT_STATUS_ALL))
        .WillOnce(Return(sharesResponse));
    EXPECT_CALL(*grpcClientMock, findBonds(QThread::currentThread())).WillOnce(Return(bondsResponse));
    EXPECT_CALL(*grpcClientMock, findCurrencies(QThread::currentThread())).WillOnce(Return(currenciesResponse));
    EXPECT_CALL(*grpcClientMock, findEtfs(QThread::currentThread())).WillOnce(Return(etfsResponse));
    EXPECT_CALL(*grpcClientMock, findFutures(QThread::currentThread())).WillOnce(Return(futuresResponse));

    EXPECT_CALL(*fileFactoryMock, newInstance(QString("%1/data/instruments/logos/bbbbb.png").arg(appDir)))
        .WillOnce(Return(std::shared_ptr<IFile>(logoFileMock1)));
    EXPECT_CALL(
        *httpClientMock, download(QUrl(QString("https://invest-brands.cdn-tinkoff.ru/ZARAx160.png")), IHttpClient::Headers())
    )
        .WillOnce(Return(httpResult));
    EXPECT_CALL(*logosStorageMock, lock());
    EXPECT_CALL(*logosStorageMock, setLogo(QString("bbbbb"), _));
    EXPECT_CALL(*logosStorageMock, unlock());

    EXPECT_CALL(*fileFactoryMock, newInstance(QString("%1/data/instruments/logos/ccccc.png").arg(appDir)))
        .WillOnce(Return(std::shared_ptr<IFile>(logoFileMock2)));
    EXPECT_CALL(
        *httpClientMock, download(QUrl(QString("https://invest-brands.cdn-tinkoff.ru/LOTOx160.png")), IHttpClient::Headers())
    )
        .WillOnce(Return(httpResult));
    EXPECT_CALL(*logosStorageMock, lock());
    EXPECT_CALL(*logosStorageMock, setLogo(QString("ccccc"), _));
    EXPECT_CALL(*logosStorageMock, unlock());

    EXPECT_CALL(*fileFactoryMock, newInstance(QString("%1/data/instruments/logos/ddddd.png").arg(appDir)))
        .WillOnce(Return(std::shared_ptr<IFile>(logoFileMock3)));
    EXPECT_CALL(
        *httpClientMock, download(QUrl(QString("https://invest-brands.cdn-tinkoff.ru/USDx160.png")), IHttpClient::Headers())
    )
        .WillOnce(Return(httpResult));
    EXPECT_CALL(*logosStorageMock, lock());
    EXPECT_CALL(*logosStorageMock, setLogo(QString("ddddd"), _));
    EXPECT_CALL(*logosStorageMock, unlock());

    EXPECT_CALL(*fileFactoryMock, newInstance(QString("%1/data/instruments/logos/eeeee.png").arg(appDir)))
        .WillOnce(Return(std::shared_ptr<IFile>(logoFileMock4)));
    EXPECT_CALL(
        *httpClientMock, download(QUrl(QString("https://invest-brands.cdn-tinkoff.ru/DOGSx160.png")), IHttpClient::Headers())
    )
        .WillOnce(Return(httpResult));
    EXPECT_CALL(*logosStorageMock, lock());
    EXPECT_CALL(*logosStorageMock, setLogo(QString("eeeee"), _));
    EXPECT_CALL(*logosStorageMock, unlock());

    EXPECT_CALL(*fileFactoryMock, newInstance(QString("%1/data/instruments/logos/fffff.png").arg(appDir)))
        .WillOnce(Return(std::shared_ptr<IFile>(logoFileMock5)));
    EXPECT_CALL(
        *httpClientMock, download(QUrl(QString("https://invest-brands.cdn-tinkoff.ru/GOLDx160.png")), IHttpClient::Headers())
    )
        .WillOnce(Return(internalServerErrorHttpResult));
    EXPECT_CALL(*fileFactoryMock, newInstance(QString(":/assets/images/no_image.png")))
        .WillOnce(Return(std::shared_ptr<IFile>(noImageFileMock)));
    EXPECT_CALL(*noImageFileMock, open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
    EXPECT_CALL(*noImageFileMock, readAll()).WillOnce(Return(logoBuffer.data()));
    EXPECT_CALL(*noImageFileMock, close());
    EXPECT_CALL(*logosStorageMock, lock());
    EXPECT_CALL(*logosStorageMock, setLogo(QString("fffff"), _));
    EXPECT_CALL(*logosStorageMock, unlock());

    EXPECT_CALL(*instrumentsStorageMock, lock());
    EXPECT_CALL(*instrumentsStorageMock, mergeInstruments(Ne(Instruments())));
    EXPECT_CALL(*instrumentsStorageMock, unlock());

    EXPECT_CALL(*dirFactoryMock, newInstance(QString())).WillOnce(Return(std::shared_ptr<IDir>(dirMock1)));
    EXPECT_CALL(*dirMock1, mkpath(appDir + "/cache/stocks")).WillOnce(Return(true));

    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*configMock, getStorageMonthLimit()).WillOnce(Return(24));

    EXPECT_CALL(*fileFactoryMock, newInstance(QString("%1/cache/stocks/aaaaa_2023.zip").arg(appDir)))
        .WillOnce(Return(std::shared_ptr<IFile>(zipFileMock1)));
    EXPECT_CALL(*zipFileMock1, exists()).WillOnce(Return(false));
    EXPECT_CALL(*userStorageMock, getToken()).WillOnce(ReturnRef(token));
    EXPECT_CALL(
        *httpClientMock, download(QUrl("https://invest-public-api.tinkoff.ru/history-data?instrumentId=aaaaa&year=2023"), headers)
    )
        .WillOnce(Return(tooManyRequestsHttpResult));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(
        *httpClientMock, download(QUrl("https://invest-public-api.tinkoff.ru/history-data?instrumentId=aaaaa&year=2023"), headers)
    )
        .WillOnce(Return(httpResult));
    EXPECT_CALL(*zipFileMock1, open(QIODevice::OpenMode(QIODevice::WriteOnly))).WillOnce(Return(true));
    EXPECT_CALL(*zipFileMock1, write(httpResult.body)).WillOnce(Return(1));
    EXPECT_CALL(*zipFileMock1, close());
    EXPECT_CALL(*qZipFactoryMock, newInstance(QString("%1/cache/stocks/aaaaa_2023.zip").arg(appDir)))
        .WillOnce(Return(std::shared_ptr<IQZip>(qZipMock1)));
    EXPECT_CALL(*qZipMock1, open(QuaZip::mdUnzip)).WillOnce(Return(true));
    EXPECT_CALL(*qZipMock1, getZip()).WillOnce(Return(&zip));
    EXPECT_CALL(*qZipFileFactoryMock, newInstance(&zip)).WillOnce(Return(std::shared_ptr<IQZipFile>(qZipFileMock1)));
    EXPECT_CALL(*qZipMock1, getFileNameList()).WillOnce(Return(QStringList() << "data.txt"));
    EXPECT_CALL(*qZipMock1, setCurrentFile(QString("data.txt")));
    EXPECT_CALL(*qZipFileMock1, open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
    EXPECT_CALL(*qZipFileMock1, readAll())
        .WillOnce(Return(
            "02b2ea14-3c4b-47e8-9548-45a8dbcc8f8a;2025-03-12T03:59:00Z;124.45;124.45;124.45;124.45;2759;\n"
            "02b2ea14-3c4b-47e8-9548-45a8dbcc8f8a;2025-03-12T04:00:00Z;124.45;124.05;124.45;123.65;2914;"
        ));
    EXPECT_CALL(*qZipFileMock1, close());
    EXPECT_CALL(*qZipMock1, close());

    EXPECT_CALL(*fileFactoryMock, newInstance(QString("%1/cache/stocks/aaaaa_2024.zip").arg(appDir)))
        .WillOnce(Return(std::shared_ptr<IFile>(zipFileMock2)));
    EXPECT_CALL(*zipFileMock2, exists()).WillOnce(Return(false));
    EXPECT_CALL(*userStorageMock, getToken()).WillOnce(ReturnRef(token));
    EXPECT_CALL(
        *httpClientMock, download(QUrl("https://invest-public-api.tinkoff.ru/history-data?instrumentId=aaaaa&year=2024"), headers)
    )
        .WillOnce(Return(internalServerErrorHttpResult));
    EXPECT_CALL(*qZipFactoryMock, newInstance(QString("%1/cache/stocks/aaaaa_2024.zip").arg(appDir)))
        .WillOnce(Return(std::shared_ptr<IQZip>(qZipMock2)));
    EXPECT_CALL(*qZipMock2, open(QuaZip::mdUnzip)).WillOnce(Return(false));

    EXPECT_CALL(*fileFactoryMock, newInstance(QString("%1/cache/stocks/aaaaa_2025.zip").arg(appDir)))
        .WillOnce(Return(std::shared_ptr<IFile>(zipFileMock3)));
    EXPECT_CALL(*userStorageMock, getToken()).WillOnce(ReturnRef(token));
    EXPECT_CALL(
        *httpClientMock, download(QUrl("https://invest-public-api.tinkoff.ru/history-data?instrumentId=aaaaa&year=2025"), headers)
    )
        .WillOnce(Return(tooManyRequestsHttpResult));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(true));
    EXPECT_CALL(*qZipFactoryMock, newInstance(QString("%1/cache/stocks/aaaaa_2025.zip").arg(appDir)))
        .WillOnce(Return(std::shared_ptr<IQZip>(qZipMock3)));
    EXPECT_CALL(*qZipMock3, open(QuaZip::mdUnzip)).WillOnce(Return(false));

    EXPECT_CALL(*stocksStorageMock, appendStockData(&stock, NotNull(), 2));
    EXPECT_CALL(*grpcClientMock, getCandles(QThread::currentThread(), QString("aaaaa"), 60000, Gt(1704056400000)))
        .WillOnce(Return(getCandlesResponse));
    EXPECT_CALL(*grpcClientMock, getCandles(QThread::currentThread(), QString("aaaaa"), 60000, 1000000))
        .WillOnce(Return(emptyCandlesResponse));
    EXPECT_CALL(*stocksStorageMock, appendStockData(&stock, NotNull(), 1));

    EXPECT_CALL(*dirFactoryMock, newInstance(QString(appDir + "/cache/stocks")))
        .WillOnce(Return(std::shared_ptr<IDir>(dirMock2)));
    EXPECT_CALL(*dirMock2, removeRecursively()).WillOnce(Return(true));

    EXPECT_CALL(*stocksStorageMock, cleanupOperationalData(Gt(1704056400000)));
    EXPECT_CALL(*stocksStorageMock, obtainStocksDayStartPrice(Gt(1704056400000)));
    EXPECT_CALL(*stocksStorageMock, obtainTurnover(Gt(1704056400000)));
    EXPECT_CALL(*stocksStorageMock, obtainPayback(Gt(1704056400000)));

    thread->run();
}

TEST_F(Test_PriceCollectThread, Test_obtainStocksDayStartPrice)
{
    const InSequence seq;

    EXPECT_CALL(*stocksStorageMock, obtainStocksDayStartPrice(Gt(1704056400000)));

    ASSERT_EQ(thread->obtainStocksDayStartPrice(), true);
    ASSERT_EQ(thread->obtainStocksDayStartPrice(), false);
}

TEST_F(Test_PriceCollectThread, Test_notifyAboutChanges)
{
    thread->notifyAboutChanges(false, false);
    thread->notifyAboutChanges(false, true);
    thread->notifyAboutChanges(true, false);
    thread->notifyAboutChanges(true, true);
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
