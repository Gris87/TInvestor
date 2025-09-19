#include "src/threads/portfolio/portfoliothread.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"
#include "src/grpc/igrpcretryclient_mock.h"
#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/logos/ilogosstorage_mock.h"
#include "src/utils/timeutils/itimeutils_mock.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";



using ::testing::InSequence;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



class Test_PortfolioThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        instrumentsStorageMock = new StrictMock<InstrumentsStorageMock>();
        logosStorageMock       = new StrictMock<LogosStorageMock>();
        timeUtilsMock          = new StrictMock<TimeUtilsMock>();
        grpcClientMock         = new StrictMock<GrpcClientMock>();
        grpcRetryClientMock    = new StrictMock<GrpcRetryClientMock>();

        thread =
            new PortfolioThread(instrumentsStorageMock, logosStorageMock, timeUtilsMock, grpcClientMock, grpcRetryClientMock);
    }

    void TearDown() override
    {
        delete thread;
        delete instrumentsStorageMock;
        delete logosStorageMock;
        delete timeUtilsMock;
        delete grpcClientMock;
        delete grpcRetryClientMock;
    }

    PortfolioThread*                    thread;
    StrictMock<InstrumentsStorageMock>* instrumentsStorageMock;
    StrictMock<LogosStorageMock>*       logosStorageMock;
    StrictMock<TimeUtilsMock>*          timeUtilsMock;
    StrictMock<GrpcClientMock>*         grpcClientMock;
    StrictMock<GrpcRetryClientMock>*    grpcRetryClientMock;
};



TEST_F(Test_PortfolioThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_PortfolioThread, Test_run)
{
    const InSequence seq;

    thread->setAccountId("account-id");

    Instruments instruments;
    Logo        logo;

    std::shared_ptr<PortfolioStream> portfolioStream(new PortfolioStream());

    const std::shared_ptr<tinkoff::PortfolioResponse> portfolioResponse(new tinkoff::PortfolioResponse());
    tinkoff::PortfolioPosition* position = portfolioResponse->add_positions(); // portfolioResponse will take ownership

    tinkoff::Quotation*  tinkoffQuantity     = new tinkoff::Quotation();  // position will take ownership
    tinkoff::MoneyValue* tinkoffCurrentPrice = new tinkoff::MoneyValue(); // position will take ownership
    tinkoff::MoneyValue* tinkoffAvgPriceFifo = new tinkoff::MoneyValue(); // position will take ownership
    tinkoff::MoneyValue* tinkoffAvgPriceWavg = new tinkoff::MoneyValue(); // position will take ownership
    tinkoff::MoneyValue* tinkoffDailyYield   = new tinkoff::MoneyValue(); // position will take ownership

    tinkoffQuantity->set_units(100000);
    tinkoffQuantity->set_nano(0);

    tinkoffCurrentPrice->set_currency("rub");
    tinkoffCurrentPrice->set_units(1);
    tinkoffCurrentPrice->set_nano(0);

    tinkoffAvgPriceFifo->set_currency("rub");
    tinkoffAvgPriceFifo->set_units(1);
    tinkoffAvgPriceFifo->set_nano(0);

    tinkoffAvgPriceWavg->set_currency("rub");
    tinkoffAvgPriceWavg->set_units(1);
    tinkoffAvgPriceWavg->set_nano(0);

    tinkoffDailyYield->set_currency("rub");
    tinkoffDailyYield->set_units(0);
    tinkoffDailyYield->set_nano(0);

    position->set_instrument_uid(RUBLE_UID);
    position->set_instrument_type("currency");
    position->set_allocated_quantity(tinkoffQuantity);
    position->set_allocated_current_price(tinkoffCurrentPrice);
    position->set_allocated_average_position_price_fifo(tinkoffAvgPriceFifo);
    position->set_allocated_average_position_price(tinkoffAvgPriceWavg);
    position->set_allocated_daily_yield(tinkoffDailyYield);

    const std::shared_ptr<tinkoff::PortfolioStreamResponse> portfolioStreamResponse(new tinkoff::PortfolioStreamResponse());
    tinkoff::PortfolioResponse*                             portfolioResponse2 =
        new tinkoff::PortfolioResponse(); // portfolioStreamResponse will take ownership

    portfolioStreamResponse->set_allocated_portfolio(portfolioResponse2);

    EXPECT_CALL(*grpcClientMock, createPortfolioStream(QString("account-id"))).WillOnce(Return(portfolioStream));
    EXPECT_CALL(*grpcRetryClientMock, getValidPortfolio(QThread::currentThread(), QString("account-id")))
        .WillOnce(Return(portfolioResponse));
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*logosStorageMock, getLogo(QString(RUBLE_UID))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*grpcClientMock, readPortfolioStream(portfolioStream)).WillOnce(Return(portfolioStreamResponse));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(10000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*grpcRetryClientMock, getValidPortfolio(QThread::currentThread(), QString("account-id")))
        .WillOnce(Return(nullptr));
    EXPECT_CALL(*grpcClientMock, readPortfolioStream(portfolioStream)).WillOnce(Return(nullptr));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*grpcClientMock, finishPortfolioStream(portfolioStream));
    EXPECT_CALL(*grpcClientMock, createPortfolioStream(QString("account-id"))).WillOnce(Return(portfolioStream));
    EXPECT_CALL(*grpcRetryClientMock, getValidPortfolio(QThread::currentThread(), QString("account-id")))
        .WillOnce(Return(nullptr));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(true));
    EXPECT_CALL(*grpcClientMock, finishPortfolioStream(portfolioStream));

    thread->run();

    std::shared_ptr<PortfolioStream> portfolioStream2(new PortfolioStream());

    const std::shared_ptr<tinkoff::PortfolioResponse> portfolioResponse3(new tinkoff::PortfolioResponse());
    tinkoff::PortfolioPosition* position3 = portfolioResponse3->add_positions(); // portfolioResponse3 will take ownership

    tinkoff::Quotation*  tinkoffQuantity3     = new tinkoff::Quotation();  // position3 will take ownership
    tinkoff::MoneyValue* tinkoffCurrentPrice3 = new tinkoff::MoneyValue(); // position3 will take ownership
    tinkoff::MoneyValue* tinkoffAvgPriceFifo3 = new tinkoff::MoneyValue(); // position3 will take ownership
    tinkoff::MoneyValue* tinkoffAvgPriceWavg3 = new tinkoff::MoneyValue(); // position3 will take ownership
    tinkoff::MoneyValue* tinkoffDailyYield3   = new tinkoff::MoneyValue(); // position3 will take ownership

    tinkoffQuantity3->set_units(100000);
    tinkoffQuantity3->set_nano(0);

    tinkoffCurrentPrice3->set_currency("rub");
    tinkoffCurrentPrice3->set_units(1);
    tinkoffCurrentPrice3->set_nano(0);

    tinkoffAvgPriceFifo3->set_currency("rub");
    tinkoffAvgPriceFifo3->set_units(1);
    tinkoffAvgPriceFifo3->set_nano(0);

    tinkoffAvgPriceWavg3->set_currency("rub");
    tinkoffAvgPriceWavg3->set_units(1);
    tinkoffAvgPriceWavg3->set_nano(0);

    tinkoffDailyYield3->set_currency("rub");
    tinkoffDailyYield3->set_units(0);
    tinkoffDailyYield3->set_nano(0);

    position3->set_instrument_uid("bbb-bbb");
    position3->set_instrument_type("share");
    position3->set_allocated_quantity(tinkoffQuantity3);
    position3->set_allocated_current_price(tinkoffCurrentPrice3);
    position3->set_allocated_average_position_price_fifo(tinkoffAvgPriceFifo3);
    position3->set_allocated_average_position_price(tinkoffAvgPriceWavg3);
    position3->set_allocated_daily_yield(tinkoffDailyYield3);

    const std::shared_ptr<tinkoff::PortfolioStreamResponse> portfolioStreamResponse2(new tinkoff::PortfolioStreamResponse());
    tinkoff::PortfolioResponse*                             portfolioResponse4 =
        new tinkoff::PortfolioResponse(); // portfolioStreamResponse2 will take ownership

    portfolioStreamResponse2->set_allocated_portfolio(portfolioResponse4);

    EXPECT_CALL(*grpcClientMock, createPortfolioStream(QString("account-id"))).WillOnce(Return(portfolioStream2));
    EXPECT_CALL(*grpcRetryClientMock, getValidPortfolio(QThread::currentThread(), QString("account-id")))
        .WillOnce(Return(portfolioResponse3));
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*logosStorageMock, getLogo(QString("bbb-bbb"))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*grpcClientMock, readPortfolioStream(portfolioStream2)).WillOnce(Return(portfolioStreamResponse2));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(10000, QThread::currentThread())).WillOnce(Return(true));
    EXPECT_CALL(*grpcClientMock, finishPortfolioStream(portfolioStream2));
    EXPECT_CALL(*grpcClientMock, createPortfolioStream(QString("account-id"))).WillOnce(Return(nullptr));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(true));

    thread->run();
}

TEST_F(Test_PortfolioThread, Test_terminateThread)
{
    const InSequence seq;

    thread->setAccountId("account-id");

    std::shared_ptr<PortfolioStream> portfolioStream(new PortfolioStream());
    EXPECT_CALL(*grpcClientMock, createPortfolioStream(QString("account-id"))).WillOnce(Return(portfolioStream));

    ASSERT_EQ(thread->createPortfolioStream(), true);

    EXPECT_CALL(*grpcClientMock, cancelPortfolioStream(portfolioStream));

    thread->terminateThread();
}
