#include "src/storage/stocks/stocksstorage.h"

#include <gtest/gtest.h>

#include "src/db/stocks/istocksdatabase_mock.h"
#include "src/storage/user/iuserstorage_mock.h"



using ::testing::_;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_StocksStorage : public ::testing::Test
{
protected:
    void SetUp()
    {
        stocksDatabaseMock = new StrictMock<StocksDatabaseMock>();
        userStorageMock    = new StrictMock<UserStorageMock>();

        storage = new StocksStorage(stocksDatabaseMock, userStorageMock);
    }

    void TearDown()
    {
        delete storage;
        delete stocksDatabaseMock;
        delete userStorageMock;
    }

    StocksStorage*                  storage;
    StrictMock<StocksDatabaseMock>* stocksDatabaseMock;
    StrictMock<UserStorageMock>*    userStorageMock;
};



TEST_F(Test_StocksStorage, Test_constructor_and_destructor)
{
}

TEST_F(Test_StocksStorage, Test_readFromDatabase_and_getStocks)
{
    QList<Stock*>& stocks = storage->getStocks();
    ASSERT_EQ(stocks.size(), 0);

    QList<Stock*> stocksDB;

    Stock* stock1 = new Stock(); // StocksStorage will take ownership
    Stock* stock2 = new Stock(); // StocksStorage will take ownership
    Stock* stock3 = new Stock(); // StocksStorage will take ownership

    StockData stockData1;
    StockData stockData2;
    StockData stockData3;
    StockData stockData4;

    stock1->meta.uid                     = "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa";
    stock1->meta.ticker                  = "TEST";
    stock1->meta.name                    = "abc";
    stock1->meta.forQualInvestorFlag     = true;
    stock1->meta.lot                     = 1;
    stock1->meta.minPriceIncrement.units = 0;
    stock1->meta.minPriceIncrement.nano  = 100000000;
    stock2->meta.uid                     = "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb";
    stock2->meta.ticker                  = "MAGA";
    stock2->meta.name                    = "def";
    stock2->meta.forQualInvestorFlag     = false;
    stock2->meta.lot                     = 10;
    stock2->meta.minPriceIncrement.units = 0;
    stock2->meta.minPriceIncrement.nano  = 500000000;
    stock3->meta.uid                     = "cccccccc-cccc-cccc-cccc-cccccccccccc";
    stock3->meta.ticker                  = "HNYA";
    stock3->meta.name                    = "aaaa";
    stock3->meta.forQualInvestorFlag     = true;
    stock3->meta.lot                     = 100;
    stock3->meta.minPriceIncrement.units = 1;
    stock3->meta.minPriceIncrement.nano  = 500000000;

    stockData1.timestamp = 100;
    stockData2.timestamp = 200;
    stockData3.timestamp = 300;
    stockData4.timestamp = 400;

    stockData1.price = 0.1f;
    stockData2.price = 0.2f;
    stockData3.price = 0.3f;
    stockData4.price = 0.4f;

    stock1->data << stockData1 << stockData2;
    stock2->data << stockData2 << stockData3;
    stock3->data << stockData1 << stockData3 << stockData4;

    stock1->operational.lastStoredTimestamp = stock1->data.last().timestamp;
    stock2->operational.lastStoredTimestamp = stock2->data.last().timestamp;
    stock3->operational.lastStoredTimestamp = stock3->data.last().timestamp;

    stocksDB << stock1 << stock2 << stock3;

    EXPECT_CALL(*stocksDatabaseMock, readStocksMeta()).WillOnce(Return(stocksDB));
    EXPECT_CALL(*stocksDatabaseMock, readStocksData(_));

    storage->readFromDatabase();
    stocks = storage->getStocks();

    // clang-format off
    ASSERT_EQ(stocks.size(),                                 3);
    ASSERT_EQ(stocks.at(0)->meta.uid,                        "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.ticker,                     "TEST");
    ASSERT_EQ(stocks.at(0)->meta.name,                       "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,        true);
    ASSERT_EQ(stocks.at(0)->meta.lot,                        1);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.units,    0);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.nano,     100000000);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp, 200);
    ASSERT_EQ(stocks.at(0)->data.size(),                     2);
    ASSERT_EQ(stocks.at(0)->data.at(0).timestamp,            100);
    ASSERT_NEAR(stocks.at(0)->data.at(0).price,              0.1f, 0.001f);
    ASSERT_EQ(stocks.at(0)->data.at(1).timestamp,            200);
    ASSERT_NEAR(stocks.at(0)->data.at(1).price,              0.2f, 0.001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.uid,                        "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.ticker,                     "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.name,                       "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,        false);
    ASSERT_EQ(stocks.at(1)->meta.lot,                        10);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.units,    0);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.nano,     500000000);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp, 300);
    ASSERT_EQ(stocks.at(1)->data.size(),                     2);
    ASSERT_EQ(stocks.at(1)->data.at(0).timestamp,            200);
    ASSERT_NEAR(stocks.at(1)->data.at(0).price,              0.2f, 0.001f);
    ASSERT_EQ(stocks.at(1)->data.at(1).timestamp,            300);
    ASSERT_NEAR(stocks.at(1)->data.at(1).price,              0.3f, 0.001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.uid,                        "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.ticker,                     "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.name,                       "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,        true);
    ASSERT_EQ(stocks.at(2)->meta.lot,                        100);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.units,    1);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.nano,     500000000);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp, 400);
    ASSERT_EQ(stocks.at(2)->data.size(),                     3);
    ASSERT_EQ(stocks.at(2)->data.at(0).timestamp,            100);
    ASSERT_NEAR(stocks.at(2)->data.at(0).price,              0.1f, 0.001f);
    ASSERT_EQ(stocks.at(2)->data.at(1).timestamp,            300);
    ASSERT_NEAR(stocks.at(2)->data.at(1).price,              0.3f, 0.001f);
    ASSERT_EQ(stocks.at(2)->data.at(2).timestamp,            400);
    ASSERT_NEAR(stocks.at(2)->data.at(2).price,              0.4f, 0.001f);
    // clang-format on
}

TEST_F(Test_StocksStorage, Test_getMutex)
{
    ASSERT_TRUE(storage->getMutex() != nullptr);
}
