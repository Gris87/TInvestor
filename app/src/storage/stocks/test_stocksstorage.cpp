#include "src/storage/stocks/stocksstorage.h"

#include <gtest/gtest.h>

#include "src/db/stocks/istocksdatabase_mock.h"
#include "src/storage/user/iuserstorage_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Ref;
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
    InSequence seq;

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
    ASSERT_EQ(stocks.size(),                                  3);
    ASSERT_EQ(stocks.at(0)->meta.uid,                         "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.ticker,                      "TEST");
    ASSERT_EQ(stocks.at(0)->meta.name,                        "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(0)->meta.lot,                         1);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.nano,      100000000);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  200);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      2);
    ASSERT_EQ(stocks.at(0)->data.at(0).timestamp,             100);
    ASSERT_NEAR(stocks.at(0)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(1).timestamp,             200);
    ASSERT_NEAR(stocks.at(0)->data.at(1).price,               0.2f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.uid,                         "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.ticker,                      "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.name,                        "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_EQ(stocks.at(1)->meta.lot,                         10);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  300);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      2);
    ASSERT_EQ(stocks.at(1)->data.at(0).timestamp,             200);
    ASSERT_NEAR(stocks.at(1)->data.at(0).price,               0.2f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(1).timestamp,             300);
    ASSERT_NEAR(stocks.at(1)->data.at(1).price,               0.3f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.uid,                         "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.ticker,                      "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.name,                        "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(2)->meta.lot,                         100);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.units,     1);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,  400);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(2)->data.size(),                      3);
    ASSERT_EQ(stocks.at(2)->data.at(0).timestamp,             100);
    ASSERT_NEAR(stocks.at(2)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(1).timestamp,             300);
    ASSERT_NEAR(stocks.at(2)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(2).timestamp,             400);
    ASSERT_NEAR(stocks.at(2)->data.at(2).price,               0.4f, 0.0001f);
    // clang-format on
}

TEST_F(Test_StocksStorage, Test_getMutex)
{
    ASSERT_NE(storage->getMutex(), nullptr);
}

TEST_F(Test_StocksStorage, Test_mergeStocksMeta)
{
    InSequence seq;

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
    ASSERT_EQ(stocks.size(),                                  3);
    ASSERT_EQ(stocks.at(0)->meta.uid,                         "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.ticker,                      "TEST");
    ASSERT_EQ(stocks.at(0)->meta.name,                        "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(0)->meta.lot,                         1);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.nano,      100000000);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  200);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      2);
    ASSERT_EQ(stocks.at(0)->data.at(0).timestamp,             100);
    ASSERT_NEAR(stocks.at(0)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(1).timestamp,             200);
    ASSERT_NEAR(stocks.at(0)->data.at(1).price,               0.2f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.uid,                         "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.ticker,                      "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.name,                        "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_EQ(stocks.at(1)->meta.lot,                         10);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  300);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      2);
    ASSERT_EQ(stocks.at(1)->data.at(0).timestamp,             200);
    ASSERT_NEAR(stocks.at(1)->data.at(0).price,               0.2f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(1).timestamp,             300);
    ASSERT_NEAR(stocks.at(1)->data.at(1).price,               0.3f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.uid,                         "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.ticker,                      "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.name,                        "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(2)->meta.lot,                         100);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.units,     1);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,  400);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(2)->data.size(),                      3);
    ASSERT_EQ(stocks.at(2)->data.at(0).timestamp,             100);
    ASSERT_NEAR(stocks.at(2)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(1).timestamp,             300);
    ASSERT_NEAR(stocks.at(2)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(2).timestamp,             400);
    ASSERT_NEAR(stocks.at(2)->data.at(2).price,               0.4f, 0.0001f);
    // clang-format on

    QList<StockMeta> stocksMeta;

    StockMeta stockMeta1;
    StockMeta stockMeta2;

    stockMeta1.uid                     = "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa";
    stockMeta1.ticker                  = "BEST";
    stockMeta1.name                    = "cba";
    stockMeta1.forQualInvestorFlag     = false;
    stockMeta1.lot                     = 7;
    stockMeta1.minPriceIncrement.units = 2;
    stockMeta1.minPriceIncrement.nano  = 300000000;

    stockMeta2.uid                     = "dddddddd-dddd-dddd-dddd-dddddddddddd";
    stockMeta2.ticker                  = "ZARA";
    stockMeta2.name                    = "Zaradunger";
    stockMeta2.forQualInvestorFlag     = true;
    stockMeta2.lot                     = 13;
    stockMeta2.minPriceIncrement.units = 5;
    stockMeta2.minPriceIncrement.nano  = 900000000;

    stocksMeta << stockMeta1 << stockMeta2;

    EXPECT_CALL(*stocksDatabaseMock, writeStocksMeta(_));

    ASSERT_EQ(storage->mergeStocksMeta(stocksMeta), true);
    ASSERT_EQ(storage->mergeStocksMeta(stocksMeta), false);

    stocks = storage->getStocks();

    // clang-format off
    ASSERT_EQ(stocks.size(),                                  4);
    ASSERT_EQ(stocks.at(0)->meta.uid,                         "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.ticker,                      "BEST");
    ASSERT_EQ(stocks.at(0)->meta.name,                        "cba");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         false);
    ASSERT_EQ(stocks.at(0)->meta.lot,                         7);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.units,     2);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.nano,      300000000);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  200);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      2);
    ASSERT_EQ(stocks.at(0)->data.at(0).timestamp,             100);
    ASSERT_NEAR(stocks.at(0)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(1).timestamp,             200);
    ASSERT_NEAR(stocks.at(0)->data.at(1).price,               0.2f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.uid,                         "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.ticker,                      "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.name,                        "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_EQ(stocks.at(1)->meta.lot,                         10);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  300);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      2);
    ASSERT_EQ(stocks.at(1)->data.at(0).timestamp,             200);
    ASSERT_NEAR(stocks.at(1)->data.at(0).price,               0.2f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(1).timestamp,             300);
    ASSERT_NEAR(stocks.at(1)->data.at(1).price,               0.3f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.uid,                         "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.ticker,                      "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.name,                        "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(2)->meta.lot,                         100);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.units,     1);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,  400);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(2)->data.size(),                      3);
    ASSERT_EQ(stocks.at(2)->data.at(0).timestamp,             100);
    ASSERT_NEAR(stocks.at(2)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(1).timestamp,             300);
    ASSERT_NEAR(stocks.at(2)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(2).timestamp,             400);
    ASSERT_NEAR(stocks.at(2)->data.at(2).price,               0.4f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(3)->meta.uid,                         "dddddddd-dddd-dddd-dddd-dddddddddddd");
    ASSERT_EQ(stocks.at(3)->meta.ticker,                      "ZARA");
    ASSERT_EQ(stocks.at(3)->meta.name,                        "Zaradunger");
    ASSERT_EQ(stocks.at(3)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(3)->meta.lot,                         13);
    ASSERT_EQ(stocks.at(3)->meta.minPriceIncrement.units,     5);
    ASSERT_EQ(stocks.at(3)->meta.minPriceIncrement.nano,      900000000);
    ASSERT_EQ(stocks.at(3)->operational.lastStoredTimestamp,  0);
    ASSERT_NEAR(stocks.at(3)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(3)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(3)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(3)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(3)->data.size(),                      0);
    // clang-format on
}

TEST_F(Test_StocksStorage, Test_appendStockData)
{
    InSequence seq;

    Stock     stock;
    StockData stockData;

    EXPECT_CALL(*stocksDatabaseMock, appendStockData(&stock, &stockData, 1));

    storage->appendStockData(&stock, &stockData, 1);
}

TEST_F(Test_StocksStorage, Test_deleteObsoleteData)
{
    InSequence seq;

    QList<Stock*>& stocks = storage->getStocks();
    ASSERT_EQ(stocks.size(), 0);

    QList<Stock*> stocksDB;

    Stock* stock1 = new Stock(); // StocksStorage will take ownership
    Stock* stock2 = new Stock(); // StocksStorage will take ownership
    Stock* stock3 = new Stock(); // StocksStorage will take ownership

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

    QVariantList timestamps[3];
    QVariantList prices[3];

    timestamps[0] << 100 << 200 << 300;
    prices[0] << 20 << 1000 << 500;

    timestamps[1] << 150 << 250 << 350 << 450 << 550 << 650;
    prices[1] << 1000.0f << 999.85f << 1000.35f << 875.95f << 1550.75f << 650.15f;

    timestamps[2] << 120 << 220 << 320 << 420 << 520;
    prices[2] << 300 << 130 << 450 << 600 << 100;

    stocksDB << stock1 << stock2 << stock3;

    for (int i = 0; i < 3; ++i)
    {
        Stock* stock    = stocksDB.at(i);
        int    dataSize = timestamps[i].size();

        for (int j = 0; j < dataSize; ++j)
        {
            StockData stockData;

            stockData.timestamp = timestamps[i][j].toLongLong();
            stockData.price     = prices[i][j].toFloat();

            stock->data.append(stockData);
        }
    }

    stock1->operational.lastStoredTimestamp = stock1->data.last().timestamp;
    stock2->operational.lastStoredTimestamp = stock2->data.last().timestamp;
    stock3->operational.lastStoredTimestamp = stock3->data.last().timestamp;

    EXPECT_CALL(*stocksDatabaseMock, readStocksMeta()).WillOnce(Return(stocksDB));
    EXPECT_CALL(*stocksDatabaseMock, readStocksData(_));

    storage->readFromDatabase();
    stocks = storage->getStocks();

    // clang-format off
    ASSERT_EQ(stocks.size(),                                  3);
    ASSERT_EQ(stocks.at(0)->meta.uid,                         "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.ticker,                      "TEST");
    ASSERT_EQ(stocks.at(0)->meta.name,                        "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(0)->meta.lot,                         1);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.nano,      100000000);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  300);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      3);
    ASSERT_EQ(stocks.at(0)->data.at(0).timestamp,             100);
    ASSERT_NEAR(stocks.at(0)->data.at(0).price,               20, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(1).timestamp,             200);
    ASSERT_NEAR(stocks.at(0)->data.at(1).price,               1000, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(2).timestamp,             300);
    ASSERT_NEAR(stocks.at(0)->data.at(2).price,               500, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.uid,                         "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.ticker,                      "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.name,                        "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_EQ(stocks.at(1)->meta.lot,                         10);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  650);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      6);
    ASSERT_EQ(stocks.at(1)->data.at(0).timestamp,             150);
    ASSERT_NEAR(stocks.at(1)->data.at(0).price,               1000.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(1).timestamp,             250);
    ASSERT_NEAR(stocks.at(1)->data.at(1).price,               999.85f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(2).timestamp,             350);
    ASSERT_NEAR(stocks.at(1)->data.at(2).price,               1000.35f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(3).timestamp,             450);
    ASSERT_NEAR(stocks.at(1)->data.at(3).price,               875.95f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(4).timestamp,             550);
    ASSERT_NEAR(stocks.at(1)->data.at(4).price,               1550.75f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(5).timestamp,             650);
    ASSERT_NEAR(stocks.at(1)->data.at(5).price,               650.15f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.uid,                         "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.ticker,                      "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.name,                        "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(2)->meta.lot,                         100);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.units,     1);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,  520);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(2)->data.size(),                      5);
    ASSERT_EQ(stocks.at(2)->data.at(0).timestamp,             120);
    ASSERT_NEAR(stocks.at(2)->data.at(0).price,               300, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(1).timestamp,             220);
    ASSERT_NEAR(stocks.at(2)->data.at(1).price,               130, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(2).timestamp,             320);
    ASSERT_NEAR(stocks.at(2)->data.at(2).price,               450, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(3).timestamp,             420);
    ASSERT_NEAR(stocks.at(2)->data.at(3).price,               600, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(4).timestamp,             520);
    ASSERT_NEAR(stocks.at(2)->data.at(4).price,               100, 0.0001f);
    // clang-format on

    EXPECT_CALL(*stocksDatabaseMock, writeStockData(_)).Times(3);

    storage->deleteObsoleteData(200);

    // clang-format off
    ASSERT_EQ(stocks.size(),                                  3);
    ASSERT_EQ(stocks.at(0)->meta.uid,                         "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.ticker,                      "TEST");
    ASSERT_EQ(stocks.at(0)->meta.name,                        "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(0)->meta.lot,                         1);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.nano,      100000000);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  300);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      2);
    ASSERT_EQ(stocks.at(0)->data.at(0).timestamp,             200);
    ASSERT_NEAR(stocks.at(0)->data.at(0).price,               1000, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(1).timestamp,             300);
    ASSERT_NEAR(stocks.at(0)->data.at(1).price,               500, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.uid,                         "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.ticker,                      "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.name,                        "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_EQ(stocks.at(1)->meta.lot,                         10);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  650);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      5);
    ASSERT_EQ(stocks.at(1)->data.at(0).timestamp,             250);
    ASSERT_NEAR(stocks.at(1)->data.at(0).price,               999.85f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(1).timestamp,             350);
    ASSERT_NEAR(stocks.at(1)->data.at(1).price,               1000.35f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(2).timestamp,             450);
    ASSERT_NEAR(stocks.at(1)->data.at(2).price,               875.95f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(3).timestamp,             550);
    ASSERT_NEAR(stocks.at(1)->data.at(3).price,               1550.75f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(4).timestamp,             650);
    ASSERT_NEAR(stocks.at(1)->data.at(4).price,               650.15f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.uid,                         "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.ticker,                      "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.name,                        "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(2)->meta.lot,                         100);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.units,     1);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,  520);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(2)->data.size(),                      4);
    ASSERT_EQ(stocks.at(2)->data.at(0).timestamp,             220);
    ASSERT_NEAR(stocks.at(2)->data.at(0).price,               130, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(1).timestamp,             320);
    ASSERT_NEAR(stocks.at(2)->data.at(1).price,               450, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(2).timestamp,             420);
    ASSERT_NEAR(stocks.at(2)->data.at(2).price,               600, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(3).timestamp,             520);
    ASSERT_NEAR(stocks.at(2)->data.at(3).price,               100, 0.0001f);
    // clang-format on

    EXPECT_CALL(*stocksDatabaseMock, writeStockData(_)).Times(3);

    storage->deleteObsoleteData(400);

    // clang-format off
    ASSERT_EQ(stocks.size(),                                  3);
    ASSERT_EQ(stocks.at(0)->meta.uid,                         "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.ticker,                      "TEST");
    ASSERT_EQ(stocks.at(0)->meta.name,                        "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(0)->meta.lot,                         1);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.nano,      100000000);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  300);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      0);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.uid,                         "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.ticker,                      "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.name,                        "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_EQ(stocks.at(1)->meta.lot,                         10);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  650);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      3);
    ASSERT_EQ(stocks.at(1)->data.at(0).timestamp,             450);
    ASSERT_NEAR(stocks.at(1)->data.at(0).price,               875.95f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(1).timestamp,             550);
    ASSERT_NEAR(stocks.at(1)->data.at(1).price,               1550.75f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(2).timestamp,             650);
    ASSERT_NEAR(stocks.at(1)->data.at(2).price,               650.15f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.uid,                         "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.ticker,                      "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.name,                        "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(2)->meta.lot,                         100);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.units,     1);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,  520);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(2)->data.size(),                      2);
    ASSERT_EQ(stocks.at(2)->data.at(0).timestamp,             420);
    ASSERT_NEAR(stocks.at(2)->data.at(0).price,               600, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(1).timestamp,             520);
    ASSERT_NEAR(stocks.at(2)->data.at(1).price,               100, 0.0001f);
    // clang-format on
}

TEST_F(Test_StocksStorage, Test_cleanupOperationalData)
{
    InSequence seq;

    QList<Stock*>& stocks = storage->getStocks();
    ASSERT_EQ(stocks.size(), 0);

    QList<Stock*> stocksDB;

    Stock* stock1 = new Stock(); // StocksStorage will take ownership
    Stock* stock2 = new Stock(); // StocksStorage will take ownership
    Stock* stock3 = new Stock(); // StocksStorage will take ownership

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

    QVariantList timestamps[3];
    QVariantList prices[3];

    timestamps[0] << 100 << 200 << 300;
    prices[0] << 20 << 1000 << 500;

    timestamps[1] << 150 << 250 << 350 << 450 << 550 << 650;
    prices[1] << 1000.0f << 999.85f << 1000.35f << 875.95f << 1550.75f << 650.15f;

    timestamps[2] << 120 << 220 << 320 << 420 << 520;
    prices[2] << 300 << 130 << 450 << 600 << 100;

    stocksDB << stock1 << stock2 << stock3;

    for (int i = 0; i < 3; ++i)
    {
        Stock* stock    = stocksDB.at(i);
        int    dataSize = timestamps[i].size();

        for (int j = 0; j < dataSize; ++j)
        {
            StockData stockData;

            stockData.timestamp = timestamps[i][j].toLongLong();
            stockData.price     = prices[i][j].toFloat();

            stock->operational.detailedData.append(stockData);
        }
    }

    EXPECT_CALL(*stocksDatabaseMock, readStocksMeta()).WillOnce(Return(stocksDB));
    EXPECT_CALL(*stocksDatabaseMock, readStocksData(_));

    storage->readFromDatabase();
    stocks = storage->getStocks();

    // clang-format off
    ASSERT_EQ(stocks.size(),                                          3);
    ASSERT_EQ(stocks.at(0)->meta.uid,                                 "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.ticker,                              "TEST");
    ASSERT_EQ(stocks.at(0)->meta.name,                                "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,                 true);
    ASSERT_EQ(stocks.at(0)->meta.lot,                                 1);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.units,             0);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.nano,              100000000);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,          0);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,              -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice,         -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.payback,                    0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),          3);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.at(0).timestamp, 100);
    ASSERT_NEAR(stocks.at(0)->operational.detailedData.at(0).price,   20, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.at(1).timestamp, 200);
    ASSERT_NEAR(stocks.at(0)->operational.detailedData.at(1).price,   1000, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.at(2).timestamp, 300);
    ASSERT_NEAR(stocks.at(0)->operational.detailedData.at(2).price,   500, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.size(),                              0);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.uid,                                 "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.ticker,                              "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.name,                                "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,                 false);
    ASSERT_EQ(stocks.at(1)->meta.lot,                                 10);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.units,             0);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.nano,              500000000);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,          0);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,              -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice,         -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.payback,                    0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),          6);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.at(0).timestamp, 150);
    ASSERT_NEAR(stocks.at(1)->operational.detailedData.at(0).price,   1000.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.at(1).timestamp, 250);
    ASSERT_NEAR(stocks.at(1)->operational.detailedData.at(1).price,   999.85f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.at(2).timestamp, 350);
    ASSERT_NEAR(stocks.at(1)->operational.detailedData.at(2).price,   1000.35f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.at(3).timestamp, 450);
    ASSERT_NEAR(stocks.at(1)->operational.detailedData.at(3).price,   875.95f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.at(4).timestamp, 550);
    ASSERT_NEAR(stocks.at(1)->operational.detailedData.at(4).price,   1550.75f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.at(5).timestamp, 650);
    ASSERT_NEAR(stocks.at(1)->operational.detailedData.at(5).price,   650.15f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.size(),                              0);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.uid,                                 "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.ticker,                              "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.name,                                "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,                 true);
    ASSERT_EQ(stocks.at(2)->meta.lot,                                 100);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.units,             1);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.nano,              500000000);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,          0);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,              -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice,         -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.payback,                    0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),          5);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.at(0).timestamp, 120);
    ASSERT_NEAR(stocks.at(2)->operational.detailedData.at(0).price,   300, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.at(1).timestamp, 220);
    ASSERT_NEAR(stocks.at(2)->operational.detailedData.at(1).price,   130, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.at(2).timestamp, 320);
    ASSERT_NEAR(stocks.at(2)->operational.detailedData.at(2).price,   450, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.at(3).timestamp, 420);
    ASSERT_NEAR(stocks.at(2)->operational.detailedData.at(3).price,   600, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.at(4).timestamp, 520);
    ASSERT_NEAR(stocks.at(2)->operational.detailedData.at(4).price,   100, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.size(),                              0);
    // clang-format on

    storage->cleanupOperationalData(200);

    // clang-format off
    ASSERT_EQ(stocks.size(),                                          3);
    ASSERT_EQ(stocks.at(0)->meta.uid,                                 "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.ticker,                              "TEST");
    ASSERT_EQ(stocks.at(0)->meta.name,                                "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,                 true);
    ASSERT_EQ(stocks.at(0)->meta.lot,                                 1);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.units,             0);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.nano,              100000000);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,          0);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,              -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice,         -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.payback,                    0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),          2);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.at(0).timestamp, 200);
    ASSERT_NEAR(stocks.at(0)->operational.detailedData.at(0).price,   1000, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.at(1).timestamp, 300);
    ASSERT_NEAR(stocks.at(0)->operational.detailedData.at(1).price,   500, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.size(),                              0);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.uid,                                 "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.ticker,                              "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.name,                                "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,                 false);
    ASSERT_EQ(stocks.at(1)->meta.lot,                                 10);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.units,             0);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.nano,              500000000);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,          0);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,              -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice,         -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.payback,                    0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),          5);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.at(0).timestamp, 250);
    ASSERT_NEAR(stocks.at(1)->operational.detailedData.at(0).price,   999.85f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.at(1).timestamp, 350);
    ASSERT_NEAR(stocks.at(1)->operational.detailedData.at(1).price,   1000.35f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.at(2).timestamp, 450);
    ASSERT_NEAR(stocks.at(1)->operational.detailedData.at(2).price,   875.95f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.at(3).timestamp, 550);
    ASSERT_NEAR(stocks.at(1)->operational.detailedData.at(3).price,   1550.75f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.at(4).timestamp, 650);
    ASSERT_NEAR(stocks.at(1)->operational.detailedData.at(4).price,   650.15f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.size(),                              0);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.uid,                                 "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.ticker,                              "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.name,                                "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,                 true);
    ASSERT_EQ(stocks.at(2)->meta.lot,                                 100);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.units,             1);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.nano,              500000000);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,          0);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,              -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice,         -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.payback,                    0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),          4);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.at(0).timestamp, 220);
    ASSERT_NEAR(stocks.at(2)->operational.detailedData.at(0).price,   130, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.at(1).timestamp, 320);
    ASSERT_NEAR(stocks.at(2)->operational.detailedData.at(1).price,   450, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.at(2).timestamp, 420);
    ASSERT_NEAR(stocks.at(2)->operational.detailedData.at(2).price,   600, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.at(3).timestamp, 520);
    ASSERT_NEAR(stocks.at(2)->operational.detailedData.at(3).price,   100, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.size(),                              0);
    // clang-format on

    storage->cleanupOperationalData(400);

    // clang-format off
    ASSERT_EQ(stocks.size(),                                  3);
    ASSERT_EQ(stocks.at(0)->meta.uid,                         "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.ticker,                      "TEST");
    ASSERT_EQ(stocks.at(0)->meta.name,                        "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(0)->meta.lot,                         1);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.nano,      100000000);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  0);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      0);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.uid,                                 "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.ticker,                              "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.name,                                "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,                 false);
    ASSERT_EQ(stocks.at(1)->meta.lot,                                 10);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.units,             0);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.nano,              500000000);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,          0);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,              -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice,         -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.payback,                    0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),          3);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.at(0).timestamp, 450);
    ASSERT_NEAR(stocks.at(1)->operational.detailedData.at(0).price,   875.95f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.at(1).timestamp, 550);
    ASSERT_NEAR(stocks.at(1)->operational.detailedData.at(1).price,   1550.75f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.at(2).timestamp, 650);
    ASSERT_NEAR(stocks.at(1)->operational.detailedData.at(2).price,   650.15f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.size(),                              0);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.uid,                                 "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.ticker,                              "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.name,                                "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,                 true);
    ASSERT_EQ(stocks.at(2)->meta.lot,                                 100);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.units,             1);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.nano,              500000000);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,          0);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,              -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice,         -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.payback,                    0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),          2);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.at(0).timestamp, 420);
    ASSERT_NEAR(stocks.at(2)->operational.detailedData.at(0).price,   600, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.at(1).timestamp, 520);
    ASSERT_NEAR(stocks.at(2)->operational.detailedData.at(1).price,   100, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.size(),                              0);
    // clang-format on
}

TEST_F(Test_StocksStorage, Test_obtainStocksDayStartPrice)
{
    InSequence seq;

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
    StockData stockData5;
    StockData stockData6;
    StockData stockData7;

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
    stockData5.timestamp = 500;
    stockData6.timestamp = 600;
    stockData7.timestamp = 700;

    stockData1.price = 0.1f;
    stockData2.price = 0.2f;
    stockData3.price = 0.3f;
    stockData4.price = 0.4f;
    stockData5.price = 0.5f;
    stockData6.price = 0.6f;
    stockData7.price = 0.7f;

    stock1->data << stockData1 << stockData2 << stockData5;
    stock2->data << stockData2 << stockData3 << stockData6 << stockData7;
    stock3->data << stockData1 << stockData3 << stockData4 << stockData5;

    stock1->operational.lastStoredTimestamp = stock1->data.last().timestamp;
    stock2->operational.lastStoredTimestamp = stock2->data.last().timestamp;
    stock3->operational.lastStoredTimestamp = stock3->data.last().timestamp;

    stocksDB << stock1 << stock2 << stock3;

    EXPECT_CALL(*stocksDatabaseMock, readStocksMeta()).WillOnce(Return(stocksDB));
    EXPECT_CALL(*stocksDatabaseMock, readStocksData(_));

    storage->readFromDatabase();
    stocks = storage->getStocks();

    // clang-format off
    ASSERT_EQ(stocks.size(),                                  3);
    ASSERT_EQ(stocks.at(0)->meta.uid,                         "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.ticker,                      "TEST");
    ASSERT_EQ(stocks.at(0)->meta.name,                        "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(0)->meta.lot,                         1);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.nano,      100000000);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  500);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      3);
    ASSERT_EQ(stocks.at(0)->data.at(0).timestamp,             100);
    ASSERT_NEAR(stocks.at(0)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(1).timestamp,             200);
    ASSERT_NEAR(stocks.at(0)->data.at(1).price,               0.2f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(2).timestamp,             500);
    ASSERT_NEAR(stocks.at(0)->data.at(2).price,               0.5f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.uid,                         "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.ticker,                      "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.name,                        "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_EQ(stocks.at(1)->meta.lot,                         10);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  700);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      4);
    ASSERT_EQ(stocks.at(1)->data.at(0).timestamp,             200);
    ASSERT_NEAR(stocks.at(1)->data.at(0).price,               0.2f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(1).timestamp,             300);
    ASSERT_NEAR(stocks.at(1)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(2).timestamp,             600);
    ASSERT_NEAR(stocks.at(1)->data.at(2).price,               0.6f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(3).timestamp,             700);
    ASSERT_NEAR(stocks.at(1)->data.at(3).price,               0.7f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.uid,                         "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.ticker,                      "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.name,                        "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(2)->meta.lot,                         100);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.units,     1);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,  500);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(2)->data.size(),                      4);
    ASSERT_EQ(stocks.at(2)->data.at(0).timestamp,             100);
    ASSERT_NEAR(stocks.at(2)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(1).timestamp,             300);
    ASSERT_NEAR(stocks.at(2)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(2).timestamp,             400);
    ASSERT_NEAR(stocks.at(2)->data.at(2).price,               0.4f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(3).timestamp,             500);
    ASSERT_NEAR(stocks.at(2)->data.at(3).price,               0.5f, 0.0001f);
    // clang-format on

    storage->obtainStocksDayStartPrice(400);

    // clang-format off
    ASSERT_EQ(stocks.size(),                                  3);
    ASSERT_EQ(stocks.at(0)->meta.uid,                         "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.ticker,                      "TEST");
    ASSERT_EQ(stocks.at(0)->meta.name,                        "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(0)->meta.lot,                         1);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.nano,      100000000);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  500);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      0.2f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      3);
    ASSERT_EQ(stocks.at(0)->data.at(0).timestamp,             100);
    ASSERT_NEAR(stocks.at(0)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(1).timestamp,             200);
    ASSERT_NEAR(stocks.at(0)->data.at(1).price,               0.2f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(2).timestamp,             500);
    ASSERT_NEAR(stocks.at(0)->data.at(2).price,               0.5f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.uid,                         "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.ticker,                      "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.name,                        "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_EQ(stocks.at(1)->meta.lot,                         10);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  700);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      0.3f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      4);
    ASSERT_EQ(stocks.at(1)->data.at(0).timestamp,             200);
    ASSERT_NEAR(stocks.at(1)->data.at(0).price,               0.2f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(1).timestamp,             300);
    ASSERT_NEAR(stocks.at(1)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(2).timestamp,             600);
    ASSERT_NEAR(stocks.at(1)->data.at(2).price,               0.6f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(3).timestamp,             700);
    ASSERT_NEAR(stocks.at(1)->data.at(3).price,               0.7f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.uid,                         "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.ticker,                      "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.name,                        "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(2)->meta.lot,                         100);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.units,     1);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,  500);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,      0.4f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(2)->data.size(),                      4);
    ASSERT_EQ(stocks.at(2)->data.at(0).timestamp,             100);
    ASSERT_NEAR(stocks.at(2)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(1).timestamp,             300);
    ASSERT_NEAR(stocks.at(2)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(2).timestamp,             400);
    ASSERT_NEAR(stocks.at(2)->data.at(2).price,               0.4f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(3).timestamp,             500);
    ASSERT_NEAR(stocks.at(2)->data.at(3).price,               0.5f, 0.0001f);
    // clang-format on
}

TEST_F(Test_StocksStorage, Test_obtainStocksDatePrice)
{
    InSequence seq;

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
    StockData stockData5;
    StockData stockData6;
    StockData stockData7;

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
    stockData5.timestamp = 500;
    stockData6.timestamp = 600;
    stockData7.timestamp = 700;

    stockData1.price = 0.1f;
    stockData2.price = 0.2f;
    stockData3.price = 0.3f;
    stockData4.price = 0.4f;
    stockData5.price = 0.5f;
    stockData6.price = 0.6f;
    stockData7.price = 0.7f;

    stock1->data << stockData1 << stockData2 << stockData5;
    stock2->data << stockData2 << stockData3 << stockData6 << stockData7;
    stock3->data << stockData1 << stockData3 << stockData4 << stockData5;

    stock1->operational.lastStoredTimestamp = stock1->data.last().timestamp;
    stock2->operational.lastStoredTimestamp = stock2->data.last().timestamp;
    stock3->operational.lastStoredTimestamp = stock3->data.last().timestamp;

    stocksDB << stock1 << stock2 << stock3;

    EXPECT_CALL(*stocksDatabaseMock, readStocksMeta()).WillOnce(Return(stocksDB));
    EXPECT_CALL(*stocksDatabaseMock, readStocksData(_));

    storage->readFromDatabase();
    stocks = storage->getStocks();

    // clang-format off
    ASSERT_EQ(stocks.size(),                                  3);
    ASSERT_EQ(stocks.at(0)->meta.uid,                         "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.ticker,                      "TEST");
    ASSERT_EQ(stocks.at(0)->meta.name,                        "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(0)->meta.lot,                         1);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.nano,      100000000);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  500);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      3);
    ASSERT_EQ(stocks.at(0)->data.at(0).timestamp,             100);
    ASSERT_NEAR(stocks.at(0)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(1).timestamp,             200);
    ASSERT_NEAR(stocks.at(0)->data.at(1).price,               0.2f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(2).timestamp,             500);
    ASSERT_NEAR(stocks.at(0)->data.at(2).price,               0.5f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.uid,                         "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.ticker,                      "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.name,                        "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_EQ(stocks.at(1)->meta.lot,                         10);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  700);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      4);
    ASSERT_EQ(stocks.at(1)->data.at(0).timestamp,             200);
    ASSERT_NEAR(stocks.at(1)->data.at(0).price,               0.2f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(1).timestamp,             300);
    ASSERT_NEAR(stocks.at(1)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(2).timestamp,             600);
    ASSERT_NEAR(stocks.at(1)->data.at(2).price,               0.6f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(3).timestamp,             700);
    ASSERT_NEAR(stocks.at(1)->data.at(3).price,               0.7f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.uid,                         "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.ticker,                      "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.name,                        "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(2)->meta.lot,                         100);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.units,     1);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,  500);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(2)->data.size(),                      4);
    ASSERT_EQ(stocks.at(2)->data.at(0).timestamp,             100);
    ASSERT_NEAR(stocks.at(2)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(1).timestamp,             300);
    ASSERT_NEAR(stocks.at(2)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(2).timestamp,             400);
    ASSERT_NEAR(stocks.at(2)->data.at(2).price,               0.4f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(3).timestamp,             500);
    ASSERT_NEAR(stocks.at(2)->data.at(3).price,               0.5f, 0.0001f);
    // clang-format on

    storage->obtainStocksDatePrice(400);

    // clang-format off
    ASSERT_EQ(stocks.size(),                                  3);
    ASSERT_EQ(stocks.at(0)->meta.uid,                         "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.ticker,                      "TEST");
    ASSERT_EQ(stocks.at(0)->meta.name,                        "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(0)->meta.lot,                         1);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.nano,      100000000);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  500);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, 0.2f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      3);
    ASSERT_EQ(stocks.at(0)->data.at(0).timestamp,             100);
    ASSERT_NEAR(stocks.at(0)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(1).timestamp,             200);
    ASSERT_NEAR(stocks.at(0)->data.at(1).price,               0.2f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(2).timestamp,             500);
    ASSERT_NEAR(stocks.at(0)->data.at(2).price,               0.5f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.uid,                         "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.ticker,                      "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.name,                        "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_EQ(stocks.at(1)->meta.lot,                         10);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  700);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, 0.3f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      4);
    ASSERT_EQ(stocks.at(1)->data.at(0).timestamp,             200);
    ASSERT_NEAR(stocks.at(1)->data.at(0).price,               0.2f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(1).timestamp,             300);
    ASSERT_NEAR(stocks.at(1)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(2).timestamp,             600);
    ASSERT_NEAR(stocks.at(1)->data.at(2).price,               0.6f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(3).timestamp,             700);
    ASSERT_NEAR(stocks.at(1)->data.at(3).price,               0.7f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.uid,                         "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.ticker,                      "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.name,                        "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(2)->meta.lot,                         100);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.units,     1);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,  500);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice, 0.4f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(2)->data.size(),                      4);
    ASSERT_EQ(stocks.at(2)->data.at(0).timestamp,             100);
    ASSERT_NEAR(stocks.at(2)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(1).timestamp,             300);
    ASSERT_NEAR(stocks.at(2)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(2).timestamp,             400);
    ASSERT_NEAR(stocks.at(2)->data.at(2).price,               0.4f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(3).timestamp,             500);
    ASSERT_NEAR(stocks.at(2)->data.at(3).price,               0.5f, 0.0001f);
    // clang-format on
}

TEST_F(Test_StocksStorage, Test_obtainPayback)
{
    // InSequence seq;

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
    StockData stockData5;
    StockData stockData6;
    StockData stockData7;

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
    stockData5.timestamp = 500;
    stockData6.timestamp = 600;
    stockData7.timestamp = 700;

    stockData1.price = 0.1f;
    stockData2.price = 0.9f;
    stockData3.price = 0.3f;
    stockData4.price = 0.4f;
    stockData5.price = 0.2f;
    stockData6.price = 0.6f;
    stockData7.price = 0.1f;

    stock1->data << stockData1 << stockData2 << stockData5;
    stock2->data << stockData2 << stockData3 << stockData6 << stockData7;
    stock3->data << stockData1 << stockData3 << stockData4 << stockData5;

    stock1->operational.lastStoredTimestamp = stock1->data.last().timestamp;
    stock2->operational.lastStoredTimestamp = stock2->data.last().timestamp;
    stock3->operational.lastStoredTimestamp = stock3->data.last().timestamp;

    stocksDB << stock1 << stock2 << stock3;

    EXPECT_CALL(*stocksDatabaseMock, readStocksMeta()).WillOnce(Return(stocksDB));
    EXPECT_CALL(*stocksDatabaseMock, readStocksData(_));

    storage->readFromDatabase();
    stocks = storage->getStocks();

    // clang-format off
    ASSERT_EQ(stocks.size(),                                  3);
    ASSERT_EQ(stocks.at(0)->meta.uid,                         "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.ticker,                      "TEST");
    ASSERT_EQ(stocks.at(0)->meta.name,                        "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(0)->meta.lot,                         1);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.nano,      100000000);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  500);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      3);
    ASSERT_EQ(stocks.at(0)->data.at(0).timestamp,             100);
    ASSERT_NEAR(stocks.at(0)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(1).timestamp,             200);
    ASSERT_NEAR(stocks.at(0)->data.at(1).price,               0.9f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(2).timestamp,             500);
    ASSERT_NEAR(stocks.at(0)->data.at(2).price,               0.2f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.uid,                         "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.ticker,                      "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.name,                        "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_EQ(stocks.at(1)->meta.lot,                         10);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  700);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      4);
    ASSERT_EQ(stocks.at(1)->data.at(0).timestamp,             200);
    ASSERT_NEAR(stocks.at(1)->data.at(0).price,               0.9f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(1).timestamp,             300);
    ASSERT_NEAR(stocks.at(1)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(2).timestamp,             600);
    ASSERT_NEAR(stocks.at(1)->data.at(2).price,               0.6f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(3).timestamp,             700);
    ASSERT_NEAR(stocks.at(1)->data.at(3).price,               0.1f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.uid,                         "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.ticker,                      "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.name,                        "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(2)->meta.lot,                         100);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.units,     1);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,  500);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(2)->data.size(),                      4);
    ASSERT_EQ(stocks.at(2)->data.at(0).timestamp,             100);
    ASSERT_NEAR(stocks.at(2)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(1).timestamp,             300);
    ASSERT_NEAR(stocks.at(2)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(2).timestamp,             400);
    ASSERT_NEAR(stocks.at(2)->data.at(2).price,               0.4f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(3).timestamp,             500);
    ASSERT_NEAR(stocks.at(2)->data.at(3).price,               0.2f, 0.0001f);
    // clang-format on

    QMutex mutex;

    EXPECT_CALL(*userStorageMock, getMutex()).WillRepeatedly(Return(&mutex));
    EXPECT_CALL(*userStorageMock, getCommission()).WillRepeatedly(Return(0.3f));

    storage->obtainPayback(200);

    // clang-format off
    ASSERT_EQ(stocks.size(),                                  3);
    ASSERT_EQ(stocks.at(0)->meta.uid,                         "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.ticker,                      "TEST");
    ASSERT_EQ(stocks.at(0)->meta.name,                        "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(0)->meta.lot,                         1);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.nano,      100000000);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  500);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      3);
    ASSERT_EQ(stocks.at(0)->data.at(0).timestamp,             100);
    ASSERT_NEAR(stocks.at(0)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(1).timestamp,             200);
    ASSERT_NEAR(stocks.at(0)->data.at(1).price,               0.9f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(2).timestamp,             500);
    ASSERT_NEAR(stocks.at(0)->data.at(2).price,               0.2f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.uid,                         "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.ticker,                      "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.name,                        "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_EQ(stocks.at(1)->meta.lot,                         10);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  700);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            33.33333f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      4);
    ASSERT_EQ(stocks.at(1)->data.at(0).timestamp,             200);
    ASSERT_NEAR(stocks.at(1)->data.at(0).price,               0.9f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(1).timestamp,             300);
    ASSERT_NEAR(stocks.at(1)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(2).timestamp,             600);
    ASSERT_NEAR(stocks.at(1)->data.at(2).price,               0.6f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(3).timestamp,             700);
    ASSERT_NEAR(stocks.at(1)->data.at(3).price,               0.1f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.uid,                         "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.ticker,                      "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.name,                        "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(2)->meta.lot,                         100);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.units,     1);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,  500);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.payback,            66.66666f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(2)->data.size(),                      4);
    ASSERT_EQ(stocks.at(2)->data.at(0).timestamp,             100);
    ASSERT_NEAR(stocks.at(2)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(1).timestamp,             300);
    ASSERT_NEAR(stocks.at(2)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(2).timestamp,             400);
    ASSERT_NEAR(stocks.at(2)->data.at(2).price,               0.4f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(3).timestamp,             500);
    ASSERT_NEAR(stocks.at(2)->data.at(3).price,               0.2f, 0.0001f);
    // clang-format on
}
