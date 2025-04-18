#include "src/threads/pricecollect/pricecollectthread.h"

#include <QCoreApplication>
#include <gtest/gtest.h>

#include "src/config/iconfig_mock.h"
#include "src/grpc/igrpcclient_mock.h"
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
        const InSequence seq;

        appDir = qApp->applicationDirPath();

        StrictMock<DirMock>* dirMock = new StrictMock<DirMock>(); // Will be deleted in PriceCollectThread constructor
        configMock                   = new StrictMock<ConfigMock>();
        userStorageMock              = new StrictMock<UserStorageMock>();
        stocksStorageMock            = new StrictMock<StocksStorageMock>();
        dirFactoryMock               = new StrictMock<DirFactoryMock>();
        fileFactoryMock              = new StrictMock<FileFactoryMock>();
        qZipFactoryMock              = new StrictMock<QZipFactoryMock>();
        qZipFileFactoryMock          = new StrictMock<QZipFileFactoryMock>();
        timeUtilsMock                = new StrictMock<TimeUtilsMock>();
        httpClientMock               = new StrictMock<HttpClientMock>();
        grpcClientMock               = new StrictMock<GrpcClientMock>();

        EXPECT_CALL(*dirFactoryMock, newInstance(QString())).WillOnce(Return(std::shared_ptr<IDir>(dirMock)));
        EXPECT_CALL(*dirMock, mkpath(appDir + "/cache/stocks")).WillOnce(Return(true));

        thread = new PriceCollectThread(
            configMock,
            userStorageMock,
            stocksStorageMock,
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
        delete dirFactoryMock;
        delete fileFactoryMock;
        delete qZipFactoryMock;
        delete qZipFileFactoryMock;
        delete timeUtilsMock;
        delete httpClientMock;
        delete grpcClientMock;
    }

    PriceCollectThread*              thread;
    StrictMock<ConfigMock>*          configMock;
    StrictMock<UserStorageMock>*     userStorageMock;
    StrictMock<StocksStorageMock>*   stocksStorageMock;
    StrictMock<DirFactoryMock>*      dirFactoryMock;
    StrictMock<FileFactoryMock>*     fileFactoryMock;
    StrictMock<QZipFactoryMock>*     qZipFactoryMock;
    StrictMock<QZipFileFactoryMock>* qZipFileFactoryMock;
    StrictMock<TimeUtilsMock>*       timeUtilsMock;
    StrictMock<HttpClientMock>*      httpClientMock;
    StrictMock<GrpcClientMock>*      grpcClientMock;
    QString                          appDir;
};



TEST_F(Test_PriceCollectThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_PriceCollectThread, Test_run)
{
    const InSequence seq;

    StrictMock<FileMock>*     logoFileMock  = new StrictMock<FileMock>();     // Will be deleted in downloadLogosForParallel
    StrictMock<FileMock>*     zipFileMock1  = new StrictMock<FileMock>();     // Will be deleted in getCandlesWithHttp
    StrictMock<FileMock>*     zipFileMock2  = new StrictMock<FileMock>();     // Will be deleted in getCandlesWithHttp
    StrictMock<FileMock>*     zipFileMock3  = new StrictMock<FileMock>();     // Will be deleted in getCandlesWithHttp
    StrictMock<QZipMock>*     qZipMock1     = new StrictMock<QZipMock>();     // Will be deleted in getCandlesFromZipFile
    StrictMock<QZipMock>*     qZipMock2     = new StrictMock<QZipMock>();     // Will be deleted in getCandlesFromZipFile
    StrictMock<QZipMock>*     qZipMock3     = new StrictMock<QZipMock>();     // Will be deleted in getCandlesFromZipFile
    StrictMock<QZipFileMock>* qZipFileMock1 = new StrictMock<QZipFileMock>(); // Will be deleted in getCandlesFromZipFile

    QMutex mutex;

    const std::shared_ptr<tinkoff::SharesResponse> sharesResponse(new tinkoff::SharesResponse());
    tinkoff::Share*                                share = sharesResponse->add_instruments();

    tinkoff::Quotation* minPriceIncrement = new tinkoff::Quotation(); // share will take ownership
    minPriceIncrement->set_units(1);
    minPriceIncrement->set_nano(500000000);

    tinkoff::BrandData* brand = new tinkoff::BrandData(); // share will take ownership
    brand->set_logo_name("WAGA.png");

    share->set_currency("rub");
    share->set_country_of_risk("RU");
    share->set_api_trade_available_flag(true);
    share->set_uid("aaaaa");
    share->set_ticker("WAGA");
    share->set_name("Wagadaka too");
    share->set_for_qual_investor_flag(true);
    share->set_lot(100);
    share->set_allocated_min_price_increment(minPriceIncrement);
    share->set_allocated_brand(brand);

    Stock         stock;
    QList<Stock*> stocks;

    stock.meta.uid                     = "aaaaa";
    stock.meta.ticker                  = "WAGA";
    stock.meta.name                    = "Wagadaka too";
    stock.meta.forQualInvestorFlag     = true;
    stock.meta.lot                     = 100;
    stock.meta.minPriceIncrement.units = 1;
    stock.meta.minPriceIncrement.nano  = 500000000;

    stocks << &stock;

    HttpResult httpResult;
    httpResult.statusCode = 200;
    httpResult.body       = QString("What are doing here?").toUtf8();

    HttpResult tooManyRequestsHttpResult;
    tooManyRequestsHttpResult.statusCode = 429;

    HttpResult internalServerErrorHttpResult;
    internalServerErrorHttpResult.statusCode = 500;

    QString token = "SomeToken";

    IHttpClient::Headers headers;
    headers["Authorization"] = "Bearer SomeToken";

    QuaZip zip;

    const std::shared_ptr<tinkoff::GetCandlesResponse> getCandlesResponse(new tinkoff::GetCandlesResponse());
    tinkoff::HistoricCandle*                           candle = getCandlesResponse->add_candles();

    tinkoff::Quotation* price = new tinkoff::Quotation(); // marketDataResponse will take ownership
    price->set_units(125);
    price->set_nano(500000000);

    ::google::protobuf::Timestamp* time = new ::google::protobuf::Timestamp(); // marketDataResponse will take ownership
    time->set_seconds(1000);
    time->set_nanos(0);

    candle->set_is_complete(true);
    candle->set_allocated_time(time);
    candle->set_allocated_close(price);

    const std::shared_ptr<tinkoff::GetCandlesResponse> emptyCandlesResponse(new tinkoff::GetCandlesResponse());

    EXPECT_CALL(*grpcClientMock, findStocks(QThread::currentThread())).WillOnce(Return(sharesResponse));
    EXPECT_CALL(*fileFactoryMock, newInstance(QString("%1/data/stocks/logos/aaaaa.png").arg(appDir)))
        .WillOnce(Return(std::shared_ptr<IFile>(logoFileMock)));
    EXPECT_CALL(
        *httpClientMock, download(QUrl(QString("https://invest-brands.cdn-tinkoff.ru/WAGAx160.png")), IHttpClient::Headers())
    )
        .WillOnce(Return(httpResult));
    EXPECT_CALL(*logoFileMock, open(QIODevice::OpenMode(QIODevice::WriteOnly))).WillOnce(Return(true));
    EXPECT_CALL(*logoFileMock, write(httpResult.body)).WillOnce(Return(1));
    EXPECT_CALL(*logoFileMock, close());
    EXPECT_CALL(*stocksStorageMock, getMutex()).WillOnce(Return(&mutex));
    EXPECT_CALL(*stocksStorageMock, mergeStocksMeta(Ne(QList<StockMeta>()))).WillOnce(Return(true));
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*configMock, getStorageMonthLimit()).WillRepeatedly(Return(24));
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
    EXPECT_CALL(*stocksStorageMock, cleanupOperationalData(Gt(1704056400000)));
    EXPECT_CALL(*stocksStorageMock, obtainStocksDayStartPrice(Gt(1704056400000)));
    EXPECT_CALL(*stocksStorageMock, obtainTurnover(Gt(1704056400000)));
    EXPECT_CALL(*stocksStorageMock, obtainPayback(Gt(1704056400000)));

    thread->run();
}

TEST_F(Test_PriceCollectThread, Test_downloadLogo)
{
    const InSequence seq;

    StrictMock<FileMock>*        logoFileMock    = new StrictMock<FileMock>();
    StrictMock<FileMock>*        noImageFileMock = new StrictMock<FileMock>(); // Will be deleted in downloadLogo
    const std::shared_ptr<IFile> sharedLogoFile(logoFileMock);

    HttpResult httpResult;
    httpResult.statusCode = 200;
    httpResult.body       = QString("What are doing here?").toUtf8();

    HttpResult internalServerErrorHttpResult;
    internalServerErrorHttpResult.statusCode = 500;
    internalServerErrorHttpResult.body       = QString("Something uninteresting").toUtf8();

    EXPECT_CALL(*httpClientMock, download(QUrl("https://yandex.ru"), IHttpClient::Headers())).WillOnce(Return(httpResult));
    EXPECT_CALL(*logoFileMock, open(QIODevice::OpenMode(QIODevice::WriteOnly))).WillOnce(Return(true));
    EXPECT_CALL(*logoFileMock, write(httpResult.body)).WillOnce(Return(1));
    EXPECT_CALL(*logoFileMock, close());

    thread->downloadLogo(QUrl("https://yandex.ru"), sharedLogoFile);

    EXPECT_CALL(*httpClientMock, download(QUrl("https://hooyandex.ru"), IHttpClient::Headers()))
        .WillOnce(Return(internalServerErrorHttpResult));
    EXPECT_CALL(*fileFactoryMock, newInstance(QString(":/assets/images/no_image.png")))
        .WillOnce(Return(std::shared_ptr<IFile>(noImageFileMock)));
    EXPECT_CALL(*noImageFileMock, open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
    EXPECT_CALL(*logoFileMock, open(QIODevice::OpenMode(QIODevice::WriteOnly))).WillOnce(Return(true));
    EXPECT_CALL(*noImageFileMock, readAll()).WillOnce(Return(internalServerErrorHttpResult.body));
    EXPECT_CALL(*logoFileMock, write(internalServerErrorHttpResult.body)).WillOnce(Return(1));
    EXPECT_CALL(*logoFileMock, close());
    EXPECT_CALL(*noImageFileMock, close());

    thread->downloadLogo(QUrl("https://hooyandex.ru"), sharedLogoFile);
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
