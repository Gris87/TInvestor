#include "src/threads/portfoliolastprice/portfoliolastpricethread.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"
#include "src/utils/timeutils/itimeutils_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



class Test_PortfolioLastPriceThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        timeUtilsMock  = new StrictMock<TimeUtilsMock>();
        grpcClientMock = new StrictMock<GrpcClientMock>();

        thread = new PortfolioLastPriceThread(timeUtilsMock, grpcClientMock);
    }

    void TearDown() override
    {
        delete thread;
        delete timeUtilsMock;
        delete grpcClientMock;
    }

    PortfolioLastPriceThread*   thread;
    StrictMock<TimeUtilsMock>*  timeUtilsMock;
    StrictMock<GrpcClientMock>* grpcClientMock;
};



TEST_F(Test_PortfolioLastPriceThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_PortfolioLastPriceThread, Test_run)
{
    const InSequence seq;

    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(true));

    thread->run();

    Portfolio             portfolio;
    PortfolioCategoryItem category;
    PortfolioItem         item;

    item.instrumentId = "aaaaa";
    item.showPrices   = true;

    category.items << item;
    portfolio.positions << category;

    thread->portfolioChanged(portfolio);

    std::shared_ptr<MarketDataStream> marketDataStream(new MarketDataStream());

    const std::shared_ptr<tinkoff::MarketDataResponse> marketDataResponse(new tinkoff::MarketDataResponse());

    tinkoff::LastPrice* lastPrice = new tinkoff::LastPrice(); // marketDataResponse will take ownership
    tinkoff::Quotation* price     = new tinkoff::Quotation(); // marketDataResponse will take ownership

    price->set_units(100);
    price->set_nano(500000000);

    google::protobuf::Timestamp* time = new google::protobuf::Timestamp(); // marketDataResponse will take ownership

    time->set_seconds(1000);
    time->set_nanos(123000000);

    lastPrice->set_allocated_price(price);
    lastPrice->set_allocated_time(time);
    lastPrice->set_instrument_uid("aaaaa");

    marketDataResponse->set_allocated_last_price(lastPrice);

    EXPECT_CALL(*grpcClientMock, createMarketDataStreamForLastPrice(QStringList() << "aaaaa")).WillOnce(Return(marketDataStream));
    EXPECT_CALL(*grpcClientMock, readMarketDataStream(marketDataStream)).WillOnce(Return(marketDataResponse));
    EXPECT_CALL(*grpcClientMock, readMarketDataStream(marketDataStream)).WillOnce(Return(nullptr));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*grpcClientMock, finishMarketDataStream(marketDataStream));
    EXPECT_CALL(*grpcClientMock, createMarketDataStreamForLastPrice(QStringList() << "aaaaa")).WillOnce(Return(nullptr));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(true));

    thread->run();
}

TEST_F(Test_PortfolioLastPriceThread, Test_portfolioChanged)
{
    const InSequence seq;

    std::shared_ptr<MarketDataStream> marketDataStream(new MarketDataStream());
    EXPECT_CALL(*grpcClientMock, createMarketDataStreamForLastPrice(QStringList())).WillOnce(Return(marketDataStream));

    ASSERT_EQ(thread->createMarketDataStream(), true);

    Portfolio             portfolio;
    PortfolioCategoryItem category;
    PortfolioItem         item;

    item.instrumentId = "aaaaa";
    item.showPrices   = true;

    category.items << item;
    portfolio.positions << category;

    EXPECT_CALL(*grpcClientMock, cancelMarketDataStream(marketDataStream));

    thread->portfolioChanged(portfolio);
}

TEST_F(Test_PortfolioLastPriceThread, Test_terminateThread)
{
    const InSequence seq;

    std::shared_ptr<MarketDataStream> marketDataStream(new MarketDataStream());
    EXPECT_CALL(*grpcClientMock, createMarketDataStreamForLastPrice(QStringList())).WillOnce(Return(marketDataStream));

    ASSERT_EQ(thread->createMarketDataStream(), true);

    EXPECT_CALL(*grpcClientMock, cancelMarketDataStream(marketDataStream));

    thread->terminateThread();
}
