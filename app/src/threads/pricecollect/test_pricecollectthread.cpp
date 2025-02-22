#include "src/threads/pricecollect/pricecollectthread.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/stocks/istocksstorage_mock.h"



using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_PriceCollectThread : public ::testing::Test
{
protected:
    void SetUp()
    {
        stocksStorageMock = new StrictMock<StocksStorageMock>();
        grpcClientMock = new StrictMock<GrpcClientMock>();

        thread = new PriceCollectThread(stocksStorageMock, grpcClientMock);
    }

    void TearDown()
    {
        delete thread;
        delete stocksStorageMock;
        delete grpcClientMock;
    }

    PriceCollectThread*         thread;
    StrictMock<StocksStorageMock>* stocksStorageMock;
    StrictMock<GrpcClientMock>* grpcClientMock;
};



TEST_F(Test_PriceCollectThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_PriceCollectThread, Test_run)
{
}
