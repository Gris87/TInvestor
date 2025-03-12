#include "src/threads/pricecollect/pricecollectthread.h"

#include <QCoreApplication>
#include <gtest/gtest.h>

#include "src/config/iconfig_mock.h"
#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/stocks/istocksstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/utils/fs/dir/idir_mock.h"
#include "src/utils/fs/dir/idirfactory_mock.h"
#include "src/utils/fs/file/ifilefactory_mock.h"
#include "src/utils/fs/zip/qzip/iqzipfactory_mock.h"
#include "src/utils/fs/zip/qzipfile/iqzipfilefactory_mock.h"
#include "src/utils/http/ihttpclient_mock.h"
#include "src/utils/timeutils/itimeutils_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
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
}
