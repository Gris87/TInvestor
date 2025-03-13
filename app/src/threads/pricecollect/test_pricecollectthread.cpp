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



class Test_PriceCollectThread : public ::testing::Test
{
protected:
    void SetUp()
    {
        InSequence seq;

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

    void TearDown()
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
    InSequence seq;

    StrictMock<FileMock>* logoFileMock  = new StrictMock<FileMock>(); // Will be deleted in downloadLogosForParallel
    StrictMock<FileMock>* zipFileMock1  = new StrictMock<FileMock>(); // Will be deleted in getCandlesWithHttp
    StrictMock<FileMock>* zipFileMock2  = new StrictMock<FileMock>(); // Will be deleted in getCandlesWithHttp
    StrictMock<FileMock>* zipFileMock3  = new StrictMock<FileMock>(); // Will be deleted in getCandlesWithHttp
    StrictMock<QZipMock>* qZipFileMock1 = new StrictMock<QZipMock>(); // Will be deleted in getCandlesFromZipFile
    StrictMock<QZipMock>* qZipFileMock2 = new StrictMock<QZipMock>(); // Will be deleted in getCandlesFromZipFile
    StrictMock<QZipMock>* qZipFileMock3 = new StrictMock<QZipMock>(); // Will be deleted in getCandlesFromZipFile

    QMutex mutex;

    std::shared_ptr<tinkoff::SharesResponse> sharesResponse(new tinkoff::SharesResponse());

    tinkoff::Share* share = sharesResponse->add_instruments();

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

    EXPECT_CALL(*grpcClientMock, findStocks(NotNull())).WillOnce(Return(sharesResponse));
    EXPECT_CALL(*fileFactoryMock, newInstance(Ne(QString()))).WillOnce(Return(std::shared_ptr<IFile>(logoFileMock)));
    EXPECT_CALL(*logoFileMock, exists()).WillOnce(Return(false));
    EXPECT_CALL(
        *httpClientMock, download(QUrl(QString("https://invest-brands.cdn-tinkoff.ru/WAGAx160.png")), IHttpClient::Headers())
    )
        .WillOnce(Return(httpResult));
    EXPECT_CALL(*logoFileMock, open(QIODevice::OpenMode(QIODevice::WriteOnly))).WillOnce(Return(true));
    EXPECT_CALL(*logoFileMock, write(httpResult.body)).WillOnce(Return(1));
    EXPECT_CALL(*logoFileMock, close());
    EXPECT_CALL(*stocksStorageMock, getMutex()).WillOnce(Return(&mutex));
    EXPECT_CALL(*stocksStorageMock, mergeStocksMeta(Ne(QList<StockMeta>())));
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*configMock, getStorageMonthLimit()).WillRepeatedly(Return(24));
    EXPECT_CALL(*fileFactoryMock, newInstance(Ne(QString()))).WillOnce(Return(std::shared_ptr<IFile>(zipFileMock1)));
    EXPECT_CALL(*zipFileMock1, exists()).WillOnce(Return(false));
    EXPECT_CALL(*userStorageMock, getToken()).WillOnce(ReturnRef(token));
    EXPECT_CALL(*httpClientMock, download(Ne(QString()), Ne(IHttpClient::Headers()))).WillOnce(Return(tooManyRequestsHttpResult));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, NotNull())).WillOnce(Return(false));
    EXPECT_CALL(*httpClientMock, download(Ne(QString()), Ne(IHttpClient::Headers()))).WillOnce(Return(httpResult));
    EXPECT_CALL(*zipFileMock1, open(QIODevice::OpenMode(QIODevice::WriteOnly))).WillOnce(Return(true));
    EXPECT_CALL(*zipFileMock1, write(httpResult.body)).WillOnce(Return(1));
    EXPECT_CALL(*zipFileMock1, close());
    EXPECT_CALL(*qZipFactoryMock, newInstance(Ne(QString()))).WillOnce(Return(std::shared_ptr<IQZip>(qZipFileMock1)));
    EXPECT_CALL(*qZipFileMock1, open(QuaZip::mdUnzip)).WillOnce(Return(false));
    EXPECT_CALL(*fileFactoryMock, newInstance(Ne(QString()))).WillOnce(Return(std::shared_ptr<IFile>(zipFileMock2)));
    EXPECT_CALL(*zipFileMock2, exists()).WillOnce(Return(false));
    EXPECT_CALL(*userStorageMock, getToken()).WillOnce(ReturnRef(token));
    EXPECT_CALL(*httpClientMock, download(Ne(QString()), Ne(IHttpClient::Headers())))
        .WillOnce(Return(internalServerErrorHttpResult));
    EXPECT_CALL(*qZipFactoryMock, newInstance(Ne(QString()))).WillOnce(Return(std::shared_ptr<IQZip>(qZipFileMock2)));
    EXPECT_CALL(*qZipFileMock2, open(QuaZip::mdUnzip)).WillOnce(Return(false));
    EXPECT_CALL(*fileFactoryMock, newInstance(Ne(QString()))).WillOnce(Return(std::shared_ptr<IFile>(zipFileMock3)));
    EXPECT_CALL(*userStorageMock, getToken()).WillOnce(ReturnRef(token));
    EXPECT_CALL(*httpClientMock, download(Ne(QString()), Ne(IHttpClient::Headers()))).WillOnce(Return(tooManyRequestsHttpResult));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, NotNull())).WillOnce(Return(true));
    EXPECT_CALL(*qZipFactoryMock, newInstance(Ne(QString()))).WillOnce(Return(std::shared_ptr<IQZip>(qZipFileMock3)));
    EXPECT_CALL(*qZipFileMock3, open(QuaZip::mdUnzip)).WillOnce(Return(false));
    EXPECT_CALL(*stocksStorageMock, appendStockData(&stock, NotNull(), 0));
    EXPECT_CALL(*grpcClientMock, getCandles(NotNull(), stock.meta.uid, 60, Gt(1741726800))).WillOnce(Return(nullptr));
    EXPECT_CALL(*stocksStorageMock, obtainStocksDayStartPrice(Gt(1741726800000)));
    EXPECT_CALL(*stocksStorageMock, obtainPayback(Gt(1741726800000)));

    thread->run();
}

TEST_F(Test_PriceCollectThread, Test_obtainStocksDayStartPrice)
{
    InSequence seq;

    EXPECT_CALL(*stocksStorageMock, obtainStocksDayStartPrice(Gt(1741726800000)));

    ASSERT_EQ(thread->obtainStocksDayStartPrice(), true);
    ASSERT_EQ(thread->obtainStocksDayStartPrice(), false);
}
