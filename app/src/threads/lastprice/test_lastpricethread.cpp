#include "src/threads/lastprice/lastpricethread.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/stocks/istocksstorage_mock.h"



using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_LastPriceThread : public ::testing::Test
{
protected:
    void SetUp()
    {
        stocksStorageMock = new StrictMock<StocksStorageMock>();
        grpcClientMock = new StrictMock<GrpcClientMock>();

        thread = new LastPriceThread(stocksStorageMock, grpcClientMock);
    }

    void TearDown()
    {
        delete thread;
        delete stocksStorageMock;
        delete grpcClientMock;
    }

    LastPriceThread*            thread;
    StrictMock<StocksStorageMock>* stocksStorageMock;
    StrictMock<GrpcClientMock>* grpcClientMock;
};



TEST_F(Test_LastPriceThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_LastPriceThread, Test_run)
{
}
