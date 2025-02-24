#include "src/threads/pricecollect/pricecollectthread.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/stocks/istocksstorage_mock.h"
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
        stocksStorageMock = new StrictMock<StocksStorageMock>();
        fileFactoryMock   = new StrictMock<FileFactoryMock>();
        httpClientMock    = new StrictMock<HttpClientMock>();
        grpcClientMock    = new StrictMock<GrpcClientMock>();

        thread = new PriceCollectThread(stocksStorageMock, fileFactoryMock, httpClientMock, grpcClientMock);
    }

    void TearDown()
    {
        delete thread;
        delete stocksStorageMock;
        delete fileFactoryMock;
        delete httpClientMock;
        delete grpcClientMock;
    }

    PriceCollectThread*            thread;
    StrictMock<StocksStorageMock>* stocksStorageMock;
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
