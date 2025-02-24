#include "src/storage/stocks/stocksstorage.h"

#include <gtest/gtest.h>

#include "src/db/stocks/istocksdatabase_mock.h"



using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_StocksStorage : public ::testing::Test
{
protected:
    void SetUp()
    {
        stocksDatabaseMock = new StrictMock<StocksDatabaseMock>();

        storage = new StocksStorage(stocksDatabaseMock);
    }

    void TearDown()
    {
        delete storage;
        delete stocksDatabaseMock;
    }

    StocksStorage*                  storage;
    StrictMock<StocksDatabaseMock>* stocksDatabaseMock;
};



TEST_F(Test_StocksStorage, Test_constructor_and_destructor)
{
}

TEST_F(Test_StocksStorage, Test_readFromDatabase_and_getStocks)
{
    QList<Stock>* stocks = storage->getStocks();
    ASSERT_EQ(stocks->size(), 0);

    QList<Stock> stocksDB;

    Stock stock1;
    Stock stock2;
    Stock stock3;

    StockData stockData1;
    StockData stockData2;
    StockData stockData3;
    StockData stockData4;

    stock1.meta.ticker = "TEST";
    stock2.meta.ticker = "MAGA";
    stock3.meta.ticker = "HNYA";

    stock1.meta.name = "abc";
    stock2.meta.name = "def";
    stock3.meta.name = "aaaa";

    stockData1.timestamp = 100;
    stockData2.timestamp = 200;
    stockData3.timestamp = 300;
    stockData4.timestamp = 400;

    stockData1.value = 0.1f;
    stockData2.value = 0.2f;
    stockData3.value = 0.3f;
    stockData4.value = 0.4f;

    stock1.data << stockData1 << stockData2;
    stock2.data << stockData2 << stockData3;
    stock3.data << stockData1 << stockData3 << stockData4;
    stocksDB << stock1 << stock2 << stock3;

    EXPECT_CALL(*stocksDatabaseMock, readStocksMeta()).WillOnce(Return(stocksDB));
    EXPECT_CALL(*stocksDatabaseMock, readStocksData(NotNull()));

    storage->readFromDatabase();
    stocks = storage->getStocks();

    // clang-format off
    ASSERT_EQ(stocks->size(),                     3);
    ASSERT_EQ(stocks->at(0).meta.ticker,               "TEST");
    ASSERT_EQ(stocks->at(0).meta.name,                 "abc");
    ASSERT_EQ(stocks->at(0).data.size(),          2);
    ASSERT_EQ(stocks->at(0).data.at(0).timestamp, 100);
    ASSERT_NEAR(stocks->at(0).data.at(0).value,   0.1f, 0.001f);
    ASSERT_EQ(stocks->at(0).data.at(1).timestamp, 200);
    ASSERT_NEAR(stocks->at(0).data.at(1).value,   0.2f, 0.001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks->at(1).meta.ticker,               "MAGA");
    ASSERT_EQ(stocks->at(1).meta.name,                 "def");
    ASSERT_EQ(stocks->at(1).data.size(),          2);
    ASSERT_EQ(stocks->at(1).data.at(0).timestamp, 200);
    ASSERT_NEAR(stocks->at(1).data.at(0).value,   0.2f, 0.001f);
    ASSERT_EQ(stocks->at(1).data.at(1).timestamp, 300);
    ASSERT_NEAR(stocks->at(1).data.at(1).value,   0.3f, 0.001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks->at(2).meta.ticker,               "HNYA");
    ASSERT_EQ(stocks->at(2).meta.name,                 "aaaa");
    ASSERT_EQ(stocks->at(2).data.size(),          3);
    ASSERT_EQ(stocks->at(2).data.at(0).timestamp, 100);
    ASSERT_NEAR(stocks->at(2).data.at(0).value,   0.1f, 0.001f);
    ASSERT_EQ(stocks->at(2).data.at(1).timestamp, 300);
    ASSERT_NEAR(stocks->at(2).data.at(1).value,   0.3f, 0.001f);
    ASSERT_EQ(stocks->at(2).data.at(2).timestamp, 400);
    ASSERT_NEAR(stocks->at(2).data.at(2).value,   0.4f, 0.001f);
    // clang-format on
}

TEST_F(Test_StocksStorage, Test_getMutex)
{
    ASSERT_TRUE(storage->getMutex() != nullptr);
}
