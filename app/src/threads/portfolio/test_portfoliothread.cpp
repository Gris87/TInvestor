#include "src/threads/portfolio/portfoliothread.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/logos/ilogosstorage_mock.h"



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
        grpcClientMock         = new StrictMock<GrpcClientMock>();

        thread = new PortfolioThread(instrumentsStorageMock, logosStorageMock, grpcClientMock);
    }

    void TearDown() override
    {
        delete thread;
        delete instrumentsStorageMock;
        delete logosStorageMock;
        delete grpcClientMock;
    }

    PortfolioThread*                    thread;
    StrictMock<InstrumentsStorageMock>* instrumentsStorageMock;
    StrictMock<LogosStorageMock>*       logosStorageMock;
    StrictMock<GrpcClientMock>*         grpcClientMock;
};



TEST_F(Test_PortfolioThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_PortfolioThread, Test_run)
{
    const InSequence seq;

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

    Instruments instruments;
    Logo        logo;

    std::shared_ptr<PortfolioStream> portfolioStream(new PortfolioStream());

    const std::shared_ptr<tinkoff::PortfolioStreamResponse> portfolioStreamResponse(new tinkoff::PortfolioStreamResponse());
    tinkoff::PortfolioResponse*                             portfolioResponse2 =
        new tinkoff::PortfolioResponse(); // portfolioStreamResponse will take ownership

    tinkoff::PortfolioPosition* position2 = portfolioResponse2->add_positions(); // portfolioResponse will take ownership

    tinkoff::Quotation*  tinkoffQuantity2     = new tinkoff::Quotation();  // position will take ownership
    tinkoff::MoneyValue* tinkoffCurrentPrice2 = new tinkoff::MoneyValue(); // position will take ownership
    tinkoff::MoneyValue* tinkoffAvgPriceFifo2 = new tinkoff::MoneyValue(); // position will take ownership
    tinkoff::MoneyValue* tinkoffAvgPriceWavg2 = new tinkoff::MoneyValue(); // position will take ownership
    tinkoff::MoneyValue* tinkoffDailyYield2   = new tinkoff::MoneyValue(); // position will take ownership

    tinkoffQuantity2->set_units(100000);
    tinkoffQuantity2->set_nano(0);

    tinkoffCurrentPrice2->set_currency("rub");
    tinkoffCurrentPrice2->set_units(1);
    tinkoffCurrentPrice2->set_nano(0);

    tinkoffAvgPriceFifo2->set_currency("rub");
    tinkoffAvgPriceFifo2->set_units(1);
    tinkoffAvgPriceFifo2->set_nano(0);

    tinkoffAvgPriceWavg2->set_currency("rub");
    tinkoffAvgPriceWavg2->set_units(1);
    tinkoffAvgPriceWavg2->set_nano(0);

    tinkoffDailyYield2->set_currency("rub");
    tinkoffDailyYield2->set_units(0);
    tinkoffDailyYield2->set_nano(0);

    position2->set_instrument_uid("bbb-bbb");
    position2->set_instrument_type("share");
    position2->set_allocated_quantity(tinkoffQuantity2);
    position2->set_allocated_current_price(tinkoffCurrentPrice2);
    position2->set_allocated_average_position_price_fifo(tinkoffAvgPriceFifo2);
    position2->set_allocated_average_position_price(tinkoffAvgPriceWavg2);
    position2->set_allocated_daily_yield(tinkoffDailyYield2);

    portfolioStreamResponse->set_allocated_portfolio(portfolioResponse2);

    EXPECT_CALL(*grpcClientMock, getPortfolio(QThread::currentThread(), QString("aaaaa"))).WillOnce(Return(portfolioResponse));
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*logosStorageMock, getLogo(QString(RUBLE_UID))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*grpcClientMock, createPortfolioStream(QString("aaaaa"))).WillOnce(Return(portfolioStream));
    EXPECT_CALL(*grpcClientMock, readPortfolioStream(portfolioStream)).WillOnce(Return(portfolioStreamResponse));
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*logosStorageMock, getLogo(QString("bbb-bbb"))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*grpcClientMock, readPortfolioStream(portfolioStream)).WillOnce(Return(nullptr));
    EXPECT_CALL(*grpcClientMock, finishPortfolioStream(portfolioStream));

    thread->setAccountId("aaaaa");
    thread->run();
}

TEST_F(Test_PortfolioThread, Test_terminateThread)
{
    const InSequence seq;

    std::shared_ptr<PortfolioStream> portfolioStream(new PortfolioStream());
    EXPECT_CALL(*grpcClientMock, createPortfolioStream(QString("aaaaa"))).WillOnce(Return(portfolioStream));

    thread->setAccountId("aaaaa");
    thread->createPortfolioStream();

    EXPECT_CALL(*grpcClientMock, cancelPortfolioStream(portfolioStream));

    thread->terminateThread();
}
