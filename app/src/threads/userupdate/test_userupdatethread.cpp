#include "src/threads/userupdate/userupdatethread.h"

#include <gtest/gtest.h>

#include "src/config/iconfig_mock.h"
#include "src/db/stocks/istocksdatabase_mock.h"
#include "src/storage/stocks/istocksstorage_mock.h"



using ::testing::Gt;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_UserUpdateThread : public ::testing::Test
{
protected:
    void SetUp()
    {
        configMock         = new StrictMock<ConfigMock>();
        stocksDatabaseMock = new StrictMock<StocksDatabaseMock>();
        stocksStorageMock  = new StrictMock<StocksStorageMock>();

        thread = new UserUpdateThread(configMock, stocksDatabaseMock, stocksStorageMock);
    }

    void TearDown()
    {
        delete thread;
        delete configMock;
        delete stocksDatabaseMock;
        delete stocksStorageMock;
    }

    UserUpdateThread*               thread;
    StrictMock<ConfigMock>*         configMock;
    StrictMock<StocksDatabaseMock>* stocksDatabaseMock;
    StrictMock<StocksStorageMock>*  stocksStorageMock;
};



TEST_F(Test_UserUpdateThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_UserUpdateThread, Test_run)
{
    QMutex       mutex;
    QList<Stock> stocks;

    EXPECT_CALL(*configMock, getStorageMonthLimit()).WillOnce(Return(12));
    EXPECT_CALL(*stocksStorageMock, getMutex()).WillOnce(Return(&mutex));
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(Return(&stocks));
    EXPECT_CALL(*stocksDatabaseMock, deleteObsoleteData(Gt(0), &stocks));

    thread->run();
}
