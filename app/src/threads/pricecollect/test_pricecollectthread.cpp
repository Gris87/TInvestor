#include "src/threads/pricecollect/pricecollectthread.h"

#include <gtest/gtest.h>

#include "src/config/iconfig_mock.h"
#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/stocks/istocksstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/utils/fs/dir/idirfactory_mock.h"
#include "src/utils/fs/file/ifilefactory_mock.h"
#include "src/utils/http/ihttpclient_mock.h"



using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_PriceCollectThread : public ::testing::Test
{
protected:
    void SetUp()
    {
        configMock        = new StrictMock<ConfigMock>();
        userStorageMock   = new StrictMock<UserStorageMock>();
        stocksStorageMock = new StrictMock<StocksStorageMock>();
        dirFactoryMock    = new StrictMock<DirFactoryMock>();
        fileFactoryMock   = new StrictMock<FileFactoryMock>();
        httpClientMock    = new StrictMock<HttpClientMock>();
        grpcClientMock    = new StrictMock<GrpcClientMock>();

        thread = new PriceCollectThread(
            configMock, userStorageMock, stocksStorageMock, dirFactoryMock, fileFactoryMock, httpClientMock, grpcClientMock
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
        delete httpClientMock;
        delete grpcClientMock;
    }

    PriceCollectThread*            thread;
    StrictMock<ConfigMock>*        configMock;
    StrictMock<UserStorageMock>*   userStorageMock;
    StrictMock<StocksStorageMock>* stocksStorageMock;
    StrictMock<DirFactoryMock>*    dirFactoryMock;
    StrictMock<FileFactoryMock>*   fileFactoryMock;
    StrictMock<HttpClientMock>*    httpClientMock;
    StrictMock<GrpcClientMock>*    grpcClientMock;
};



TEST_F(Test_PriceCollectThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_PriceCollectThread, Test_run)
{
}
