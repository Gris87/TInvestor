#include "src/threads/cleanupthread.h"

#include <gtest/gtest.h>

#include "src/db/stocks/istocksdatabase_mock.h"
#include "src/storage/istocksstorage_mock.h"



using ::testing::StrictMock;
using ::testing::NotNull;
using ::testing::Return;



class Test_CleanupThread : public ::testing::Test
{
protected:
    void SetUp()
    {
        stocksDatabaseMock = new StrictMock<StocksDatabaseMock>();
        stocksStorageMock  = new StrictMock<StocksStorageMock>();

        thread = new CleanupThread(stocksDatabaseMock, stocksStorageMock);
    }

    void TearDown()
    {
        delete thread;
        delete stocksDatabaseMock;
        delete stocksStorageMock;
    }

    CleanupThread                  *thread;
    StrictMock<StocksDatabaseMock> *stocksDatabaseMock;
    StrictMock<StocksStorageMock>  *stocksStorageMock;
};



TEST_F(Test_CleanupThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_CleanupThread, Test_process)
{
    QList<Stock> stocks;

    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(Return(&stocks));
    EXPECT_CALL(*stocksDatabaseMock, readStocksData(&stocks));

    thread->process();
}
