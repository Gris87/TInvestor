#include "src/threads/detectshorts/detectshortsthread.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/stocks/istocksstorage_mock.h"



using ::testing::Ge;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_DetectShortsThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        stocksStorageMock = new StrictMock<StocksStorageMock>();
        grpcClientMock    = new StrictMock<GrpcClientMock>();

        thread = new DetectShortsThread(stocksStorageMock, grpcClientMock);
    }

    void TearDown() override
    {
        delete thread;
        delete stocksStorageMock;
        delete grpcClientMock;
    }

    DetectShortsThread*            thread;
    StrictMock<StocksStorageMock>* stocksStorageMock;
    StrictMock<GrpcClientMock>*    grpcClientMock;
};



TEST_F(Test_DetectShortsThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_DetectShortsThread, Test_run)
{
    const InSequence seq;

    const std::shared_ptr<tinkoff::SharesResponse> stocksResponse(new tinkoff::SharesResponse());
    tinkoff::Share*                                stockShare1 = stocksResponse->add_instruments();
    tinkoff::Share*                                stockShare2 = stocksResponse->add_instruments();

    stockShare1->set_uid("aaaaa");
    stockShare1->set_short_enabled_flag(true);

    stockShare2->set_uid("bbbbb");
    stockShare2->set_short_enabled_flag(false);

    QList<Stock*> stocks;
    Stock         stock1;
    Stock         stock2;

    stock1.meta.instrumentId                = "aaaaa";
    stock1.meta.instrumentTicker            = "WAGA";
    stock1.meta.instrumentName              = "Wagadaka too";
    stock1.meta.forQualInvestorFlag         = true;
    stock1.meta.lot                         = 1;
    stock1.meta.pricePrecision              = 3;
    stock1.meta.minPriceIncrement           = 0.001f;
    stock1.meta.ignore                      = false;
    stock1.meta.lastTradeTime               = QTime(18, 39);
    stock1.meta.turnover                    = 5000;
    stock1.meta.dividends.createTimestamp   = 0;
    stock1.meta.dividends.paymentTimestamp  = 0;
    stock1.meta.dividends.price             = 0.0f;
    stock1.meta.dividends.yield             = 0.0f;
    stock1.meta.shorts.enabled              = false;
    stock1.meta.shorts.lastEnabledTimestamp = 0;

    stock2.meta.instrumentId                = "bbbbb";
    stock2.meta.instrumentTicker            = "BAZA";
    stock2.meta.instrumentName              = "Barazada";
    stock2.meta.forQualInvestorFlag         = false;
    stock2.meta.lot                         = 10;
    stock2.meta.pricePrecision              = 4;
    stock2.meta.minPriceIncrement           = 0.0001f;
    stock2.meta.ignore                      = false;
    stock2.meta.lastTradeTime               = QTime(23, 49);
    stock2.meta.turnover                    = 10000;
    stock2.meta.dividends.createTimestamp   = 0;
    stock2.meta.dividends.paymentTimestamp  = 0;
    stock2.meta.dividends.price             = 0.0f;
    stock2.meta.dividends.yield             = 0.0f;
    stock2.meta.shorts.enabled              = false;
    stock2.meta.shorts.lastEnabledTimestamp = 0;

    stocks << &stock1 << &stock2;

    // clang-format off
    ASSERT_EQ(stocks.size(),                                  2);
    ASSERT_EQ(stocks.at(0)->meta.instrumentId,                "aaaaa");
    ASSERT_EQ(stocks.at(0)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(0)->meta.instrumentTicker,            "WAGA");
    ASSERT_EQ(stocks.at(0)->meta.instrumentName,              "Wagadaka too");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(0)->meta.lot,                         1);
    ASSERT_EQ(stocks.at(0)->meta.pricePrecision,              3);
    ASSERT_NEAR(stocks.at(0)->meta.minPriceIncrement,         0.001f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->meta.ignore,                      false);
    ASSERT_EQ(stocks.at(0)->meta.lastTradeTime,               QTime(18, 39));
    ASSERT_EQ(stocks.at(0)->meta.turnover,                    5000);
    ASSERT_EQ(stocks.at(0)->meta.dividends.createTimestamp,   0);
    ASSERT_EQ(stocks.at(0)->meta.dividends.paymentTimestamp,  0);
    ASSERT_NEAR(stocks.at(0)->meta.dividends.price,           0.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->meta.dividends.yield,           0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->meta.shorts.enabled,              false);
    ASSERT_EQ(stocks.at(0)->meta.shorts.lastEnabledTimestamp, 0);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  0);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      0);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.instrumentId,                "bbbbb");
    ASSERT_EQ(stocks.at(1)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(1)->meta.instrumentTicker,            "BAZA");
    ASSERT_EQ(stocks.at(1)->meta.instrumentName,              "Barazada");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_EQ(stocks.at(1)->meta.lot,                         10);
    ASSERT_EQ(stocks.at(1)->meta.pricePrecision,              4);
    ASSERT_NEAR(stocks.at(1)->meta.minPriceIncrement,         0.0001f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->meta.ignore,                      false);
    ASSERT_EQ(stocks.at(1)->meta.lastTradeTime,               QTime(23, 49));
    ASSERT_EQ(stocks.at(1)->meta.turnover,                    10000);
    ASSERT_EQ(stocks.at(1)->meta.dividends.createTimestamp,   0);
    ASSERT_EQ(stocks.at(1)->meta.dividends.paymentTimestamp,  0);
    ASSERT_NEAR(stocks.at(1)->meta.dividends.price,           0.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->meta.dividends.yield,           0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->meta.shorts.enabled,              false);
    ASSERT_EQ(stocks.at(1)->meta.shorts.lastEnabledTimestamp, 0);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  0);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      0);
    // clang-format on

    EXPECT_CALL(*grpcClientMock, findStocks(QThread::currentThread(), tinkoff::INSTRUMENT_STATUS_BASE))
        .WillOnce(Return(stocksResponse));
    EXPECT_CALL(*stocksStorageMock, readLock());
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*stocksStorageMock, readUnlock());
    EXPECT_CALL(*stocksStorageMock, readLock());
    EXPECT_CALL(*stocksStorageMock, writeStocksMeta());
    EXPECT_CALL(*stocksStorageMock, readUnlock());

    thread->run();

    // clang-format off
    ASSERT_EQ(stocks.size(),                                  2);
    ASSERT_EQ(stocks.at(0)->meta.instrumentId,                "aaaaa");
    ASSERT_EQ(stocks.at(0)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(0)->meta.instrumentTicker,            "WAGA");
    ASSERT_EQ(stocks.at(0)->meta.instrumentName,              "Wagadaka too");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(0)->meta.lot,                         1);
    ASSERT_EQ(stocks.at(0)->meta.pricePrecision,              3);
    ASSERT_NEAR(stocks.at(0)->meta.minPriceIncrement,         0.001f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->meta.ignore,                      false);
    ASSERT_EQ(stocks.at(0)->meta.lastTradeTime,               QTime(18, 39));
    ASSERT_EQ(stocks.at(0)->meta.turnover,                    5000);
    ASSERT_EQ(stocks.at(0)->meta.dividends.createTimestamp,   0);
    ASSERT_EQ(stocks.at(0)->meta.dividends.paymentTimestamp,  0);
    ASSERT_NEAR(stocks.at(0)->meta.dividends.price,           0.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->meta.dividends.yield,           0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->meta.shorts.enabled,              true);
    ASSERT_GT(stocks.at(0)->meta.shorts.lastEnabledTimestamp, 1704056400000);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  0);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      0);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.instrumentId,                "bbbbb");
    ASSERT_EQ(stocks.at(1)->meta.instrumentLogo,              nullptr);
    ASSERT_EQ(stocks.at(1)->meta.instrumentTicker,            "BAZA");
    ASSERT_EQ(stocks.at(1)->meta.instrumentName,              "Barazada");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_EQ(stocks.at(1)->meta.lot,                         10);
    ASSERT_EQ(stocks.at(1)->meta.pricePrecision,              4);
    ASSERT_NEAR(stocks.at(1)->meta.minPriceIncrement,         0.0001f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->meta.ignore,                      false);
    ASSERT_EQ(stocks.at(1)->meta.lastTradeTime,               QTime(23, 49));
    ASSERT_EQ(stocks.at(1)->meta.turnover,                    10000);
    ASSERT_EQ(stocks.at(1)->meta.dividends.createTimestamp,   0);
    ASSERT_EQ(stocks.at(1)->meta.dividends.paymentTimestamp,  0);
    ASSERT_NEAR(stocks.at(1)->meta.dividends.price,           0.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->meta.dividends.yield,           0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->meta.shorts.enabled,              false);
    ASSERT_EQ(stocks.at(1)->meta.shorts.lastEnabledTimestamp, 0);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  0);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      0);
    // clang-format on
}

TEST_F(Test_DetectShortsThread, Test_terminateThread)
{
    thread->terminateThread();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
