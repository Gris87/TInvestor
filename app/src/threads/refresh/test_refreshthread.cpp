#include "src/threads/refresh/refreshthread.h"

#include <gtest/gtest.h>

#include "src/db/stocks/istocksdatabase_mock.h"
#include "src/storage/stocks/istocksstorage_mock.h"



using ::testing::StrictMock;
using ::testing::NotNull;
using ::testing::Return;



class Test_RefreshThread : public ::testing::Test
{
protected:
    void SetUp()
    {
        stocksDatabaseMock = new StrictMock<StocksDatabaseMock>();
        stocksStorageMock  = new StrictMock<StocksStorageMock>();

        thread = new RefreshThread(stocksDatabaseMock, stocksStorageMock);
    }

    void TearDown()
    {
        delete thread;
        delete stocksDatabaseMock;
        delete stocksStorageMock;
    }

    RefreshThread                  *thread;
    StrictMock<StocksDatabaseMock> *stocksDatabaseMock;
    StrictMock<StocksStorageMock>  *stocksStorageMock;
};



TEST_F(Test_RefreshThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_RefreshThread, Test_process)
{
    thread->process();
}
