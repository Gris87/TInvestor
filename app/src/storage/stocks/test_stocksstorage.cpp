#include "src/storage/stocks/stocksstorage.h"

#include <gtest/gtest.h>

#include "src/db/stocks/istocksdatabase_mock.h"
#include "src/storage/user/iuserstorage_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::Ne;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity, readability-function-size, readability-magic-numbers)
class Test_StocksStorage : public ::testing::Test
{
protected:
    void SetUp() override
    {
        stocksDatabaseMock = new StrictMock<StocksDatabaseMock>();
        userStorageMock    = new StrictMock<UserStorageMock>();

        storage = new StocksStorage(stocksDatabaseMock, userStorageMock);
    }

    void TearDown() override
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

TEST_F(Test_StocksStorage, Test_readFromDatabase_and_getStocks_and_assignLogos)
{
    const InSequence seq;

    QList<Stock*> stocks = storage->getStocks();
    ASSERT_EQ(stocks.size(), 0);

    QList<Stock*> stocksDB;

    Stock* stock1 = new Stock(); // StocksStorage will take ownership
    Stock* stock2 = new Stock(); // StocksStorage will take ownership
    Stock* stock3 = new Stock(); // StocksStorage will take ownership

    StockData stockData1;
    StockData stockData2;
    StockData stockData3;
    StockData stockData4;

    stock1->meta.instrumentId        = "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa";
    stock1->meta.instrumentTicker    = "TEST";
    stock1->meta.instrumentName      = "abc";
    stock1->meta.forQualInvestorFlag = true;
    stock1->meta.minPriceIncrement   = 0.1f;
    stock1->meta.pricePrecision      = 1;
    stock2->meta.instrumentId        = "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb";
    stock2->meta.instrumentTicker    = "MAGA";
    stock2->meta.instrumentName      = "def";
    stock2->meta.forQualInvestorFlag = false;
    stock2->meta.minPriceIncrement   = 0.01f;
    stock2->meta.pricePrecision      = 2;
    stock3->meta.instrumentId        = "cccccccc-cccc-cccc-cccc-cccccccccccc";
    stock3->meta.instrumentTicker    = "HNYA";
    stock3->meta.instrumentName      = "aaaa";
    stock3->meta.forQualInvestorFlag = true;
    stock3->meta.minPriceIncrement   = 0.001f;
    stock3->meta.pricePrecision      = 3;

    stockData1.timestamp = 100;
    stockData2.timestamp = 200;
    stockData3.timestamp = 300;
    stockData4.timestamp = 400;

    stockData1.quantity = 1;
    stockData2.quantity = 2;
    stockData3.quantity = 3;
    stockData4.quantity = 4;

    stockData1.price = 0.1f;
    stockData2.price = 0.2f;
    stockData3.price = 0.3f;
    stockData4.price = 0.4f;

    stock1->data << stockData1 << stockData2;
    stock2->data << stockData2 << stockData3;
    stock3->data << stockData1 << stockData3 << stockData4;

    stock1->operational.lastStoredTimestamp = stock1->data.constLast().timestamp;
    stock2->operational.lastStoredTimestamp = stock2->data.constLast().timestamp;
    stock3->operational.lastStoredTimestamp = stock3->data.constLast().timestamp;

    stocksDB << stock1 << stock2 << stock3;

    EXPECT_CALL(*stocksDatabaseMock, readStocksMeta()).WillOnce(Return(stocksDB));
    EXPECT_CALL(*stocksDatabaseMock, readStocksData(Ne(QList<Stock*>())));

    storage->readFromDatabase();
    stocks = storage->getStocks();

    // clang-format off
    ASSERT_EQ(stocks.size(),                                  3);
    ASSERT_EQ(stocks.at(0)->meta.instrumentId,                "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(0)->meta.instrumentTicker,            "TEST");
    ASSERT_EQ(stocks.at(0)->meta.instrumentName,              "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_NEAR(stocks.at(0)->meta.minPriceIncrement,         0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->meta.pricePrecision,              1);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  200);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      2);
    ASSERT_EQ(stocks.at(0)->data.at(0).timestamp,             100);
    ASSERT_EQ(stocks.at(0)->data.at(0).quantity,              1);
    ASSERT_NEAR(stocks.at(0)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(1).timestamp,             200);
    ASSERT_EQ(stocks.at(0)->data.at(1).quantity,              2);
    ASSERT_NEAR(stocks.at(0)->data.at(1).price,               0.2f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.instrumentId,                "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(1)->meta.instrumentTicker,            "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.instrumentName,              "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_NEAR(stocks.at(1)->meta.minPriceIncrement,         0.01f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->meta.pricePrecision,              2);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  300);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      2);
    ASSERT_EQ(stocks.at(1)->data.at(0).timestamp,             200);
    ASSERT_EQ(stocks.at(1)->data.at(0).quantity,              2);
    ASSERT_NEAR(stocks.at(1)->data.at(0).price,               0.2f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(1).timestamp,             300);
    ASSERT_EQ(stocks.at(1)->data.at(1).quantity,              3);
    ASSERT_NEAR(stocks.at(1)->data.at(1).price,               0.3f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.instrumentId,                "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(2)->meta.instrumentTicker,            "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.instrumentName,              "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,         true);
    ASSERT_NEAR(stocks.at(2)->meta.minPriceIncrement,         0.001f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->meta.pricePrecision,              3);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,  400);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(2)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(2)->data.size(),                      3);
    ASSERT_EQ(stocks.at(2)->data.at(0).timestamp,             100);
    ASSERT_EQ(stocks.at(2)->data.at(0).quantity,              1);
    ASSERT_NEAR(stocks.at(2)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(1).timestamp,             300);
    ASSERT_EQ(stocks.at(2)->data.at(1).quantity,              3);
    ASSERT_NEAR(stocks.at(2)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(2).timestamp,             400);
    ASSERT_EQ(stocks.at(2)->data.at(2).quantity,              4);
    ASSERT_NEAR(stocks.at(2)->data.at(2).price,               0.4f, 0.0001f);
    // clang-format on

    EXPECT_CALL(*stocksDatabaseMock, assignLogos(Ne(QList<Stock*>())));

    storage->assignLogos();
}

TEST_F(Test_StocksStorage, Test_lock_and_unlock)
{
    storage->writeLock();
    storage->writeUnlock();
    storage->readLock();
    storage->readUnlock();
}

TEST_F(Test_StocksStorage, Test_mergeStocksMeta)
{
    const InSequence seq;

    QList<Stock*> stocks = storage->getStocks();
    ASSERT_EQ(stocks.size(), 0);

    QList<Stock*> stocksDB;

    Stock* stock1 = new Stock(); // StocksStorage will take ownership
    Stock* stock2 = new Stock(); // StocksStorage will take ownership
    Stock* stock3 = new Stock(); // StocksStorage will take ownership

    StockData stockData1;
    StockData stockData2;
    StockData stockData3;
    StockData stockData4;

    stock1->meta.instrumentId        = "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa";
    stock1->meta.instrumentTicker    = "TEST";
    stock1->meta.instrumentName      = "abc";
    stock1->meta.forQualInvestorFlag = true;
    stock1->meta.minPriceIncrement   = 0.1f;
    stock1->meta.pricePrecision      = 1;
    stock2->meta.instrumentId        = "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb";
    stock2->meta.instrumentTicker    = "MAGA";
    stock2->meta.instrumentName      = "def";
    stock2->meta.forQualInvestorFlag = false;
    stock2->meta.minPriceIncrement   = 0.01f;
    stock2->meta.pricePrecision      = 2;
    stock3->meta.instrumentId        = "cccccccc-cccc-cccc-cccc-cccccccccccc";
    stock3->meta.instrumentTicker    = "HNYA";
    stock3->meta.instrumentName      = "aaaa";
    stock3->meta.forQualInvestorFlag = true;
    stock3->meta.minPriceIncrement   = 0.001f;
    stock3->meta.pricePrecision      = 3;

    stockData1.timestamp = 100;
    stockData2.timestamp = 200;
    stockData3.timestamp = 300;
    stockData4.timestamp = 400;

    stockData1.quantity = 1;
    stockData2.quantity = 2;
    stockData3.quantity = 3;
    stockData4.quantity = 4;

    stockData1.price = 0.1f;
    stockData2.price = 0.2f;
    stockData3.price = 0.3f;
    stockData4.price = 0.4f;

    stock1->data << stockData1 << stockData2;
    stock2->data << stockData2 << stockData3;
    stock3->data << stockData1 << stockData3 << stockData4;

    stock1->operational.lastStoredTimestamp = stock1->data.constLast().timestamp;
    stock2->operational.lastStoredTimestamp = stock2->data.constLast().timestamp;
    stock3->operational.lastStoredTimestamp = stock3->data.constLast().timestamp;

    stocksDB << stock1 << stock2 << stock3;

    EXPECT_CALL(*stocksDatabaseMock, readStocksMeta()).WillOnce(Return(stocksDB));
    EXPECT_CALL(*stocksDatabaseMock, readStocksData(Ne(QList<Stock*>())));

    storage->readFromDatabase();
    stocks = storage->getStocks();

    // clang-format off
    ASSERT_EQ(stocks.size(),                                  3);
    ASSERT_EQ(stocks.at(0)->meta.instrumentId,                "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(0)->meta.instrumentTicker,            "TEST");
    ASSERT_EQ(stocks.at(0)->meta.instrumentName,              "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_NEAR(stocks.at(0)->meta.minPriceIncrement,         0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->meta.pricePrecision,              1);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  200);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      2);
    ASSERT_EQ(stocks.at(0)->data.at(0).timestamp,             100);
    ASSERT_EQ(stocks.at(0)->data.at(0).quantity,              1);
    ASSERT_NEAR(stocks.at(0)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(1).timestamp,             200);
    ASSERT_EQ(stocks.at(0)->data.at(1).quantity,              2);
    ASSERT_NEAR(stocks.at(0)->data.at(1).price,               0.2f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.instrumentId,                "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(1)->meta.instrumentTicker,            "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.instrumentName,              "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_NEAR(stocks.at(1)->meta.minPriceIncrement,         0.01f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->meta.pricePrecision,              2);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  300);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      2);
    ASSERT_EQ(stocks.at(1)->data.at(0).timestamp,             200);
    ASSERT_EQ(stocks.at(1)->data.at(0).quantity,              2);
    ASSERT_NEAR(stocks.at(1)->data.at(0).price,               0.2f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(1).timestamp,             300);
    ASSERT_EQ(stocks.at(1)->data.at(1).quantity,              3);
    ASSERT_NEAR(stocks.at(1)->data.at(1).price,               0.3f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.instrumentId,                "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(2)->meta.instrumentTicker,            "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.instrumentName,              "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,         true);
    ASSERT_NEAR(stocks.at(2)->meta.minPriceIncrement,         0.001f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->meta.pricePrecision,              3);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,  400);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(2)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(2)->data.size(),                      3);
    ASSERT_EQ(stocks.at(2)->data.at(0).timestamp,             100);
    ASSERT_EQ(stocks.at(2)->data.at(0).quantity,              1);
    ASSERT_NEAR(stocks.at(2)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(1).timestamp,             300);
    ASSERT_EQ(stocks.at(2)->data.at(1).quantity,              3);
    ASSERT_NEAR(stocks.at(2)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(2).timestamp,             400);
    ASSERT_EQ(stocks.at(2)->data.at(2).quantity,              4);
    ASSERT_NEAR(stocks.at(2)->data.at(2).price,               0.4f, 0.0001f);
    // clang-format on

    QList<StockMeta> stocksMeta;

    StockMeta stockMeta1;
    StockMeta stockMeta2;

    stockMeta1.instrumentId        = "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa";
    stockMeta1.instrumentTicker    = "BEST";
    stockMeta1.instrumentName      = "cba";
    stockMeta1.forQualInvestorFlag = false;
    stockMeta1.minPriceIncrement   = 0.01f;
    stockMeta1.pricePrecision      = 2;

    stockMeta2.instrumentId        = "dddddddd-dddd-dddd-dddd-dddddddddddd";
    stockMeta2.instrumentTicker    = "ZARA";
    stockMeta2.instrumentName      = "Zaradunger";
    stockMeta2.forQualInvestorFlag = true;
    stockMeta2.minPriceIncrement   = 0.001f;
    stockMeta2.pricePrecision      = 3;

    stocksMeta << stockMeta1 << stockMeta2;

    EXPECT_CALL(*stocksDatabaseMock, writeStocksMeta(Ne(QList<Stock*>())));

    ASSERT_EQ(storage->mergeStocksMeta(stocksMeta), true);
    ASSERT_EQ(storage->mergeStocksMeta(stocksMeta), false);

    stocks = storage->getStocks();

    // clang-format off
    ASSERT_EQ(stocks.size(),                                  4);
    ASSERT_EQ(stocks.at(0)->meta.instrumentId,                "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(0)->meta.instrumentTicker,            "BEST");
    ASSERT_EQ(stocks.at(0)->meta.instrumentName,              "cba");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         false);
    ASSERT_NEAR(stocks.at(0)->meta.minPriceIncrement,         0.01f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->meta.pricePrecision,              2);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  200);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      2);
    ASSERT_EQ(stocks.at(0)->data.at(0).timestamp,             100);
    ASSERT_EQ(stocks.at(0)->data.at(0).quantity,              1);
    ASSERT_NEAR(stocks.at(0)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(1).timestamp,             200);
    ASSERT_EQ(stocks.at(0)->data.at(1).quantity,              2);
    ASSERT_NEAR(stocks.at(0)->data.at(1).price,               0.2f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.instrumentId,                "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(1)->meta.instrumentTicker,            "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.instrumentName,              "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_NEAR(stocks.at(1)->meta.minPriceIncrement,         0.01f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->meta.pricePrecision,              2);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  300);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      2);
    ASSERT_EQ(stocks.at(1)->data.at(0).timestamp,             200);
    ASSERT_EQ(stocks.at(1)->data.at(0).quantity,              2);
    ASSERT_NEAR(stocks.at(1)->data.at(0).price,               0.2f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(1).timestamp,             300);
    ASSERT_EQ(stocks.at(1)->data.at(1).quantity,              3);
    ASSERT_NEAR(stocks.at(1)->data.at(1).price,               0.3f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.instrumentId,                "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(2)->meta.instrumentTicker,            "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.instrumentName,              "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,         true);
    ASSERT_NEAR(stocks.at(2)->meta.minPriceIncrement,         0.001f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->meta.pricePrecision,              3);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,  400);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(2)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(2)->data.size(),                      3);
    ASSERT_EQ(stocks.at(2)->data.at(0).timestamp,             100);
    ASSERT_EQ(stocks.at(2)->data.at(0).quantity,              1);
    ASSERT_NEAR(stocks.at(2)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(1).timestamp,             300);
    ASSERT_EQ(stocks.at(2)->data.at(1).quantity,              3);
    ASSERT_NEAR(stocks.at(2)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(2).timestamp,             400);
    ASSERT_EQ(stocks.at(2)->data.at(2).quantity,              4);
    ASSERT_NEAR(stocks.at(2)->data.at(2).price,               0.4f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(3)->meta.instrumentId,                "dddddddd-dddd-dddd-dddd-dddddddddddd");
    ASSERT_EQ(stocks.at(3)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(3)->meta.instrumentTicker,            "ZARA");
    ASSERT_EQ(stocks.at(3)->meta.instrumentName,              "Zaradunger");
    ASSERT_EQ(stocks.at(3)->meta.forQualInvestorFlag,         true);
    ASSERT_NEAR(stocks.at(3)->meta.minPriceIncrement,         0.001f, 0.0001f);
    ASSERT_EQ(stocks.at(3)->meta.pricePrecision,              3);
    ASSERT_EQ(stocks.at(3)->operational.lastStoredTimestamp,  0);
    ASSERT_NEAR(stocks.at(3)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(3)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(3)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(3)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(3)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(3)->data.size(),                      0);
    // clang-format on
}

TEST_F(Test_StocksStorage, Test_appendStockData)
{
    const InSequence seq;

    Stock           stock;
    const StockData stockData{};

    EXPECT_CALL(*stocksDatabaseMock, appendStockData(&stock, &stockData, 1));

    storage->appendStockData(&stock, &stockData, 1);
}

TEST_F(Test_StocksStorage, Test_deleteObsoleteData)
{
    const InSequence seq;

    QList<Stock*> stocks = storage->getStocks();
    ASSERT_EQ(stocks.size(), 0);

    QList<Stock*> stocksDB;

    Stock* stock1 = new Stock(); // StocksStorage will take ownership
    Stock* stock2 = new Stock(); // StocksStorage will take ownership
    Stock* stock3 = new Stock(); // StocksStorage will take ownership

    stock1->meta.instrumentId        = "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa";
    stock1->meta.instrumentTicker    = "TEST";
    stock1->meta.instrumentName      = "abc";
    stock1->meta.forQualInvestorFlag = true;
    stock1->meta.minPriceIncrement   = 0.1f;
    stock1->meta.pricePrecision      = 1;
    stock2->meta.instrumentId        = "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb";
    stock2->meta.instrumentTicker    = "MAGA";
    stock2->meta.instrumentName      = "def";
    stock2->meta.forQualInvestorFlag = false;
    stock2->meta.minPriceIncrement   = 0.01f;
    stock2->meta.pricePrecision      = 2;
    stock3->meta.instrumentId        = "cccccccc-cccc-cccc-cccc-cccccccccccc";
    stock3->meta.instrumentTicker    = "HNYA";
    stock3->meta.instrumentName      = "aaaa";
    stock3->meta.forQualInvestorFlag = true;
    stock3->meta.minPriceIncrement   = 0.001f;
    stock3->meta.pricePrecision      = 3;

    QVariantList timestamps[3];
    QVariantList quantities[3];
    QVariantList prices[3];

    timestamps[0] << 100 << 200 << 300;
    quantities[0] << 1 << 2 << 3;
    prices[0] << 20 << 1000 << 500;

    timestamps[1] << 150 << 250 << 350 << 450 << 550 << 650;
    quantities[1] << 10 << 20 << 30 << 40 << 50 << 60;
    prices[1] << 1000.0f << 999.85f << 1000.35f << 875.95f << 1550.75f << 650.15f;

    timestamps[2] << 120 << 220 << 320 << 420 << 520;
    quantities[2] << 9 << 8 << 7 << 6 << 5;
    prices[2] << 300 << 130 << 450 << 600 << 100;

    stocksDB << stock1 << stock2 << stock3;

    for (int i = 0; i < 3; ++i)
    {
        Stock*    stock    = stocksDB.at(i);
        const int dataSize = timestamps[i].size();

        for (int j = 0; j < dataSize; ++j)
        {
            StockData stockData;

            stockData.timestamp = timestamps[i][j].toLongLong();
            stockData.quantity  = quantities[i][j].toLongLong();
            stockData.price     = prices[i][j].toFloat();

            stock->data.append(stockData);
        }
    }

    stock1->operational.lastStoredTimestamp = stock1->data.constLast().timestamp;
    stock2->operational.lastStoredTimestamp = stock2->data.constLast().timestamp;
    stock3->operational.lastStoredTimestamp = stock3->data.constLast().timestamp;

    EXPECT_CALL(*stocksDatabaseMock, readStocksMeta()).WillOnce(Return(stocksDB));
    EXPECT_CALL(*stocksDatabaseMock, readStocksData(Ne(QList<Stock*>())));

    storage->readFromDatabase();
    stocks = storage->getStocks();

    // clang-format off
    ASSERT_EQ(stocks.size(),                                  3);
    ASSERT_EQ(stocks.at(0)->meta.instrumentId,                "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(0)->meta.instrumentTicker,            "TEST");
    ASSERT_EQ(stocks.at(0)->meta.instrumentName,              "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_NEAR(stocks.at(0)->meta.minPriceIncrement,         0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->meta.pricePrecision,              1);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  300);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      3);
    ASSERT_EQ(stocks.at(0)->data.at(0).timestamp,             100);
    ASSERT_EQ(stocks.at(0)->data.at(0).quantity,              1);
    ASSERT_NEAR(stocks.at(0)->data.at(0).price,               20, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(1).timestamp,             200);
    ASSERT_EQ(stocks.at(0)->data.at(1).quantity,              2);
    ASSERT_NEAR(stocks.at(0)->data.at(1).price,               1000, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(2).timestamp,             300);
    ASSERT_EQ(stocks.at(0)->data.at(2).quantity,              3);
    ASSERT_NEAR(stocks.at(0)->data.at(2).price,               500, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.instrumentId,                "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(1)->meta.instrumentTicker,            "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.instrumentName,              "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_NEAR(stocks.at(1)->meta.minPriceIncrement,         0.01f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->meta.pricePrecision,              2);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  650);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      6);
    ASSERT_EQ(stocks.at(1)->data.at(0).timestamp,             150);
    ASSERT_EQ(stocks.at(1)->data.at(0).quantity,              10);
    ASSERT_NEAR(stocks.at(1)->data.at(0).price,               1000.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(1).timestamp,             250);
    ASSERT_EQ(stocks.at(1)->data.at(1).quantity,              20);
    ASSERT_NEAR(stocks.at(1)->data.at(1).price,               999.85f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(2).timestamp,             350);
    ASSERT_EQ(stocks.at(1)->data.at(2).quantity,              30);
    ASSERT_NEAR(stocks.at(1)->data.at(2).price,               1000.35f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(3).timestamp,             450);
    ASSERT_EQ(stocks.at(1)->data.at(3).quantity,              40);
    ASSERT_NEAR(stocks.at(1)->data.at(3).price,               875.95f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(4).timestamp,             550);
    ASSERT_EQ(stocks.at(1)->data.at(4).quantity,              50);
    ASSERT_NEAR(stocks.at(1)->data.at(4).price,               1550.75f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(5).timestamp,             650);
    ASSERT_EQ(stocks.at(1)->data.at(5).quantity,              60);
    ASSERT_NEAR(stocks.at(1)->data.at(5).price,               650.15f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.instrumentId,                "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(2)->meta.instrumentTicker,            "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.instrumentName,              "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,         true);
    ASSERT_NEAR(stocks.at(2)->meta.minPriceIncrement,         0.001f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->meta.pricePrecision,              3);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,  520);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(2)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(2)->data.size(),                      5);
    ASSERT_EQ(stocks.at(2)->data.at(0).timestamp,             120);
    ASSERT_EQ(stocks.at(2)->data.at(0).quantity,              9);
    ASSERT_NEAR(stocks.at(2)->data.at(0).price,               300, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(1).timestamp,             220);
    ASSERT_EQ(stocks.at(2)->data.at(1).quantity,              8);
    ASSERT_NEAR(stocks.at(2)->data.at(1).price,               130, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(2).timestamp,             320);
    ASSERT_EQ(stocks.at(2)->data.at(2).quantity,              7);
    ASSERT_NEAR(stocks.at(2)->data.at(2).price,               450, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(3).timestamp,             420);
    ASSERT_EQ(stocks.at(2)->data.at(3).quantity,              6);
    ASSERT_NEAR(stocks.at(2)->data.at(3).price,               600, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(4).timestamp,             520);
    ASSERT_EQ(stocks.at(2)->data.at(4).quantity,              5);
    ASSERT_NEAR(stocks.at(2)->data.at(4).price,               100, 0.0001f);
    // clang-format on

    EXPECT_CALL(*stocksDatabaseMock, writeStockData(_)).Times(3);

    storage->deleteObsoleteData(200);

    // clang-format off
    ASSERT_EQ(stocks.size(),                                  3);
    ASSERT_EQ(stocks.at(0)->meta.instrumentId,                "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(0)->meta.instrumentTicker,            "TEST");
    ASSERT_EQ(stocks.at(0)->meta.instrumentName,              "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_NEAR(stocks.at(0)->meta.minPriceIncrement,         0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->meta.pricePrecision,              1);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  300);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      2);
    ASSERT_EQ(stocks.at(0)->data.at(0).timestamp,             200);
    ASSERT_EQ(stocks.at(0)->data.at(0).quantity,              2);
    ASSERT_NEAR(stocks.at(0)->data.at(0).price,               1000, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(1).timestamp,             300);
    ASSERT_EQ(stocks.at(0)->data.at(1).quantity,              3);
    ASSERT_NEAR(stocks.at(0)->data.at(1).price,               500, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.instrumentId,                "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(1)->meta.instrumentTicker,            "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.instrumentName,              "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_NEAR(stocks.at(1)->meta.minPriceIncrement,         0.01f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->meta.pricePrecision,              2);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  650);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      5);
    ASSERT_EQ(stocks.at(1)->data.at(0).timestamp,             250);
    ASSERT_EQ(stocks.at(1)->data.at(0).quantity,              20);
    ASSERT_NEAR(stocks.at(1)->data.at(0).price,               999.85f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(1).timestamp,             350);
    ASSERT_EQ(stocks.at(1)->data.at(1).quantity,              30);
    ASSERT_NEAR(stocks.at(1)->data.at(1).price,               1000.35f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(2).timestamp,             450);
    ASSERT_EQ(stocks.at(1)->data.at(2).quantity,              40);
    ASSERT_NEAR(stocks.at(1)->data.at(2).price,               875.95f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(3).timestamp,             550);
    ASSERT_EQ(stocks.at(1)->data.at(3).quantity,              50);
    ASSERT_NEAR(stocks.at(1)->data.at(3).price,               1550.75f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(4).timestamp,             650);
    ASSERT_EQ(stocks.at(1)->data.at(4).quantity,              60);
    ASSERT_NEAR(stocks.at(1)->data.at(4).price,               650.15f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.instrumentId,                "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(2)->meta.instrumentTicker,            "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.instrumentName,              "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,         true);
    ASSERT_NEAR(stocks.at(2)->meta.minPriceIncrement,         0.001f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->meta.pricePrecision,              3);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,  520);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(2)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(2)->data.size(),                      4);
    ASSERT_EQ(stocks.at(2)->data.at(0).timestamp,             220);
    ASSERT_EQ(stocks.at(2)->data.at(0).quantity,              8);
    ASSERT_NEAR(stocks.at(2)->data.at(0).price,               130, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(1).timestamp,             320);
    ASSERT_EQ(stocks.at(2)->data.at(1).quantity,              7);
    ASSERT_NEAR(stocks.at(2)->data.at(1).price,               450, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(2).timestamp,             420);
    ASSERT_EQ(stocks.at(2)->data.at(2).quantity,              6);
    ASSERT_NEAR(stocks.at(2)->data.at(2).price,               600, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(3).timestamp,             520);
    ASSERT_EQ(stocks.at(2)->data.at(3).quantity,              5);
    ASSERT_NEAR(stocks.at(2)->data.at(3).price,               100, 0.0001f);
    // clang-format on

    EXPECT_CALL(*stocksDatabaseMock, writeStockData(_)).Times(3);

    storage->deleteObsoleteData(400);

    // clang-format off
    ASSERT_EQ(stocks.size(),                                  3);
    ASSERT_EQ(stocks.at(0)->meta.instrumentId,                "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(0)->meta.instrumentTicker,            "TEST");
    ASSERT_EQ(stocks.at(0)->meta.instrumentName,              "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_NEAR(stocks.at(0)->meta.minPriceIncrement,         0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->meta.pricePrecision,              1);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  300);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      0);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.instrumentId,                "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(1)->meta.instrumentTicker,            "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.instrumentName,              "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_NEAR(stocks.at(1)->meta.minPriceIncrement,         0.01f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->meta.pricePrecision,              2);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  650);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      3);
    ASSERT_EQ(stocks.at(1)->data.at(0).timestamp,             450);
    ASSERT_EQ(stocks.at(1)->data.at(0).quantity,              40);
    ASSERT_NEAR(stocks.at(1)->data.at(0).price,               875.95f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(1).timestamp,             550);
    ASSERT_EQ(stocks.at(1)->data.at(1).quantity,              50);
    ASSERT_NEAR(stocks.at(1)->data.at(1).price,               1550.75f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(2).timestamp,             650);
    ASSERT_EQ(stocks.at(1)->data.at(2).quantity,              60);
    ASSERT_NEAR(stocks.at(1)->data.at(2).price,               650.15f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.instrumentId,                "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(2)->meta.instrumentTicker,            "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.instrumentName,              "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,         true);
    ASSERT_NEAR(stocks.at(2)->meta.minPriceIncrement,         0.001f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->meta.pricePrecision,              3);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,  520);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(2)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(2)->data.size(),                      2);
    ASSERT_EQ(stocks.at(2)->data.at(0).timestamp,             420);
    ASSERT_EQ(stocks.at(2)->data.at(0).quantity,              6);
    ASSERT_NEAR(stocks.at(2)->data.at(0).price,               600, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(1).timestamp,             520);
    ASSERT_EQ(stocks.at(2)->data.at(1).quantity,              5);
    ASSERT_NEAR(stocks.at(2)->data.at(1).price,               100, 0.0001f);
    // clang-format on
}

TEST_F(Test_StocksStorage, Test_cleanupOperationalData)
{
    const InSequence seq;

    QList<Stock*> stocks = storage->getStocks();
    ASSERT_EQ(stocks.size(), 0);

    QList<Stock*> stocksDB;

    Stock* stock1 = new Stock(); // StocksStorage will take ownership
    Stock* stock2 = new Stock(); // StocksStorage will take ownership
    Stock* stock3 = new Stock(); // StocksStorage will take ownership

    stock1->meta.instrumentId        = "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa";
    stock1->meta.instrumentTicker    = "TEST";
    stock1->meta.instrumentName      = "abc";
    stock1->meta.forQualInvestorFlag = true;
    stock1->meta.minPriceIncrement   = 0.1f;
    stock1->meta.pricePrecision      = 1;
    stock2->meta.instrumentId        = "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb";
    stock2->meta.instrumentTicker    = "MAGA";
    stock2->meta.instrumentName      = "def";
    stock2->meta.forQualInvestorFlag = false;
    stock2->meta.minPriceIncrement   = 0.01f;
    stock2->meta.pricePrecision      = 2;
    stock3->meta.instrumentId        = "cccccccc-cccc-cccc-cccc-cccccccccccc";
    stock3->meta.instrumentTicker    = "HNYA";
    stock3->meta.instrumentName      = "aaaa";
    stock3->meta.forQualInvestorFlag = true;
    stock3->meta.minPriceIncrement   = 0.001f;
    stock3->meta.pricePrecision      = 3;

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
        Stock*    stock    = stocksDB.at(i);
        const int dataSize = timestamps[i].size();

        for (int j = 0; j < dataSize; ++j)
        {
            StockOperationalData stockData;

            stockData.timestamp = timestamps[i][j].toLongLong();
            stockData.price     = prices[i][j].toFloat();

            stock->operational.detailedData.append(stockData);
        }
    }

    EXPECT_CALL(*stocksDatabaseMock, readStocksMeta()).WillOnce(Return(stocksDB));
    EXPECT_CALL(*stocksDatabaseMock, readStocksData(Ne(QList<Stock*>())));

    storage->readFromDatabase();
    stocks = storage->getStocks();

    // clang-format off
    ASSERT_EQ(stocks.size(),                                          3);
    ASSERT_EQ(stocks.at(0)->meta.instrumentId,                        "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.instrumentLogo,                      nullptr);
    ASSERT_EQ(stocks.at(0)->meta.instrumentTicker,                    "TEST");
    ASSERT_EQ(stocks.at(0)->meta.instrumentName,                      "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,                 true);
    ASSERT_NEAR(stocks.at(0)->meta.minPriceIncrement,                 0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->meta.pricePrecision,                      1);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,          0);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,              -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice,         -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.turnover,                     0);
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
    ASSERT_EQ(stocks.at(1)->meta.instrumentId,                        "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.instrumentLogo,                      nullptr);
    ASSERT_EQ(stocks.at(1)->meta.instrumentTicker,                    "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.instrumentName,                      "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,                 false);
    ASSERT_NEAR(stocks.at(1)->meta.minPriceIncrement,                 0.01f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->meta.pricePrecision,                      2);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,          0);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,              -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice,         -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.turnover,                     0);
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
    ASSERT_EQ(stocks.at(2)->meta.instrumentId,                        "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.instrumentLogo,                      nullptr);
    ASSERT_EQ(stocks.at(2)->meta.instrumentTicker,                    "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.instrumentName,                      "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,                 true);
    ASSERT_NEAR(stocks.at(2)->meta.minPriceIncrement,                 0.001f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->meta.pricePrecision,                      3);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,          0);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,              -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice,         -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.turnover,                     0);
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
    ASSERT_EQ(stocks.at(0)->meta.instrumentId,                        "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.instrumentLogo,                      nullptr);
    ASSERT_EQ(stocks.at(0)->meta.instrumentTicker,                    "TEST");
    ASSERT_EQ(stocks.at(0)->meta.instrumentName,                      "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,                 true);
    ASSERT_NEAR(stocks.at(0)->meta.minPriceIncrement,                 0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->meta.pricePrecision,                      1);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,          0);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,              -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice,         -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.turnover,                     0);
    ASSERT_NEAR(stocks.at(0)->operational.payback,                    0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),          2);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.at(0).timestamp, 200);
    ASSERT_NEAR(stocks.at(0)->operational.detailedData.at(0).price,   1000, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.at(1).timestamp, 300);
    ASSERT_NEAR(stocks.at(0)->operational.detailedData.at(1).price,   500, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.size(),                              0);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.instrumentId,                        "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.instrumentLogo,                      nullptr);
    ASSERT_EQ(stocks.at(1)->meta.instrumentTicker,                    "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.instrumentName,                      "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,                 false);
    ASSERT_NEAR(stocks.at(1)->meta.minPriceIncrement,                 0.01f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->meta.pricePrecision,                      2);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,          0);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,              -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice,         -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.turnover,                     0);
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
    ASSERT_EQ(stocks.at(2)->meta.instrumentId,                        "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.instrumentLogo,                      nullptr);
    ASSERT_EQ(stocks.at(2)->meta.instrumentTicker,                    "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.instrumentName,                      "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,                 true);
    ASSERT_NEAR(stocks.at(2)->meta.minPriceIncrement,                 0.001f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->meta.pricePrecision,                      3);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,          0);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,              -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice,         -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.turnover,                     0);
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
    ASSERT_EQ(stocks.at(0)->meta.instrumentId,                "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(0)->meta.instrumentTicker,            "TEST");
    ASSERT_EQ(stocks.at(0)->meta.instrumentName,              "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_NEAR(stocks.at(0)->meta.minPriceIncrement,         0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->meta.pricePrecision,              1);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  0);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      0);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.instrumentId,                        "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.instrumentLogo,                      nullptr);
    ASSERT_EQ(stocks.at(1)->meta.instrumentTicker,                    "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.instrumentName,                      "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,                 false);
    ASSERT_NEAR(stocks.at(1)->meta.minPriceIncrement,                 0.01f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->meta.pricePrecision,                      2);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,          0);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,              -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice,         -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.turnover,                     0);
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
    ASSERT_EQ(stocks.at(2)->meta.instrumentId,                        "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.instrumentLogo,                      nullptr);
    ASSERT_EQ(stocks.at(2)->meta.instrumentTicker,                    "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.instrumentName,                      "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,                 true);
    ASSERT_NEAR(stocks.at(2)->meta.minPriceIncrement,                 0.001f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->meta.pricePrecision,                      3);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,          0);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,              -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice,         -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.turnover,                     0);
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
    const InSequence seq;

    QList<Stock*> stocks = storage->getStocks();
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

    stock1->meta.instrumentId        = "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa";
    stock1->meta.instrumentTicker    = "TEST";
    stock1->meta.instrumentName      = "abc";
    stock1->meta.forQualInvestorFlag = true;
    stock1->meta.minPriceIncrement   = 0.1f;
    stock1->meta.pricePrecision      = 1;
    stock2->meta.instrumentId        = "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb";
    stock2->meta.instrumentTicker    = "MAGA";
    stock2->meta.instrumentName      = "def";
    stock2->meta.forQualInvestorFlag = false;
    stock2->meta.minPriceIncrement   = 0.01f;
    stock2->meta.pricePrecision      = 2;
    stock3->meta.instrumentId        = "cccccccc-cccc-cccc-cccc-cccccccccccc";
    stock3->meta.instrumentTicker    = "HNYA";
    stock3->meta.instrumentName      = "aaaa";
    stock3->meta.forQualInvestorFlag = true;
    stock3->meta.minPriceIncrement   = 0.001f;
    stock3->meta.pricePrecision      = 3;

    stockData1.timestamp = 100;
    stockData2.timestamp = 200;
    stockData3.timestamp = 300;
    stockData4.timestamp = 400;
    stockData5.timestamp = 500;
    stockData6.timestamp = 600;
    stockData7.timestamp = 700;

    stockData1.quantity = 1;
    stockData2.quantity = 2;
    stockData3.quantity = 3;
    stockData4.quantity = 4;
    stockData5.quantity = 5;
    stockData6.quantity = 6;
    stockData7.quantity = 7;

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

    stock1->operational.lastStoredTimestamp = stock1->data.constLast().timestamp;
    stock2->operational.lastStoredTimestamp = stock2->data.constLast().timestamp;
    stock3->operational.lastStoredTimestamp = stock3->data.constLast().timestamp;

    stocksDB << stock1 << stock2 << stock3;

    EXPECT_CALL(*stocksDatabaseMock, readStocksMeta()).WillOnce(Return(stocksDB));
    EXPECT_CALL(*stocksDatabaseMock, readStocksData(Ne(QList<Stock*>())));

    storage->readFromDatabase();
    stocks = storage->getStocks();

    // clang-format off
    ASSERT_EQ(stocks.size(),                                  3);
    ASSERT_EQ(stocks.at(0)->meta.instrumentId,                "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(0)->meta.instrumentTicker,            "TEST");
    ASSERT_EQ(stocks.at(0)->meta.instrumentName,              "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_NEAR(stocks.at(0)->meta.minPriceIncrement,         0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->meta.pricePrecision,              1);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  500);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      3);
    ASSERT_EQ(stocks.at(0)->data.at(0).timestamp,             100);
    ASSERT_EQ(stocks.at(0)->data.at(0).quantity,              1);
    ASSERT_NEAR(stocks.at(0)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(1).timestamp,             200);
    ASSERT_EQ(stocks.at(0)->data.at(1).quantity,              2);
    ASSERT_NEAR(stocks.at(0)->data.at(1).price,               0.2f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(2).timestamp,             500);
    ASSERT_EQ(stocks.at(0)->data.at(2).quantity,              5);
    ASSERT_NEAR(stocks.at(0)->data.at(2).price,               0.5f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.instrumentId,                "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(1)->meta.instrumentTicker,            "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.instrumentName,              "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_NEAR(stocks.at(1)->meta.minPriceIncrement,         0.01f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->meta.pricePrecision,              2);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  700);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      4);
    ASSERT_EQ(stocks.at(1)->data.at(0).timestamp,             200);
    ASSERT_EQ(stocks.at(1)->data.at(0).quantity,              2);
    ASSERT_NEAR(stocks.at(1)->data.at(0).price,               0.2f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(1).timestamp,             300);
    ASSERT_EQ(stocks.at(1)->data.at(1).quantity,              3);
    ASSERT_NEAR(stocks.at(1)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(2).timestamp,             600);
    ASSERT_EQ(stocks.at(1)->data.at(2).quantity,              6);
    ASSERT_NEAR(stocks.at(1)->data.at(2).price,               0.6f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(3).timestamp,             700);
    ASSERT_EQ(stocks.at(1)->data.at(3).quantity,              7);
    ASSERT_NEAR(stocks.at(1)->data.at(3).price,               0.7f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.instrumentId,                "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(2)->meta.instrumentTicker,            "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.instrumentName,              "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,         true);
    ASSERT_NEAR(stocks.at(2)->meta.minPriceIncrement,         0.001f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->meta.pricePrecision,              3);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,  500);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(2)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(2)->data.size(),                      4);
    ASSERT_EQ(stocks.at(2)->data.at(0).timestamp,             100);
    ASSERT_EQ(stocks.at(2)->data.at(0).quantity,              1);
    ASSERT_NEAR(stocks.at(2)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(1).timestamp,             300);
    ASSERT_EQ(stocks.at(2)->data.at(1).quantity,              3);
    ASSERT_NEAR(stocks.at(2)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(2).timestamp,             400);
    ASSERT_EQ(stocks.at(2)->data.at(2).quantity,              4);
    ASSERT_NEAR(stocks.at(2)->data.at(2).price,               0.4f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(3).timestamp,             500);
    ASSERT_EQ(stocks.at(2)->data.at(3).quantity,              5);
    ASSERT_NEAR(stocks.at(2)->data.at(3).price,               0.5f, 0.0001f);
    // clang-format on

    storage->obtainStocksDayStartPrice(400);

    // clang-format off
    ASSERT_EQ(stocks.size(),                                  3);
    ASSERT_EQ(stocks.at(0)->meta.instrumentId,                "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(0)->meta.instrumentTicker,            "TEST");
    ASSERT_EQ(stocks.at(0)->meta.instrumentName,              "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_NEAR(stocks.at(0)->meta.minPriceIncrement,         0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->meta.pricePrecision,              1);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  500);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      0.2f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      3);
    ASSERT_EQ(stocks.at(0)->data.at(0).timestamp,             100);
    ASSERT_EQ(stocks.at(0)->data.at(0).quantity,              1);
    ASSERT_NEAR(stocks.at(0)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(1).timestamp,             200);
    ASSERT_EQ(stocks.at(0)->data.at(1).quantity,              2);
    ASSERT_NEAR(stocks.at(0)->data.at(1).price,               0.2f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(2).timestamp,             500);
    ASSERT_EQ(stocks.at(0)->data.at(2).quantity,              5);
    ASSERT_NEAR(stocks.at(0)->data.at(2).price,               0.5f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.instrumentId,                "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(1)->meta.instrumentTicker,            "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.instrumentName,              "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_NEAR(stocks.at(1)->meta.minPriceIncrement,         0.01f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->meta.pricePrecision,              2);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  700);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      0.3f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      4);
    ASSERT_EQ(stocks.at(1)->data.at(0).timestamp,             200);
    ASSERT_EQ(stocks.at(1)->data.at(0).quantity,              2);
    ASSERT_NEAR(stocks.at(1)->data.at(0).price,               0.2f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(1).timestamp,             300);
    ASSERT_EQ(stocks.at(1)->data.at(1).quantity,              3);
    ASSERT_NEAR(stocks.at(1)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(2).timestamp,             600);
    ASSERT_EQ(stocks.at(1)->data.at(2).quantity,              6);
    ASSERT_NEAR(stocks.at(1)->data.at(2).price,               0.6f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(3).timestamp,             700);
    ASSERT_EQ(stocks.at(1)->data.at(3).quantity,              7);
    ASSERT_NEAR(stocks.at(1)->data.at(3).price,               0.7f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.instrumentId,                "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(2)->meta.instrumentTicker,            "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.instrumentName,              "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,         true);
    ASSERT_NEAR(stocks.at(2)->meta.minPriceIncrement,         0.001f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->meta.pricePrecision,              3);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,  500);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,      0.4f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(2)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(2)->data.size(),                      4);
    ASSERT_EQ(stocks.at(2)->data.at(0).timestamp,             100);
    ASSERT_EQ(stocks.at(2)->data.at(0).quantity,              1);
    ASSERT_NEAR(stocks.at(2)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(1).timestamp,             300);
    ASSERT_EQ(stocks.at(2)->data.at(1).quantity,              3);
    ASSERT_NEAR(stocks.at(2)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(2).timestamp,             400);
    ASSERT_EQ(stocks.at(2)->data.at(2).quantity,              4);
    ASSERT_NEAR(stocks.at(2)->data.at(2).price,               0.4f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(3).timestamp,             500);
    ASSERT_EQ(stocks.at(2)->data.at(3).quantity,              5);
    ASSERT_NEAR(stocks.at(2)->data.at(3).price,               0.5f, 0.0001f);
    // clang-format on
}

TEST_F(Test_StocksStorage, Test_obtainStocksDatePrice)
{
    const InSequence seq;

    QList<Stock*> stocks = storage->getStocks();
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

    stock1->meta.instrumentId        = "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa";
    stock1->meta.instrumentTicker    = "TEST";
    stock1->meta.instrumentName      = "abc";
    stock1->meta.forQualInvestorFlag = true;
    stock1->meta.minPriceIncrement   = 0.1f;
    stock1->meta.pricePrecision      = 1;
    stock2->meta.instrumentId        = "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb";
    stock2->meta.instrumentTicker    = "MAGA";
    stock2->meta.instrumentName      = "def";
    stock2->meta.forQualInvestorFlag = false;
    stock2->meta.minPriceIncrement   = 0.01f;
    stock2->meta.pricePrecision      = 2;
    stock3->meta.instrumentId        = "cccccccc-cccc-cccc-cccc-cccccccccccc";
    stock3->meta.instrumentTicker    = "HNYA";
    stock3->meta.instrumentName      = "aaaa";
    stock3->meta.forQualInvestorFlag = true;
    stock3->meta.minPriceIncrement   = 0.001f;
    stock3->meta.pricePrecision      = 3;

    stockData1.timestamp = 100;
    stockData2.timestamp = 200;
    stockData3.timestamp = 300;
    stockData4.timestamp = 400;
    stockData5.timestamp = 500;
    stockData6.timestamp = 600;
    stockData7.timestamp = 700;

    stockData1.quantity = 1;
    stockData2.quantity = 2;
    stockData3.quantity = 3;
    stockData4.quantity = 4;
    stockData5.quantity = 5;
    stockData6.quantity = 6;
    stockData7.quantity = 7;

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

    stock1->operational.lastStoredTimestamp = stock1->data.constLast().timestamp;
    stock2->operational.lastStoredTimestamp = stock2->data.constLast().timestamp;
    stock3->operational.lastStoredTimestamp = stock3->data.constLast().timestamp;

    stocksDB << stock1 << stock2 << stock3;

    EXPECT_CALL(*stocksDatabaseMock, readStocksMeta()).WillOnce(Return(stocksDB));
    EXPECT_CALL(*stocksDatabaseMock, readStocksData(Ne(QList<Stock*>())));

    storage->readFromDatabase();
    stocks = storage->getStocks();

    // clang-format off
    ASSERT_EQ(stocks.size(),                                  3);
    ASSERT_EQ(stocks.at(0)->meta.instrumentId,                "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(0)->meta.instrumentTicker,            "TEST");
    ASSERT_EQ(stocks.at(0)->meta.instrumentName,              "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_NEAR(stocks.at(0)->meta.minPriceIncrement,         0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->meta.pricePrecision,              1);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  500);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      3);
    ASSERT_EQ(stocks.at(0)->data.at(0).timestamp,             100);
    ASSERT_EQ(stocks.at(0)->data.at(0).quantity,              1);
    ASSERT_NEAR(stocks.at(0)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(1).timestamp,             200);
    ASSERT_EQ(stocks.at(0)->data.at(1).quantity,              2);
    ASSERT_NEAR(stocks.at(0)->data.at(1).price,               0.2f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(2).timestamp,             500);
    ASSERT_EQ(stocks.at(0)->data.at(2).quantity,              5);
    ASSERT_NEAR(stocks.at(0)->data.at(2).price,               0.5f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.instrumentId,                "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(1)->meta.instrumentTicker,            "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.instrumentName,              "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_NEAR(stocks.at(1)->meta.minPriceIncrement,         0.01f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->meta.pricePrecision,              2);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  700);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      4);
    ASSERT_EQ(stocks.at(1)->data.at(0).timestamp,             200);
    ASSERT_EQ(stocks.at(1)->data.at(0).quantity,              2);
    ASSERT_NEAR(stocks.at(1)->data.at(0).price,               0.2f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(1).timestamp,             300);
    ASSERT_EQ(stocks.at(1)->data.at(1).quantity,              3);
    ASSERT_NEAR(stocks.at(1)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(2).timestamp,             600);
    ASSERT_EQ(stocks.at(1)->data.at(2).quantity,              6);
    ASSERT_NEAR(stocks.at(1)->data.at(2).price,               0.6f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(3).timestamp,             700);
    ASSERT_EQ(stocks.at(1)->data.at(3).quantity,              7);
    ASSERT_NEAR(stocks.at(1)->data.at(3).price,               0.7f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.instrumentId,                "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(2)->meta.instrumentTicker,            "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.instrumentName,              "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,         true);
    ASSERT_NEAR(stocks.at(2)->meta.minPriceIncrement,         0.001f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->meta.pricePrecision,              3);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,  500);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(2)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(2)->data.size(),                      4);
    ASSERT_EQ(stocks.at(2)->data.at(0).timestamp,             100);
    ASSERT_EQ(stocks.at(2)->data.at(0).quantity,              1);
    ASSERT_NEAR(stocks.at(2)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(1).timestamp,             300);
    ASSERT_EQ(stocks.at(2)->data.at(1).quantity,              3);
    ASSERT_NEAR(stocks.at(2)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(2).timestamp,             400);
    ASSERT_EQ(stocks.at(2)->data.at(2).quantity,              4);
    ASSERT_NEAR(stocks.at(2)->data.at(2).price,               0.4f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(3).timestamp,             500);
    ASSERT_EQ(stocks.at(2)->data.at(3).quantity,              5);
    ASSERT_NEAR(stocks.at(2)->data.at(3).price,               0.5f, 0.0001f);
    // clang-format on

    storage->obtainStocksDatePrice(400);

    // clang-format off
    ASSERT_EQ(stocks.size(),                                  3);
    ASSERT_EQ(stocks.at(0)->meta.instrumentId,                "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(0)->meta.instrumentTicker,            "TEST");
    ASSERT_EQ(stocks.at(0)->meta.instrumentName,              "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_NEAR(stocks.at(0)->meta.minPriceIncrement,         0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->meta.pricePrecision,              1);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  500);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, 0.2f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      3);
    ASSERT_EQ(stocks.at(0)->data.at(0).timestamp,             100);
    ASSERT_EQ(stocks.at(0)->data.at(0).quantity,              1);
    ASSERT_NEAR(stocks.at(0)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(1).timestamp,             200);
    ASSERT_EQ(stocks.at(0)->data.at(1).quantity,              2);
    ASSERT_NEAR(stocks.at(0)->data.at(1).price,               0.2f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(2).timestamp,             500);
    ASSERT_EQ(stocks.at(0)->data.at(2).quantity,              5);
    ASSERT_NEAR(stocks.at(0)->data.at(2).price,               0.5f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.instrumentId,                "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(1)->meta.instrumentTicker,            "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.instrumentName,              "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_NEAR(stocks.at(1)->meta.minPriceIncrement,         0.01f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->meta.pricePrecision,              2);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  700);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, 0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      4);
    ASSERT_EQ(stocks.at(1)->data.at(0).timestamp,             200);
    ASSERT_EQ(stocks.at(1)->data.at(0).quantity,              2);
    ASSERT_NEAR(stocks.at(1)->data.at(0).price,               0.2f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(1).timestamp,             300);
    ASSERT_EQ(stocks.at(1)->data.at(1).quantity,              3);
    ASSERT_NEAR(stocks.at(1)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(2).timestamp,             600);
    ASSERT_EQ(stocks.at(1)->data.at(2).quantity,              6);
    ASSERT_NEAR(stocks.at(1)->data.at(2).price,               0.6f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(3).timestamp,             700);
    ASSERT_EQ(stocks.at(1)->data.at(3).quantity,              7);
    ASSERT_NEAR(stocks.at(1)->data.at(3).price,               0.7f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.instrumentId,                "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(2)->meta.instrumentTicker,            "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.instrumentName,              "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,         true);
    ASSERT_NEAR(stocks.at(2)->meta.minPriceIncrement,         0.001f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->meta.pricePrecision,              3);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,  500);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice, 0.4f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(2)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(2)->data.size(),                      4);
    ASSERT_EQ(stocks.at(2)->data.at(0).timestamp,             100);
    ASSERT_EQ(stocks.at(2)->data.at(0).quantity,              1);
    ASSERT_NEAR(stocks.at(2)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(1).timestamp,             300);
    ASSERT_EQ(stocks.at(2)->data.at(1).quantity,              3);
    ASSERT_NEAR(stocks.at(2)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(2).timestamp,             400);
    ASSERT_EQ(stocks.at(2)->data.at(2).quantity,              4);
    ASSERT_NEAR(stocks.at(2)->data.at(2).price,               0.4f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(3).timestamp,             500);
    ASSERT_EQ(stocks.at(2)->data.at(3).quantity,              5);
    ASSERT_NEAR(stocks.at(2)->data.at(3).price,               0.5f, 0.0001f);
    // clang-format on
}

TEST_F(Test_StocksStorage, Test_obtainTurnover)
{
    const InSequence seq;

    QList<Stock*> stocks = storage->getStocks();
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

    stock1->meta.instrumentId        = "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa";
    stock1->meta.instrumentTicker    = "TEST";
    stock1->meta.instrumentName      = "abc";
    stock1->meta.forQualInvestorFlag = true;
    stock1->meta.minPriceIncrement   = 0.1f;
    stock1->meta.pricePrecision      = 1;
    stock2->meta.instrumentId        = "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb";
    stock2->meta.instrumentTicker    = "MAGA";
    stock2->meta.instrumentName      = "def";
    stock2->meta.forQualInvestorFlag = false;
    stock2->meta.minPriceIncrement   = 0.01f;
    stock2->meta.pricePrecision      = 2;
    stock3->meta.instrumentId        = "cccccccc-cccc-cccc-cccc-cccccccccccc";
    stock3->meta.instrumentTicker    = "HNYA";
    stock3->meta.instrumentName      = "aaaa";
    stock3->meta.forQualInvestorFlag = true;
    stock3->meta.minPriceIncrement   = 0.001f;
    stock3->meta.pricePrecision      = 3;

    stockData1.timestamp = 100;
    stockData2.timestamp = 200;
    stockData3.timestamp = 300;
    stockData4.timestamp = 400;
    stockData5.timestamp = 500;
    stockData6.timestamp = 600;
    stockData7.timestamp = 700;

    stockData1.quantity = 1;
    stockData2.quantity = 2;
    stockData3.quantity = 3;
    stockData4.quantity = 4;
    stockData5.quantity = 5;
    stockData6.quantity = 6;
    stockData7.quantity = 7;

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

    stock1->operational.lastStoredTimestamp = stock1->data.constLast().timestamp;
    stock2->operational.lastStoredTimestamp = stock2->data.constLast().timestamp;
    stock3->operational.lastStoredTimestamp = stock3->data.constLast().timestamp;

    stocksDB << stock1 << stock2 << stock3;

    EXPECT_CALL(*stocksDatabaseMock, readStocksMeta()).WillOnce(Return(stocksDB));
    EXPECT_CALL(*stocksDatabaseMock, readStocksData(Ne(QList<Stock*>())));

    storage->readFromDatabase();
    stocks = storage->getStocks();

    // clang-format off
    ASSERT_EQ(stocks.size(),                                  3);
    ASSERT_EQ(stocks.at(0)->meta.instrumentId,                "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(0)->meta.instrumentTicker,            "TEST");
    ASSERT_EQ(stocks.at(0)->meta.instrumentName,              "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_NEAR(stocks.at(0)->meta.minPriceIncrement,         0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->meta.pricePrecision,              1);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  500);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      3);
    ASSERT_EQ(stocks.at(0)->data.at(0).timestamp,             100);
    ASSERT_EQ(stocks.at(0)->data.at(0).quantity,              1);
    ASSERT_NEAR(stocks.at(0)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(1).timestamp,             200);
    ASSERT_EQ(stocks.at(0)->data.at(1).quantity,              2);
    ASSERT_NEAR(stocks.at(0)->data.at(1).price,               0.9f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(2).timestamp,             500);
    ASSERT_EQ(stocks.at(0)->data.at(2).quantity,              5);
    ASSERT_NEAR(stocks.at(0)->data.at(2).price,               0.2f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.instrumentId,                "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(1)->meta.instrumentTicker,            "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.instrumentName,              "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_NEAR(stocks.at(1)->meta.minPriceIncrement,         0.01f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->meta.pricePrecision,              2);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  700);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      4);
    ASSERT_EQ(stocks.at(1)->data.at(0).timestamp,             200);
    ASSERT_EQ(stocks.at(1)->data.at(0).quantity,              2);
    ASSERT_NEAR(stocks.at(1)->data.at(0).price,               0.9f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(1).timestamp,             300);
    ASSERT_EQ(stocks.at(1)->data.at(1).quantity,              3);
    ASSERT_NEAR(stocks.at(1)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(2).timestamp,             600);
    ASSERT_EQ(stocks.at(1)->data.at(2).quantity,              6);
    ASSERT_NEAR(stocks.at(1)->data.at(2).price,               0.6f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(3).timestamp,             700);
    ASSERT_EQ(stocks.at(1)->data.at(3).quantity,              7);
    ASSERT_NEAR(stocks.at(1)->data.at(3).price,               0.1f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.instrumentId,                "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(2)->meta.instrumentTicker,            "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.instrumentName,              "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,         true);
    ASSERT_NEAR(stocks.at(2)->meta.minPriceIncrement,         0.001f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->meta.pricePrecision,              3);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,  500);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(2)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(2)->data.size(),                      4);
    ASSERT_EQ(stocks.at(2)->data.at(0).timestamp,             100);
    ASSERT_EQ(stocks.at(2)->data.at(0).quantity,              1);
    ASSERT_NEAR(stocks.at(2)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(1).timestamp,             300);
    ASSERT_EQ(stocks.at(2)->data.at(1).quantity,              3);
    ASSERT_NEAR(stocks.at(2)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(2).timestamp,             400);
    ASSERT_EQ(stocks.at(2)->data.at(2).quantity,              4);
    ASSERT_NEAR(stocks.at(2)->data.at(2).price,               0.4f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(3).timestamp,             500);
    ASSERT_EQ(stocks.at(2)->data.at(3).quantity,              5);
    ASSERT_NEAR(stocks.at(2)->data.at(3).price,               0.2f, 0.0001f);
    // clang-format on

    storage->obtainTurnover(200);

    // clang-format off
    ASSERT_EQ(stocks.size(),                                  3);
    ASSERT_EQ(stocks.at(0)->meta.instrumentId,                "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(0)->meta.instrumentTicker,            "TEST");
    ASSERT_EQ(stocks.at(0)->meta.instrumentName,              "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_NEAR(stocks.at(0)->meta.minPriceIncrement,         0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->meta.pricePrecision,              1);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  500);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.turnover,             864000);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      3);
    ASSERT_EQ(stocks.at(0)->data.at(0).timestamp,             100);
    ASSERT_EQ(stocks.at(0)->data.at(0).quantity,              1);
    ASSERT_NEAR(stocks.at(0)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(1).timestamp,             200);
    ASSERT_EQ(stocks.at(0)->data.at(1).quantity,              2);
    ASSERT_NEAR(stocks.at(0)->data.at(1).price,               0.9f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(2).timestamp,             500);
    ASSERT_EQ(stocks.at(0)->data.at(2).quantity,              5);
    ASSERT_NEAR(stocks.at(0)->data.at(2).price,               0.2f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.instrumentId,                "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(1)->meta.instrumentTicker,            "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.instrumentName,              "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_NEAR(stocks.at(1)->meta.minPriceIncrement,         0.01f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->meta.pricePrecision,              2);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  700);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.turnover,             1382400);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      4);
    ASSERT_EQ(stocks.at(1)->data.at(0).timestamp,             200);
    ASSERT_EQ(stocks.at(1)->data.at(0).quantity,              2);
    ASSERT_NEAR(stocks.at(1)->data.at(0).price,               0.9f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(1).timestamp,             300);
    ASSERT_EQ(stocks.at(1)->data.at(1).quantity,              3);
    ASSERT_NEAR(stocks.at(1)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(2).timestamp,             600);
    ASSERT_EQ(stocks.at(1)->data.at(2).quantity,              6);
    ASSERT_NEAR(stocks.at(1)->data.at(2).price,               0.6f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(3).timestamp,             700);
    ASSERT_EQ(stocks.at(1)->data.at(3).quantity,              7);
    ASSERT_NEAR(stocks.at(1)->data.at(3).price,               0.1f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.instrumentId,                "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(2)->meta.instrumentTicker,            "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.instrumentName,              "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,         true);
    ASSERT_NEAR(stocks.at(2)->meta.minPriceIncrement,         0.001f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->meta.pricePrecision,              3);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,  500);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.turnover,             864000);
    ASSERT_NEAR(stocks.at(2)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(2)->data.size(),                      4);
    ASSERT_EQ(stocks.at(2)->data.at(0).timestamp,             100);
    ASSERT_EQ(stocks.at(2)->data.at(0).quantity,              1);
    ASSERT_NEAR(stocks.at(2)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(1).timestamp,             300);
    ASSERT_EQ(stocks.at(2)->data.at(1).quantity,              3);
    ASSERT_NEAR(stocks.at(2)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(2).timestamp,             400);
    ASSERT_EQ(stocks.at(2)->data.at(2).quantity,              4);
    ASSERT_NEAR(stocks.at(2)->data.at(2).price,               0.4f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(3).timestamp,             500);
    ASSERT_EQ(stocks.at(2)->data.at(3).quantity,              5);
    ASSERT_NEAR(stocks.at(2)->data.at(3).price,               0.2f, 0.0001f);
    // clang-format on
}

TEST_F(Test_StocksStorage, Test_obtainPayback)
{
    const InSequence seq;

    QList<Stock*> stocks = storage->getStocks();
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

    stock1->meta.instrumentId        = "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa";
    stock1->meta.instrumentTicker    = "TEST";
    stock1->meta.instrumentName      = "abc";
    stock1->meta.forQualInvestorFlag = true;
    stock1->meta.minPriceIncrement   = 0.1f;
    stock1->meta.pricePrecision      = 1;
    stock2->meta.instrumentId        = "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb";
    stock2->meta.instrumentTicker    = "MAGA";
    stock2->meta.instrumentName      = "def";
    stock2->meta.forQualInvestorFlag = false;
    stock2->meta.minPriceIncrement   = 0.01f;
    stock2->meta.pricePrecision      = 2;
    stock3->meta.instrumentId        = "cccccccc-cccc-cccc-cccc-cccccccccccc";
    stock3->meta.instrumentTicker    = "HNYA";
    stock3->meta.instrumentName      = "aaaa";
    stock3->meta.forQualInvestorFlag = true;
    stock3->meta.minPriceIncrement   = 0.001f;
    stock3->meta.pricePrecision      = 3;

    stockData1.timestamp = 100;
    stockData2.timestamp = 200;
    stockData3.timestamp = 300;
    stockData4.timestamp = 400;
    stockData5.timestamp = 500;
    stockData6.timestamp = 600;
    stockData7.timestamp = 700;

    stockData1.quantity = 1;
    stockData2.quantity = 2;
    stockData3.quantity = 3;
    stockData4.quantity = 4;
    stockData5.quantity = 5;
    stockData6.quantity = 6;
    stockData7.quantity = 7;

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

    stock1->operational.lastStoredTimestamp = stock1->data.constLast().timestamp;
    stock2->operational.lastStoredTimestamp = stock2->data.constLast().timestamp;
    stock3->operational.lastStoredTimestamp = stock3->data.constLast().timestamp;

    stocksDB << stock1 << stock2 << stock3;

    EXPECT_CALL(*stocksDatabaseMock, readStocksMeta()).WillOnce(Return(stocksDB));
    EXPECT_CALL(*stocksDatabaseMock, readStocksData(Ne(QList<Stock*>())));

    storage->readFromDatabase();
    stocks = storage->getStocks();

    // clang-format off
    ASSERT_EQ(stocks.size(),                                  3);
    ASSERT_EQ(stocks.at(0)->meta.instrumentId,                "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(0)->meta.instrumentTicker,            "TEST");
    ASSERT_EQ(stocks.at(0)->meta.instrumentName,              "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_NEAR(stocks.at(0)->meta.minPriceIncrement,         0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->meta.pricePrecision,              1);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  500);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      3);
    ASSERT_EQ(stocks.at(0)->data.at(0).timestamp,             100);
    ASSERT_EQ(stocks.at(0)->data.at(0).quantity,              1);
    ASSERT_NEAR(stocks.at(0)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(1).timestamp,             200);
    ASSERT_EQ(stocks.at(0)->data.at(1).quantity,              2);
    ASSERT_NEAR(stocks.at(0)->data.at(1).price,               0.9f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(2).timestamp,             500);
    ASSERT_EQ(stocks.at(0)->data.at(2).quantity,              5);
    ASSERT_NEAR(stocks.at(0)->data.at(2).price,               0.2f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.instrumentId,                "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(1)->meta.instrumentTicker,            "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.instrumentName,              "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_NEAR(stocks.at(1)->meta.minPriceIncrement,         0.01f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->meta.pricePrecision,              2);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  700);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      4);
    ASSERT_EQ(stocks.at(1)->data.at(0).timestamp,             200);
    ASSERT_EQ(stocks.at(1)->data.at(0).quantity,              2);
    ASSERT_NEAR(stocks.at(1)->data.at(0).price,               0.9f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(1).timestamp,             300);
    ASSERT_EQ(stocks.at(1)->data.at(1).quantity,              3);
    ASSERT_NEAR(stocks.at(1)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(2).timestamp,             600);
    ASSERT_EQ(stocks.at(1)->data.at(2).quantity,              6);
    ASSERT_NEAR(stocks.at(1)->data.at(2).price,               0.6f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(3).timestamp,             700);
    ASSERT_EQ(stocks.at(1)->data.at(3).quantity,              7);
    ASSERT_NEAR(stocks.at(1)->data.at(3).price,               0.1f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.instrumentId,                "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(2)->meta.instrumentTicker,            "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.instrumentName,              "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,         true);
    ASSERT_NEAR(stocks.at(2)->meta.minPriceIncrement,         0.001f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->meta.pricePrecision,              3);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,  500);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(2)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(2)->data.size(),                      4);
    ASSERT_EQ(stocks.at(2)->data.at(0).timestamp,             100);
    ASSERT_EQ(stocks.at(2)->data.at(0).quantity,              1);
    ASSERT_NEAR(stocks.at(2)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(1).timestamp,             300);
    ASSERT_EQ(stocks.at(2)->data.at(1).quantity,              3);
    ASSERT_NEAR(stocks.at(2)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(2).timestamp,             400);
    ASSERT_EQ(stocks.at(2)->data.at(2).quantity,              4);
    ASSERT_NEAR(stocks.at(2)->data.at(2).price,               0.4f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(3).timestamp,             500);
    ASSERT_EQ(stocks.at(2)->data.at(3).quantity,              5);
    ASSERT_NEAR(stocks.at(2)->data.at(3).price,               0.2f, 0.0001f);
    // clang-format on

    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.3f));
    EXPECT_CALL(*userStorageMock, readUnlock());

    storage->obtainPayback(200);

    // clang-format off
    ASSERT_EQ(stocks.size(),                                  3);
    ASSERT_EQ(stocks.at(0)->meta.instrumentId,                "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa");
    ASSERT_EQ(stocks.at(0)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(0)->meta.instrumentTicker,            "TEST");
    ASSERT_EQ(stocks.at(0)->meta.instrumentName,              "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_NEAR(stocks.at(0)->meta.minPriceIncrement,         0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->meta.pricePrecision,              1);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  500);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      3);
    ASSERT_EQ(stocks.at(0)->data.at(0).timestamp,             100);
    ASSERT_EQ(stocks.at(0)->data.at(0).quantity,              1);
    ASSERT_NEAR(stocks.at(0)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(1).timestamp,             200);
    ASSERT_EQ(stocks.at(0)->data.at(1).quantity,              2);
    ASSERT_NEAR(stocks.at(0)->data.at(1).price,               0.9f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(2).timestamp,             500);
    ASSERT_EQ(stocks.at(0)->data.at(2).quantity,              5);
    ASSERT_NEAR(stocks.at(0)->data.at(2).price,               0.2f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.instrumentId,                "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb");
    ASSERT_EQ(stocks.at(1)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(1)->meta.instrumentTicker,            "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.instrumentName,              "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_NEAR(stocks.at(1)->meta.minPriceIncrement,         0.01f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->meta.pricePrecision,              2);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  700);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            16.66666f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      4);
    ASSERT_EQ(stocks.at(1)->data.at(0).timestamp,             200);
    ASSERT_EQ(stocks.at(1)->data.at(0).quantity,              2);
    ASSERT_NEAR(stocks.at(1)->data.at(0).price,               0.9f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(1).timestamp,             300);
    ASSERT_EQ(stocks.at(1)->data.at(1).quantity,              3);
    ASSERT_NEAR(stocks.at(1)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(2).timestamp,             600);
    ASSERT_EQ(stocks.at(1)->data.at(2).quantity,              6);
    ASSERT_NEAR(stocks.at(1)->data.at(2).price,               0.6f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(3).timestamp,             700);
    ASSERT_EQ(stocks.at(1)->data.at(3).quantity,              7);
    ASSERT_NEAR(stocks.at(1)->data.at(3).price,               0.1f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.instrumentId,                "cccccccc-cccc-cccc-cccc-cccccccccccc");
    ASSERT_EQ(stocks.at(2)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(2)->meta.instrumentTicker,            "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.instrumentName,              "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,         true);
    ASSERT_NEAR(stocks.at(2)->meta.minPriceIncrement,         0.001f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->meta.pricePrecision,              3);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,  500);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(2)->operational.payback,            66.66666f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(2)->data.size(),                      4);
    ASSERT_EQ(stocks.at(2)->data.at(0).timestamp,             100);
    ASSERT_EQ(stocks.at(2)->data.at(0).quantity,              1);
    ASSERT_NEAR(stocks.at(2)->data.at(0).price,               0.1f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(1).timestamp,             300);
    ASSERT_EQ(stocks.at(2)->data.at(1).quantity,              3);
    ASSERT_NEAR(stocks.at(2)->data.at(1).price,               0.3f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(2).timestamp,             400);
    ASSERT_EQ(stocks.at(2)->data.at(2).quantity,              4);
    ASSERT_NEAR(stocks.at(2)->data.at(2).price,               0.4f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(3).timestamp,             500);
    ASSERT_EQ(stocks.at(2)->data.at(3).quantity,              5);
    ASSERT_NEAR(stocks.at(2)->data.at(3).price,               0.2f, 0.0001f);
    // clang-format on
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity, readability-function-size, readability-magic-numbers)
