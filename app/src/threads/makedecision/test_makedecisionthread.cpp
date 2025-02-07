#include "src/threads/makedecision/makedecisionthread.h"

#include <gtest/gtest.h>

#include "src/config/iconfig_mock.h"
#include "src/db/stocks/istocksdatabase_mock.h"
#include "src/storage/stocks/istocksstorage_mock.h"



using ::testing::StrictMock;
using ::testing::NotNull;
using ::testing::Return;



class Test_MakeDecisionThread : public ::testing::Test
{
protected:
    void SetUp()
    {
        configMock         = new StrictMock<ConfigMock>();
        stocksDatabaseMock = new StrictMock<StocksDatabaseMock>();
        stocksStorageMock  = new StrictMock<StocksStorageMock>();

        thread = new MakeDecisionThread(configMock, stocksDatabaseMock, stocksStorageMock);
    }

    void TearDown()
    {
        delete thread;
        delete configMock;
        delete stocksDatabaseMock;
        delete stocksStorageMock;
    }

    MakeDecisionThread                  *thread;
    StrictMock<ConfigMock>         *configMock;
    StrictMock<StocksDatabaseMock> *stocksDatabaseMock;
    StrictMock<StocksStorageMock>  *stocksStorageMock;
};



TEST_F(Test_MakeDecisionThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_MakeDecisionThread, Test_run)
{
    thread->run();
}
