#include "src/grpc/grpcretryclient.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"
#include "src/utils/timeutils/itimeutils_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_GrpcRetryClient : public ::testing::Test
{
protected:
    void SetUp() override
    {
        grpcClientMock = new StrictMock<GrpcClientMock>();
        timeUtilsMock  = new StrictMock<TimeUtilsMock>();

        client = new GrpcRetryClient(grpcClientMock, timeUtilsMock);
    }

    void TearDown() override
    {
        delete client;
        delete grpcClientMock;
        delete timeUtilsMock;
    }

    GrpcRetryClient*            client;
    StrictMock<GrpcClientMock>* grpcClientMock;
    StrictMock<TimeUtilsMock>*  timeUtilsMock;
};



TEST_F(Test_GrpcRetryClient, Test_constructor_and_destructor)
{
}

TEST_F(Test_GrpcRetryClient, Test_getValidOperations)
{
    const InSequence seq;

    EXPECT_CALL(
        *grpcClientMock, getOperations(QThread::currentThread(), QString("account-id"), 1704056400000, 1704142800000, QString(""))
    )
        .WillOnce(Return(nullptr));

    ASSERT_EQ(client->getValidOperations(QThread::currentThread(), "account-id", 1704056400000, 1704142800000, ""), nullptr);

    const std::shared_ptr<tinkoff::GetOperationsByCursorResponse> getOperationsByCursorResponse(
        new tinkoff::GetOperationsByCursorResponse()
    );

    tinkoff::OperationItem* operationItem =
        getOperationsByCursorResponse->add_items(); // getOperationsByCursorResponse will take ownership

    operationItem->set_instrument_kind(tinkoff::INSTRUMENT_TYPE_SHARE);
    operationItem->set_type(tinkoff::OPERATION_TYPE_BUY);

    EXPECT_CALL(
        *grpcClientMock, getOperations(QThread::currentThread(), QString("account-id"), 1704056400000, 1704142800000, QString(""))
    )
        .WillOnce(Return(getOperationsByCursorResponse));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(1000, QThread::currentThread())).WillOnce(Return(true));

    ASSERT_EQ(client->getValidOperations(QThread::currentThread(), "account-id", 1704056400000, 1704142800000, ""), nullptr);

    tinkoff::MoneyValue* commission = new tinkoff::MoneyValue(); // operationItem will take ownership

    commission->set_units(-1);
    commission->set_nano(-266500000);

    operationItem->set_allocated_commission(commission);

    EXPECT_CALL(
        *grpcClientMock, getOperations(QThread::currentThread(), QString("account-id"), 1704056400000, 1704142800000, QString(""))
    )
        .WillOnce(Return(getOperationsByCursorResponse));

    ASSERT_NE(client->getValidOperations(QThread::currentThread(), "account-id", 1704056400000, 1704142800000, ""), nullptr);
}

TEST_F(Test_GrpcRetryClient, Test_getValidPortfolio)
{
    const InSequence seq;

    EXPECT_CALL(*grpcClientMock, getPortfolio(QThread::currentThread(), QString("account-id"))).WillOnce(Return(nullptr));

    ASSERT_EQ(client->getValidPortfolio(QThread::currentThread(), "account-id"), nullptr);

    const std::shared_ptr<tinkoff::PortfolioResponse> portfolioResponse(new tinkoff::PortfolioResponse());

    tinkoff::PortfolioPosition* position = portfolioResponse->add_positions(); // portfolioResponse will take ownership

    EXPECT_CALL(*grpcClientMock, getPortfolio(QThread::currentThread(), QString("account-id")))
        .WillOnce(Return(portfolioResponse));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(1000, QThread::currentThread())).WillOnce(Return(true));

    ASSERT_EQ(client->getValidPortfolio(QThread::currentThread(), "account-id"), nullptr);

    tinkoff::MoneyValue* tinkoffAvgPriceFifo = new tinkoff::MoneyValue(); // position will take ownership
    tinkoff::MoneyValue* tinkoffAvgPriceWavg = new tinkoff::MoneyValue(); // position will take ownership

    tinkoffAvgPriceFifo->set_currency("rub");
    tinkoffAvgPriceFifo->set_units(1);
    tinkoffAvgPriceFifo->set_nano(0);

    tinkoffAvgPriceWavg->set_currency("rub");
    tinkoffAvgPriceWavg->set_units(1);
    tinkoffAvgPriceWavg->set_nano(0);

    position->set_allocated_average_position_price_fifo(tinkoffAvgPriceFifo);
    position->set_allocated_average_position_price(tinkoffAvgPriceWavg);

    EXPECT_CALL(*grpcClientMock, getPortfolio(QThread::currentThread(), QString("account-id")))
        .WillOnce(Return(portfolioResponse));

    ASSERT_NE(client->getValidPortfolio(QThread::currentThread(), "account-id"), nullptr);
}
