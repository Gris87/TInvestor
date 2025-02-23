#include "src/threads/pricecollect/pricecollectthread.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/stocks/istocksstorage_mock.h"
#include "src/utils/fs/file/ifilefactory_mock.h"
#include "src/utils/http/ihttpclientfactory_mock.h"



using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_PriceCollectThread : public ::testing::Test
{
protected:
    void SetUp()
    {
        stocksStorageMock     = new StrictMock<StocksStorageMock>();
        fileFactoryMock       = new StrictMock<FileFactoryMock>();
        httpClientFactoryMock = new StrictMock<HttpClientFactoryMock>();
        grpcClientMock        = new StrictMock<GrpcClientMock>();

        thread = new PriceCollectThread(stocksStorageMock, fileFactoryMock, httpClientFactoryMock, grpcClientMock);
    }

    void TearDown()
    {
        delete thread;
        delete stocksStorageMock;
        delete fileFactoryMock;
        delete httpClientFactoryMock;
        delete grpcClientMock;
    }

    PriceCollectThread*                thread;
    StrictMock<StocksStorageMock>*     stocksStorageMock;
    StrictMock<FileFactoryMock>*       fileFactoryMock;
    StrictMock<HttpClientFactoryMock>* httpClientFactoryMock;
    StrictMock<GrpcClientMock>*        grpcClientMock;
};



TEST_F(Test_PriceCollectThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_PriceCollectThread, Test_run)
{
}
