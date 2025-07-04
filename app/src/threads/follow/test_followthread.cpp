#include "src/threads/follow/followthread.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/instruments/iinstrumentsstorage_mock.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";



using ::testing::InSequence;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



class Test_FollowThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        instrumentsStorageMock = new StrictMock<InstrumentsStorageMock>();
        grpcClientMock         = new StrictMock<GrpcClientMock>();

        thread = new FollowThread(instrumentsStorageMock, grpcClientMock);
    }

    void TearDown() override
    {
        delete thread;
        delete instrumentsStorageMock;
        delete grpcClientMock;
    }

    FollowThread*                       thread;
    StrictMock<InstrumentsStorageMock>* instrumentsStorageMock;
    StrictMock<GrpcClientMock>*         grpcClientMock;
};



TEST_F(Test_FollowThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_FollowThread, Test_run)
{
    const InSequence seq;

    thread->setAccounts("account-id", "another-account-id", "Bankrupt account");

    const std::shared_ptr<tinkoff::PortfolioResponse> portfolioResponse1(new tinkoff::PortfolioResponse());
    const std::shared_ptr<tinkoff::PortfolioResponse> portfolioResponse2(new tinkoff::PortfolioResponse());

    tinkoff::PortfolioPosition* position1 = portfolioResponse1->add_positions(); // portfolioResponse1 will take ownership
    tinkoff::PortfolioPosition* position2 = portfolioResponse1->add_positions(); // portfolioResponse1 will take ownership
    tinkoff::PortfolioPosition* position3 = portfolioResponse2->add_positions(); // portfolioResponse2 will take ownership
    tinkoff::PortfolioPosition* position4 = portfolioResponse2->add_positions(); // portfolioResponse2 will take ownership

    tinkoff::Quotation*  tinkoffQuantity1     = new tinkoff::Quotation();  // position1 will take ownership
    tinkoff::MoneyValue* tinkoffCurrentPrice1 = new tinkoff::MoneyValue(); // position1 will take ownership
    tinkoff::MoneyValue* tinkoffAvgPriceFifo1 = new tinkoff::MoneyValue(); // position1 will take ownership

    tinkoff::Quotation*  tinkoffQuantity2     = new tinkoff::Quotation();  // position2 will take ownership
    tinkoff::MoneyValue* tinkoffCurrentPrice2 = new tinkoff::MoneyValue(); // position2 will take ownership
    tinkoff::MoneyValue* tinkoffAvgPriceFifo2 = new tinkoff::MoneyValue(); // position2 will take ownership

    tinkoff::Quotation*  tinkoffQuantity3     = new tinkoff::Quotation();  // position3 will take ownership
    tinkoff::MoneyValue* tinkoffCurrentPrice3 = new tinkoff::MoneyValue(); // position3 will take ownership
    tinkoff::MoneyValue* tinkoffAvgPriceFifo3 = new tinkoff::MoneyValue(); // position3 will take ownership

    tinkoff::Quotation*  tinkoffQuantity4     = new tinkoff::Quotation();  // position4 will take ownership
    tinkoff::MoneyValue* tinkoffCurrentPrice4 = new tinkoff::MoneyValue(); // position4 will take ownership
    tinkoff::MoneyValue* tinkoffAvgPriceFifo4 = new tinkoff::MoneyValue(); // position4 will take ownership

    tinkoffQuantity1->set_units(10000);
    tinkoffQuantity1->set_nano(0);

    tinkoffCurrentPrice1->set_currency("rub");
    tinkoffCurrentPrice1->set_units(5);
    tinkoffCurrentPrice1->set_nano(100000000);

    tinkoffAvgPriceFifo1->set_currency("rub");
    tinkoffAvgPriceFifo1->set_units(50);
    tinkoffAvgPriceFifo1->set_nano(500000000);

    position1->set_instrument_uid(RUBLE_UID);
    position1->set_allocated_quantity(tinkoffQuantity1);
    position1->set_allocated_current_price(tinkoffCurrentPrice1);
    position1->set_allocated_average_position_price_fifo(tinkoffAvgPriceFifo1);

    tinkoffQuantity2->set_units(10000);
    tinkoffQuantity2->set_nano(0);

    tinkoffCurrentPrice2->set_currency("rub");
    tinkoffCurrentPrice2->set_units(5);
    tinkoffCurrentPrice2->set_nano(100000000);

    tinkoffAvgPriceFifo2->set_currency("rub");
    tinkoffAvgPriceFifo2->set_units(50);
    tinkoffAvgPriceFifo2->set_nano(500000000);

    position2->set_instrument_uid("bbbbb");
    position2->set_allocated_quantity(tinkoffQuantity2);
    position2->set_allocated_current_price(tinkoffCurrentPrice2);
    position2->set_allocated_average_position_price_fifo(tinkoffAvgPriceFifo2);

    tinkoffQuantity3->set_units(30000);
    tinkoffQuantity3->set_nano(0);

    tinkoffCurrentPrice3->set_currency("rub");
    tinkoffCurrentPrice3->set_units(5);
    tinkoffCurrentPrice3->set_nano(300000000);

    tinkoffAvgPriceFifo3->set_currency("rub");
    tinkoffAvgPriceFifo3->set_units(50);
    tinkoffAvgPriceFifo3->set_nano(500000000);

    position3->set_instrument_uid(RUBLE_UID);
    position3->set_allocated_quantity(tinkoffQuantity3);
    position3->set_allocated_current_price(tinkoffCurrentPrice3);
    position3->set_allocated_average_position_price_fifo(tinkoffAvgPriceFifo3);

    tinkoffQuantity4->set_units(50000);
    tinkoffQuantity4->set_nano(0);

    tinkoffCurrentPrice4->set_currency("rub");
    tinkoffCurrentPrice4->set_units(5);
    tinkoffCurrentPrice4->set_nano(100000000);

    tinkoffAvgPriceFifo4->set_currency("rub");
    tinkoffAvgPriceFifo4->set_units(50);
    tinkoffAvgPriceFifo4->set_nano(500000000);

    position4->set_instrument_uid("aaaaa");
    position4->set_allocated_quantity(tinkoffQuantity4);
    position4->set_allocated_current_price(tinkoffCurrentPrice4);
    position4->set_allocated_average_position_price_fifo(tinkoffAvgPriceFifo4);

    std::shared_ptr<PortfolioStream> portfolioStream(new PortfolioStream());

    const std::shared_ptr<tinkoff::PortfolioStreamResponse> portfolioStreamResponse1(new tinkoff::PortfolioStreamResponse());
    const std::shared_ptr<tinkoff::PortfolioStreamResponse> portfolioStreamResponse2(new tinkoff::PortfolioStreamResponse());

    tinkoff::PortfolioResponse* portfolioResponse3 =
        new tinkoff::PortfolioResponse(); // portfolioStreamResponse1 will take ownership
    tinkoff::PortfolioResponse* portfolioResponse4 =
        new tinkoff::PortfolioResponse(); // portfolioStreamResponse2 will take ownership

    portfolioResponse3->set_account_id("account-id");
    portfolioResponse4->set_account_id("another-account-id");

    tinkoff::PortfolioPosition* position5 = portfolioResponse3->add_positions(); // portfolioResponse3 will take ownership
    tinkoff::PortfolioPosition* position6 = portfolioResponse3->add_positions(); // portfolioResponse3 will take ownership
    tinkoff::PortfolioPosition* position7 = portfolioResponse4->add_positions(); // portfolioResponse4 will take ownership
    tinkoff::PortfolioPosition* position8 = portfolioResponse4->add_positions(); // portfolioResponse4 will take ownership

    tinkoff::Quotation*  tinkoffQuantity5     = new tinkoff::Quotation();  // position5 will take ownership
    tinkoff::MoneyValue* tinkoffCurrentPrice5 = new tinkoff::MoneyValue(); // position5 will take ownership
    tinkoff::MoneyValue* tinkoffAvgPriceFifo5 = new tinkoff::MoneyValue(); // position5 will take ownership

    tinkoff::Quotation*  tinkoffQuantity6     = new tinkoff::Quotation();  // position6 will take ownership
    tinkoff::MoneyValue* tinkoffCurrentPrice6 = new tinkoff::MoneyValue(); // position6 will take ownership
    tinkoff::MoneyValue* tinkoffAvgPriceFifo6 = new tinkoff::MoneyValue(); // position6 will take ownership

    tinkoff::Quotation*  tinkoffQuantity7     = new tinkoff::Quotation();  // position7 will take ownership
    tinkoff::MoneyValue* tinkoffCurrentPrice7 = new tinkoff::MoneyValue(); // position7 will take ownership
    tinkoff::MoneyValue* tinkoffAvgPriceFifo7 = new tinkoff::MoneyValue(); // position7 will take ownership

    tinkoff::Quotation*  tinkoffQuantity8     = new tinkoff::Quotation();  // position8 will take ownership
    tinkoff::MoneyValue* tinkoffCurrentPrice8 = new tinkoff::MoneyValue(); // position8 will take ownership
    tinkoff::MoneyValue* tinkoffAvgPriceFifo8 = new tinkoff::MoneyValue(); // position8 will take ownership

    tinkoffQuantity5->set_units(50000);
    tinkoffQuantity5->set_nano(0);

    tinkoffCurrentPrice5->set_currency("rub");
    tinkoffCurrentPrice5->set_units(5);
    tinkoffCurrentPrice5->set_nano(500000000);

    tinkoffAvgPriceFifo5->set_currency("rub");
    tinkoffAvgPriceFifo5->set_units(50);
    tinkoffAvgPriceFifo5->set_nano(500000000);

    position5->set_instrument_uid(RUBLE_UID);
    position5->set_allocated_quantity(tinkoffQuantity5);
    position5->set_allocated_current_price(tinkoffCurrentPrice5);
    position5->set_allocated_average_position_price_fifo(tinkoffAvgPriceFifo5);

    tinkoffQuantity6->set_units(10000);
    tinkoffQuantity6->set_nano(0);

    tinkoffCurrentPrice6->set_currency("rub");
    tinkoffCurrentPrice6->set_units(5);
    tinkoffCurrentPrice6->set_nano(100000000);

    tinkoffAvgPriceFifo6->set_currency("rub");
    tinkoffAvgPriceFifo6->set_units(50);
    tinkoffAvgPriceFifo6->set_nano(500000000);

    position6->set_instrument_uid("aaaaa");
    position6->set_allocated_quantity(tinkoffQuantity6);
    position6->set_allocated_current_price(tinkoffCurrentPrice6);
    position6->set_allocated_average_position_price_fifo(tinkoffAvgPriceFifo6);

    tinkoffQuantity7->set_units(70000);
    tinkoffQuantity7->set_nano(0);

    tinkoffCurrentPrice7->set_currency("rub");
    tinkoffCurrentPrice7->set_units(5);
    tinkoffCurrentPrice7->set_nano(700000000);

    tinkoffAvgPriceFifo7->set_currency("rub");
    tinkoffAvgPriceFifo7->set_units(50);
    tinkoffAvgPriceFifo7->set_nano(500000000);

    position7->set_instrument_uid(RUBLE_UID);
    position7->set_allocated_quantity(tinkoffQuantity7);
    position7->set_allocated_current_price(tinkoffCurrentPrice7);
    position7->set_allocated_average_position_price_fifo(tinkoffAvgPriceFifo7);

    tinkoffQuantity8->set_units(50);
    tinkoffQuantity8->set_nano(0);

    tinkoffCurrentPrice8->set_currency("rub");
    tinkoffCurrentPrice8->set_units(5);
    tinkoffCurrentPrice8->set_nano(100000000);

    tinkoffAvgPriceFifo8->set_currency("rub");
    tinkoffAvgPriceFifo8->set_units(50);
    tinkoffAvgPriceFifo8->set_nano(500000000);

    position8->set_instrument_uid("aaaaa");
    position8->set_allocated_quantity(tinkoffQuantity8);
    position8->set_allocated_current_price(tinkoffCurrentPrice8);
    position8->set_allocated_average_position_price_fifo(tinkoffAvgPriceFifo8);

    portfolioStreamResponse1->set_allocated_portfolio(portfolioResponse3);
    portfolioStreamResponse2->set_allocated_portfolio(portfolioResponse4);

    Instruments instruments;
    Instrument  instrument1;
    Instrument  instrument2;

    instrument1.lot = 10;
    instrument2.lot = 5;

    instruments["aaaaa"] = instrument1;
    instruments["bbbbb"] = instrument2;

    EXPECT_CALL(*grpcClientMock, getPortfolio(QThread::currentThread(), QString("account-id")))
        .WillOnce(Return(portfolioResponse1));
    EXPECT_CALL(*grpcClientMock, getPortfolio(QThread::currentThread(), QString("another-account-id")))
        .WillOnce(Return(portfolioResponse2));
    EXPECT_CALL(*grpcClientMock, createPortfolioStream(QString("account-id"), QString("another-account-id")))
        .WillOnce(Return(portfolioStream));
    EXPECT_CALL(*grpcClientMock, readPortfolioStream(portfolioStream)).WillOnce(Return(portfolioStreamResponse1));
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*grpcClientMock, readPortfolioStream(portfolioStream)).WillOnce(Return(portfolioStreamResponse2));
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*grpcClientMock, readPortfolioStream(portfolioStream)).WillOnce(Return(nullptr));
    EXPECT_CALL(*grpcClientMock, finishPortfolioStream(portfolioStream));

    thread->run();
}

TEST_F(Test_FollowThread, Test_terminateThread)
{
    const InSequence seq;

    thread->setAccounts("account-id", "another-account-id", "Bankrupt account");

    std::shared_ptr<PortfolioStream> portfolioStream(new PortfolioStream());
    EXPECT_CALL(*grpcClientMock, createPortfolioStream(QString("account-id"), QString("another-account-id")))
        .WillOnce(Return(portfolioStream));

    thread->createPortfolioStream();

    EXPECT_CALL(*grpcClientMock, cancelPortfolioStream(portfolioStream));

    thread->terminateThread();
}
