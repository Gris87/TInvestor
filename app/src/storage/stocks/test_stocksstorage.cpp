#include "src/storage/stocks/stocksstorage.h"

#include <gtest/gtest.h>

#include "src/db/stocks/istocksdatabase_mock.h"



using ::testing::StrictMock;
using ::testing::NotNull;
using ::testing::Return;



TEST(Test_StocksStorage, Test_constructor_and_destructor)
{
    StocksStorage storage;
}

TEST(Test_StocksStorage, Test_getMutex)
{
    StocksStorage storage;

    ASSERT_TRUE(storage.getMutex() != nullptr);
}

TEST(Test_StocksStorage, Test_getStocks_and_readFromDatabase)
{
    StrictMock<StocksDatabaseMock> stocksDatabaseMock;

    StocksStorage storage;

    QList<Stock> *stocks = storage.getStocks();
    ASSERT_EQ(stocks->size(), 0);

    QList<Stock> stocksDB;

    Stock stock1;
    Stock stock2;
    Stock stock3;

    StockData stockData1;
    StockData stockData2;
    StockData stockData3;
    StockData stockData4;

    stock1.name = "TEST";
    stock2.name = "MAGA";
    stock3.name = "HNYA";

    stock1.fullname = "abc";
    stock2.fullname = "def";
    stock3.fullname = "aaaa";

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

    EXPECT_CALL(stocksDatabaseMock, readStocksMeta()).WillOnce(Return(stocksDB));
    EXPECT_CALL(stocksDatabaseMock, readStocksData(NotNull()));

    storage.readFromDatabase(&stocksDatabaseMock);
    stocks = storage.getStocks();

    ASSERT_EQ(stocks->size(), 3);
    ASSERT_EQ(stocks->at(0).name,                 "TEST");
    ASSERT_EQ(stocks->at(0).fullname,             "abc");
    ASSERT_EQ(stocks->at(0).data.size(),          2);
    ASSERT_EQ(stocks->at(0).data.at(0).timestamp, 100);
    ASSERT_NEAR(stocks->at(0).data.at(0).value,   0.1f, 0.001f);
    ASSERT_EQ(stocks->at(0).data.at(1).timestamp, 200);
    ASSERT_NEAR(stocks->at(0).data.at(1).value,   0.2f, 0.001f);

    ASSERT_EQ(stocks->at(1).name,                 "MAGA");
    ASSERT_EQ(stocks->at(1).fullname,             "def");
    ASSERT_EQ(stocks->at(1).data.size(),          2);
    ASSERT_EQ(stocks->at(1).data.at(0).timestamp, 200);
    ASSERT_NEAR(stocks->at(1).data.at(0).value,   0.2f, 0.001f);
    ASSERT_EQ(stocks->at(1).data.at(1).timestamp, 300);
    ASSERT_NEAR(stocks->at(1).data.at(1).value,   0.3f, 0.001f);

    ASSERT_EQ(stocks->at(2).name,                 "HNYA");
    ASSERT_EQ(stocks->at(2).fullname,             "aaaa");
    ASSERT_EQ(stocks->at(2).data.size(),          3);
    ASSERT_EQ(stocks->at(2).data.at(0).timestamp, 100);
    ASSERT_NEAR(stocks->at(2).data.at(0).value,   0.1f, 0.001f);
    ASSERT_EQ(stocks->at(2).data.at(1).timestamp, 300);
    ASSERT_NEAR(stocks->at(2).data.at(1).value,   0.3f, 0.001f);
    ASSERT_EQ(stocks->at(2).data.at(2).timestamp, 400);
    ASSERT_NEAR(stocks->at(2).data.at(2).value,   0.4f, 0.001f);
}
