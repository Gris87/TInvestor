#include "src/grpc/grpcclient.h"

#include <gtest/gtest.h>

#include "src/grpc/irawgrpcclient_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/utils/timeutils/itimeutils_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)
class Test_GrpcClient : public ::testing::Test
{
protected:
    void SetUp() override
    {
        userStorageMock   = new StrictMock<UserStorageMock>();
        rawGrpcClientMock = new StrictMock<RawGrpcClientMock>();
        timeUtilsMock     = new StrictMock<TimeUtilsMock>();

        client = new GrpcClient(userStorageMock, rawGrpcClientMock, timeUtilsMock);
    }

    void TearDown() override
    {
        delete client;
        delete userStorageMock;
        delete rawGrpcClientMock;
        delete timeUtilsMock;
    }

    GrpcClient*                    client;
    StrictMock<UserStorageMock>*   userStorageMock;
    StrictMock<RawGrpcClientMock>* rawGrpcClientMock;
    StrictMock<TimeUtilsMock>*     timeUtilsMock;
};



TEST_F(Test_GrpcClient, Test_constructor_and_destructor)
{
}

TEST_F(Test_GrpcClient, Test_getUserInfo)
{
    const InSequence seq;

    const grpc::Status goodStatus(grpc::StatusCode::OK, "");
    const grpc::Status resourceExhaustedStatus(grpc::StatusCode::RESOURCE_EXHAUSTED, "");
    const grpc::Status badStatus(grpc::StatusCode::INVALID_ARGUMENT, "");

    EXPECT_CALL(*rawGrpcClientMock, getUserInfo(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(goodStatus));

    ASSERT_NE(client->getUserInfo(QThread::currentThread()), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, getUserInfo(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*rawGrpcClientMock, getUserInfo(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(badStatus));

    ASSERT_EQ(client->getUserInfo(QThread::currentThread()), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, getUserInfo(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(true));

    ASSERT_EQ(client->getUserInfo(QThread::currentThread()), nullptr);
}

TEST_F(Test_GrpcClient, Test_getAccounts)
{
    const InSequence seq;

    const grpc::Status goodStatus(grpc::StatusCode::OK, "");
    const grpc::Status resourceExhaustedStatus(grpc::StatusCode::RESOURCE_EXHAUSTED, "");
    const grpc::Status badStatus(grpc::StatusCode::INVALID_ARGUMENT, "");

    EXPECT_CALL(*rawGrpcClientMock, getAccounts(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(goodStatus));

    ASSERT_NE(client->getAccounts(QThread::currentThread()), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, getAccounts(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*rawGrpcClientMock, getAccounts(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(badStatus));

    ASSERT_EQ(client->getAccounts(QThread::currentThread()), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, getAccounts(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(true));

    ASSERT_EQ(client->getAccounts(QThread::currentThread()), nullptr);
}

TEST_F(Test_GrpcClient, Test_findStocks)
{
    const InSequence seq;

    const grpc::Status goodStatus(grpc::StatusCode::OK, "");
    const grpc::Status resourceExhaustedStatus(grpc::StatusCode::RESOURCE_EXHAUSTED, "");
    const grpc::Status badStatus(grpc::StatusCode::INVALID_ARGUMENT, "");

    EXPECT_CALL(*rawGrpcClientMock, findStocks(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(goodStatus));

    ASSERT_NE(client->findStocks(QThread::currentThread(), tinkoff::INSTRUMENT_STATUS_ALL), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, findStocks(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*rawGrpcClientMock, findStocks(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(badStatus));

    ASSERT_EQ(client->findStocks(QThread::currentThread(), tinkoff::INSTRUMENT_STATUS_ALL), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, findStocks(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(true));

    ASSERT_EQ(client->findStocks(QThread::currentThread(), tinkoff::INSTRUMENT_STATUS_ALL), nullptr);
}

TEST_F(Test_GrpcClient, Test_findBonds)
{
    const InSequence seq;

    const grpc::Status goodStatus(grpc::StatusCode::OK, "");
    const grpc::Status resourceExhaustedStatus(grpc::StatusCode::RESOURCE_EXHAUSTED, "");
    const grpc::Status badStatus(grpc::StatusCode::INVALID_ARGUMENT, "");

    EXPECT_CALL(*rawGrpcClientMock, findBonds(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(goodStatus));

    ASSERT_NE(client->findBonds(QThread::currentThread()), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, findBonds(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*rawGrpcClientMock, findBonds(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(badStatus));

    ASSERT_EQ(client->findBonds(QThread::currentThread()), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, findBonds(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(true));

    ASSERT_EQ(client->findBonds(QThread::currentThread()), nullptr);
}

TEST_F(Test_GrpcClient, Test_findCurrencies)
{
    const InSequence seq;

    const grpc::Status goodStatus(grpc::StatusCode::OK, "");
    const grpc::Status resourceExhaustedStatus(grpc::StatusCode::RESOURCE_EXHAUSTED, "");
    const grpc::Status badStatus(grpc::StatusCode::INVALID_ARGUMENT, "");

    EXPECT_CALL(*rawGrpcClientMock, findCurrencies(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(goodStatus));

    ASSERT_NE(client->findCurrencies(QThread::currentThread()), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, findCurrencies(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*rawGrpcClientMock, findCurrencies(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(badStatus));

    ASSERT_EQ(client->findCurrencies(QThread::currentThread()), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, findCurrencies(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(true));

    ASSERT_EQ(client->findCurrencies(QThread::currentThread()), nullptr);
}

TEST_F(Test_GrpcClient, Test_findEtfs)
{
    const InSequence seq;

    const grpc::Status goodStatus(grpc::StatusCode::OK, "");
    const grpc::Status resourceExhaustedStatus(grpc::StatusCode::RESOURCE_EXHAUSTED, "");
    const grpc::Status badStatus(grpc::StatusCode::INVALID_ARGUMENT, "");

    EXPECT_CALL(*rawGrpcClientMock, findEtfs(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(goodStatus));

    ASSERT_NE(client->findEtfs(QThread::currentThread()), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, findEtfs(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*rawGrpcClientMock, findEtfs(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(badStatus));

    ASSERT_EQ(client->findEtfs(QThread::currentThread()), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, findEtfs(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(true));

    ASSERT_EQ(client->findEtfs(QThread::currentThread()), nullptr);
}

TEST_F(Test_GrpcClient, Test_findFutures)
{
    const InSequence seq;

    const grpc::Status goodStatus(grpc::StatusCode::OK, "");
    const grpc::Status resourceExhaustedStatus(grpc::StatusCode::RESOURCE_EXHAUSTED, "");
    const grpc::Status badStatus(grpc::StatusCode::INVALID_ARGUMENT, "");

    EXPECT_CALL(*rawGrpcClientMock, findFutures(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(goodStatus));

    ASSERT_NE(client->findFutures(QThread::currentThread()), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, findFutures(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*rawGrpcClientMock, findFutures(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(badStatus));

    ASSERT_EQ(client->findFutures(QThread::currentThread()), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, findFutures(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(true));

    ASSERT_EQ(client->findFutures(QThread::currentThread()), nullptr);
}

TEST_F(Test_GrpcClient, Test_getCandles)
{
    const InSequence seq;

    const grpc::Status goodStatus(grpc::StatusCode::OK, "");
    const grpc::Status resourceExhaustedStatus(grpc::StatusCode::RESOURCE_EXHAUSTED, "");
    const grpc::Status badStatus(grpc::StatusCode::INVALID_ARGUMENT, "");

    EXPECT_CALL(*rawGrpcClientMock, getCandles(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(goodStatus));

    ASSERT_NE(client->getCandles(QThread::currentThread(), "aaaaa", 0, 1000), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, getCandles(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*rawGrpcClientMock, getCandles(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(badStatus));

    ASSERT_EQ(client->getCandles(QThread::currentThread(), "aaaaa", 0, 1000), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, getCandles(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(true));

    ASSERT_EQ(client->getCandles(QThread::currentThread(), "aaaaa", 0, 1000), nullptr);
}

TEST_F(Test_GrpcClient, Test_getOrderBook)
{
    const InSequence seq;

    const grpc::Status goodStatus(grpc::StatusCode::OK, "");
    const grpc::Status resourceExhaustedStatus(grpc::StatusCode::RESOURCE_EXHAUSTED, "");
    const grpc::Status badStatus(grpc::StatusCode::INVALID_ARGUMENT, "");

    EXPECT_CALL(*rawGrpcClientMock, getOrderBook(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(goodStatus));

    ASSERT_NE(client->getOrderBook(QThread::currentThread(), "aaaaa", 50), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, getOrderBook(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*rawGrpcClientMock, getOrderBook(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(badStatus));

    ASSERT_EQ(client->getOrderBook(QThread::currentThread(), "aaaaa", 50), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, getOrderBook(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(true));

    ASSERT_EQ(client->getOrderBook(QThread::currentThread(), "aaaaa", 50), nullptr);
}

TEST_F(Test_GrpcClient, Test_getPortfolio)
{
    const InSequence seq;

    const grpc::Status goodStatus(grpc::StatusCode::OK, "");
    const grpc::Status resourceExhaustedStatus(grpc::StatusCode::RESOURCE_EXHAUSTED, "");
    const grpc::Status badStatus(grpc::StatusCode::INVALID_ARGUMENT, "");

    EXPECT_CALL(*rawGrpcClientMock, getPortfolio(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(goodStatus));

    ASSERT_NE(client->getPortfolio(QThread::currentThread(), "aaaaa"), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, getPortfolio(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*rawGrpcClientMock, getPortfolio(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(badStatus));

    ASSERT_EQ(client->getPortfolio(QThread::currentThread(), "aaaaa"), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, getPortfolio(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(true));

    ASSERT_EQ(client->getPortfolio(QThread::currentThread(), "aaaaa"), nullptr);
}

TEST_F(Test_GrpcClient, Test_getPositions)
{
    const InSequence seq;

    const grpc::Status goodStatus(grpc::StatusCode::OK, "");
    const grpc::Status resourceExhaustedStatus(grpc::StatusCode::RESOURCE_EXHAUSTED, "");
    const grpc::Status badStatus(grpc::StatusCode::INVALID_ARGUMENT, "");

    EXPECT_CALL(*rawGrpcClientMock, getPositions(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(goodStatus));

    ASSERT_NE(client->getPositions(QThread::currentThread(), "aaaaa"), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, getPositions(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*rawGrpcClientMock, getPositions(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(badStatus));

    ASSERT_EQ(client->getPositions(QThread::currentThread(), "aaaaa"), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, getPositions(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(true));

    ASSERT_EQ(client->getPositions(QThread::currentThread(), "aaaaa"), nullptr);
}

TEST_F(Test_GrpcClient, Test_getOperations)
{
    const InSequence seq;

    const grpc::Status goodStatus(grpc::StatusCode::OK, "");
    const grpc::Status resourceExhaustedStatus(grpc::StatusCode::RESOURCE_EXHAUSTED, "");
    const grpc::Status badStatus(grpc::StatusCode::INVALID_ARGUMENT, "");

    EXPECT_CALL(*rawGrpcClientMock, getOperations(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(goodStatus));

    ASSERT_NE(client->getOperations(QThread::currentThread(), "aaaaa", 1704056400000, 1735678800000, ""), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, getOperations(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*rawGrpcClientMock, getOperations(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(badStatus));

    ASSERT_EQ(client->getOperations(QThread::currentThread(), "aaaaa", 1704056400000, 1735678800000, ""), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, getOperations(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(true));

    ASSERT_EQ(client->getOperations(QThread::currentThread(), "aaaaa", 1704056400000, 1735678800000, ""), nullptr);
}

TEST_F(Test_GrpcClient, Test_getMaxLots)
{
    const InSequence seq;

    const grpc::Status goodStatus(grpc::StatusCode::OK, "");
    const grpc::Status resourceExhaustedStatus(grpc::StatusCode::RESOURCE_EXHAUSTED, "");
    const grpc::Status badStatus(grpc::StatusCode::INVALID_ARGUMENT, "");

    Quotation price;
    price.units = 500;
    price.nano  = 0;

    EXPECT_CALL(*rawGrpcClientMock, getMaxLots(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(goodStatus));

    ASSERT_NE(client->getMaxLots(QThread::currentThread(), "aaaaa", "aaa-aaa", price), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, getMaxLots(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*rawGrpcClientMock, getMaxLots(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(badStatus));

    ASSERT_EQ(client->getMaxLots(QThread::currentThread(), "aaaaa", "aaa-aaa", price), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, getMaxLots(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(true));

    ASSERT_EQ(client->getMaxLots(QThread::currentThread(), "aaaaa", "aaa-aaa", price), nullptr);
}

TEST_F(Test_GrpcClient, Test_postOrder)
{
    const InSequence seq;

    const grpc::Status goodStatus(grpc::StatusCode::OK, "");
    const grpc::Status resourceExhaustedStatus(grpc::StatusCode::RESOURCE_EXHAUSTED, "");
    const grpc::Status badStatus(grpc::StatusCode::INVALID_ARGUMENT, "");

    Quotation price;
    price.units = 500;
    price.nano  = 0;

    EXPECT_CALL(*rawGrpcClientMock, postOrder(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(goodStatus));

    ASSERT_NE(client->postOrder(QThread::currentThread(), "aaaaa", "aaa-aaa", tinkoff::ORDER_DIRECTION_BUY, 1, price), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, postOrder(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*rawGrpcClientMock, postOrder(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(badStatus));

    ASSERT_EQ(client->postOrder(QThread::currentThread(), "aaaaa", "aaa-aaa", tinkoff::ORDER_DIRECTION_BUY, 1, price), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, postOrder(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(true));

    ASSERT_EQ(client->postOrder(QThread::currentThread(), "aaaaa", "aaa-aaa", tinkoff::ORDER_DIRECTION_BUY, 1, price), nullptr);
}

TEST_F(Test_GrpcClient, Test_getOrderState)
{
    const InSequence seq;

    const grpc::Status goodStatus(grpc::StatusCode::OK, "");
    const grpc::Status resourceExhaustedStatus(grpc::StatusCode::RESOURCE_EXHAUSTED, "");
    const grpc::Status badStatus(grpc::StatusCode::INVALID_ARGUMENT, "");

    EXPECT_CALL(*rawGrpcClientMock, getOrderState(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(goodStatus));

    ASSERT_NE(client->getOrderState(QThread::currentThread(), "aaaaa", "007"), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, getOrderState(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*rawGrpcClientMock, getOrderState(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(badStatus));

    ASSERT_EQ(client->getOrderState(QThread::currentThread(), "aaaaa", "007"), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, getOrderState(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(true));

    ASSERT_EQ(client->getOrderState(QThread::currentThread(), "aaaaa", "007"), nullptr);
}

TEST_F(Test_GrpcClient, Test_cancelOrder)
{
    const InSequence seq;

    const grpc::Status goodStatus(grpc::StatusCode::OK, "");
    const grpc::Status resourceExhaustedStatus(grpc::StatusCode::RESOURCE_EXHAUSTED, "");
    const grpc::Status badStatus(grpc::StatusCode::INVALID_ARGUMENT, "");

    EXPECT_CALL(*rawGrpcClientMock, cancelOrder(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(goodStatus));

    ASSERT_NE(client->cancelOrder(QThread::currentThread(), "aaaaa", "007"), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, cancelOrder(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*rawGrpcClientMock, cancelOrder(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(badStatus));

    ASSERT_EQ(client->cancelOrder(QThread::currentThread(), "aaaaa", "007"), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, cancelOrder(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(true));

    ASSERT_EQ(client->cancelOrder(QThread::currentThread(), "aaaaa", "007"), nullptr);
}

TEST_F(Test_GrpcClient, Test_createMarketDataStream)
{
    const InSequence seq;

    EXPECT_CALL(*rawGrpcClientMock, createMarketDataStream(NotNull(), NotNull())).WillOnce(Return(nullptr));

    const std::shared_ptr<MarketDataStream> marketDataStream = client->createMarketDataStream();
    ASSERT_NE(marketDataStream, nullptr);
}

TEST_F(Test_GrpcClient, Test_subscribeLastPrices)
{
    const InSequence seq;

    QStringList instrumentIds;
    instrumentIds << "aaaaa";

    EXPECT_CALL(*rawGrpcClientMock, createMarketDataStream(NotNull(), NotNull())).WillOnce(Return(nullptr));

    std::shared_ptr<MarketDataStream> marketDataStream = client->createMarketDataStream();
    ASSERT_NE(marketDataStream, nullptr);

    EXPECT_CALL(*rawGrpcClientMock, writeMarketDataStream(marketDataStream, _)).WillOnce(Return(false));

    ASSERT_EQ(client->subscribeLastPrices(marketDataStream, instrumentIds), false);

    EXPECT_CALL(*rawGrpcClientMock, writeMarketDataStream(marketDataStream, _)).WillOnce(Return(true));

    ASSERT_EQ(client->subscribeLastPrices(marketDataStream, instrumentIds), true);
}

TEST_F(Test_GrpcClient, Test_unsubscribeLastPrices)
{
    const InSequence seq;

    EXPECT_CALL(*rawGrpcClientMock, createMarketDataStream(NotNull(), NotNull())).WillOnce(Return(nullptr));

    std::shared_ptr<MarketDataStream> marketDataStream = client->createMarketDataStream();
    ASSERT_NE(marketDataStream, nullptr);

    EXPECT_CALL(*rawGrpcClientMock, writeMarketDataStream(marketDataStream, _)).WillOnce(Return(false));

    ASSERT_EQ(client->unsubscribeLastPrices(marketDataStream), false);

    EXPECT_CALL(*rawGrpcClientMock, writeMarketDataStream(marketDataStream, _)).WillOnce(Return(true));

    ASSERT_EQ(client->unsubscribeLastPrices(marketDataStream), true);
}

TEST_F(Test_GrpcClient, Test_subscribeOrderBook)
{
    const InSequence seq;

    EXPECT_CALL(*rawGrpcClientMock, createMarketDataStream(NotNull(), NotNull())).WillOnce(Return(nullptr));

    std::shared_ptr<MarketDataStream> marketDataStream = client->createMarketDataStream();
    ASSERT_NE(marketDataStream, nullptr);

    EXPECT_CALL(*rawGrpcClientMock, writeMarketDataStream(marketDataStream, _)).WillOnce(Return(false));

    ASSERT_EQ(client->subscribeOrderBook(marketDataStream, "aaaaa", 50), false);

    EXPECT_CALL(*rawGrpcClientMock, writeMarketDataStream(marketDataStream, _)).WillOnce(Return(true));

    ASSERT_EQ(client->subscribeOrderBook(marketDataStream, "aaaaa", 50), true);
}

TEST_F(Test_GrpcClient, Test_unsubscribeOrderBook)
{
    const InSequence seq;

    EXPECT_CALL(*rawGrpcClientMock, createMarketDataStream(NotNull(), NotNull())).WillOnce(Return(nullptr));

    std::shared_ptr<MarketDataStream> marketDataStream = client->createMarketDataStream();
    ASSERT_NE(marketDataStream, nullptr);

    EXPECT_CALL(*rawGrpcClientMock, writeMarketDataStream(marketDataStream, _)).WillOnce(Return(false));

    ASSERT_EQ(client->unsubscribeOrderBook(marketDataStream), false);

    EXPECT_CALL(*rawGrpcClientMock, writeMarketDataStream(marketDataStream, _)).WillOnce(Return(true));

    ASSERT_EQ(client->unsubscribeOrderBook(marketDataStream), true);
}

TEST_F(Test_GrpcClient, Test_readMarketDataStream)
{
    const InSequence seq;

    EXPECT_CALL(*rawGrpcClientMock, createMarketDataStream(NotNull(), NotNull())).WillOnce(Return(nullptr));

    std::shared_ptr<MarketDataStream> marketDataStream = client->createMarketDataStream();
    ASSERT_NE(marketDataStream, nullptr);

    EXPECT_CALL(*rawGrpcClientMock, readMarketDataStream(marketDataStream, NotNull())).WillOnce(Return(false));

    ASSERT_EQ(client->readMarketDataStream(marketDataStream), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, readMarketDataStream(marketDataStream, NotNull())).WillOnce(Return(true));

    ASSERT_NE(client->readMarketDataStream(marketDataStream), nullptr);
}

TEST_F(Test_GrpcClient, Test_closeWriteMarketDataStream)
{
    const InSequence seq;

    EXPECT_CALL(*rawGrpcClientMock, createMarketDataStream(NotNull(), NotNull())).WillOnce(Return(nullptr));

    std::shared_ptr<MarketDataStream> marketDataStream = client->createMarketDataStream();
    ASSERT_NE(marketDataStream, nullptr);

    EXPECT_CALL(*rawGrpcClientMock, closeWriteMarketDataStream(marketDataStream)).WillOnce(Return(false));

    ASSERT_EQ(client->closeWriteMarketDataStream(marketDataStream), false);

    EXPECT_CALL(*rawGrpcClientMock, closeWriteMarketDataStream(marketDataStream)).WillOnce(Return(true));

    ASSERT_EQ(client->closeWriteMarketDataStream(marketDataStream), true);
}

TEST_F(Test_GrpcClient, Test_cancelMarketDataStream)
{
    const InSequence seq;

    EXPECT_CALL(*rawGrpcClientMock, createMarketDataStream(NotNull(), NotNull())).WillOnce(Return(nullptr));

    std::shared_ptr<MarketDataStream> marketDataStream = client->createMarketDataStream();
    ASSERT_NE(marketDataStream, nullptr);

    client->cancelMarketDataStream(marketDataStream);
}

TEST_F(Test_GrpcClient, Test_finishMarketDataStream)
{
    const InSequence seq;

    const grpc::Status goodStatus(grpc::StatusCode::OK, "");
    const grpc::Status badStatus(grpc::StatusCode::INVALID_ARGUMENT, "");

    EXPECT_CALL(*rawGrpcClientMock, createMarketDataStream(NotNull(), NotNull())).WillOnce(Return(nullptr));

    std::shared_ptr<MarketDataStream> marketDataStream = client->createMarketDataStream();
    ASSERT_NE(marketDataStream, nullptr);

    EXPECT_CALL(*rawGrpcClientMock, finishMarketDataStream(marketDataStream)).WillOnce(Return(badStatus));

    client->finishMarketDataStream(marketDataStream);

    EXPECT_CALL(*rawGrpcClientMock, finishMarketDataStream(marketDataStream)).WillOnce(Return(goodStatus));

    client->finishMarketDataStream(marketDataStream);
}

TEST_F(Test_GrpcClient, Test_createPortfolioStream)
{
    const InSequence seq;

    EXPECT_CALL(*rawGrpcClientMock, createPortfolioStream(NotNull(), NotNull(), _)).WillOnce(Return(nullptr));

    const std::shared_ptr<PortfolioStream> portfolioStream1 = client->createPortfolioStream("aaaaa");
    ASSERT_NE(portfolioStream1, nullptr);

    EXPECT_CALL(*rawGrpcClientMock, createPortfolioStream(NotNull(), NotNull(), _)).WillOnce(Return(nullptr));

    const std::shared_ptr<PortfolioStream> portfolioStream2 = client->createPortfolioStream("aaaaa", "bbbbb");
    ASSERT_NE(portfolioStream2, nullptr);
}

TEST_F(Test_GrpcClient, Test_readPortfolioStream)
{
    const InSequence seq;

    EXPECT_CALL(*rawGrpcClientMock, createPortfolioStream(NotNull(), NotNull(), _)).WillOnce(Return(nullptr));

    std::shared_ptr<PortfolioStream> portfolioStream = client->createPortfolioStream("aaaaa");
    ASSERT_NE(portfolioStream, nullptr);

    EXPECT_CALL(*rawGrpcClientMock, readPortfolioStream(portfolioStream, NotNull())).WillOnce(Return(false));

    ASSERT_EQ(client->readPortfolioStream(portfolioStream), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, readPortfolioStream(portfolioStream, NotNull())).WillOnce(Return(true));

    ASSERT_NE(client->readPortfolioStream(portfolioStream), nullptr);
}

TEST_F(Test_GrpcClient, Test_cancelPortfolioStream)
{
    const InSequence seq;

    EXPECT_CALL(*rawGrpcClientMock, createPortfolioStream(NotNull(), NotNull(), _)).WillOnce(Return(nullptr));

    std::shared_ptr<PortfolioStream> portfolioStream = client->createPortfolioStream("aaaaa");
    ASSERT_NE(portfolioStream, nullptr);

    client->cancelPortfolioStream(portfolioStream);
}

TEST_F(Test_GrpcClient, Test_finishPortfolioStream)
{
    const InSequence seq;

    const grpc::Status goodStatus(grpc::StatusCode::OK, "");
    const grpc::Status badStatus(grpc::StatusCode::INVALID_ARGUMENT, "");

    EXPECT_CALL(*rawGrpcClientMock, createPortfolioStream(NotNull(), NotNull(), _)).WillOnce(Return(nullptr));

    std::shared_ptr<PortfolioStream> portfolioStream = client->createPortfolioStream("aaaaa");
    ASSERT_NE(portfolioStream, nullptr);

    EXPECT_CALL(*rawGrpcClientMock, finishPortfolioStream(portfolioStream)).WillOnce(Return(badStatus));

    client->finishPortfolioStream(portfolioStream);

    EXPECT_CALL(*rawGrpcClientMock, finishPortfolioStream(portfolioStream)).WillOnce(Return(goodStatus));

    client->finishPortfolioStream(portfolioStream);
}

TEST_F(Test_GrpcClient, Test_createPositionsStream)
{
    const InSequence seq;

    EXPECT_CALL(*rawGrpcClientMock, createPositionsStream(NotNull(), NotNull(), _)).WillOnce(Return(nullptr));

    const std::shared_ptr<PositionsStream> positionsStream = client->createPositionsStream("aaaaa");
    ASSERT_NE(positionsStream, nullptr);
}

TEST_F(Test_GrpcClient, Test_readPositionsStream)
{
    const InSequence seq;

    EXPECT_CALL(*rawGrpcClientMock, createPositionsStream(NotNull(), NotNull(), _)).WillOnce(Return(nullptr));

    std::shared_ptr<PositionsStream> positionsStream = client->createPositionsStream("aaaaa");
    ASSERT_NE(positionsStream, nullptr);

    EXPECT_CALL(*rawGrpcClientMock, readPositionsStream(positionsStream, NotNull())).WillOnce(Return(false));

    ASSERT_EQ(client->readPositionsStream(positionsStream), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, readPositionsStream(positionsStream, NotNull())).WillOnce(Return(true));

    ASSERT_NE(client->readPositionsStream(positionsStream), nullptr);
}

TEST_F(Test_GrpcClient, Test_cancelPositionsStream)
{
    const InSequence seq;

    EXPECT_CALL(*rawGrpcClientMock, createPositionsStream(NotNull(), NotNull(), _)).WillOnce(Return(nullptr));

    std::shared_ptr<PositionsStream> positionsStream = client->createPositionsStream("aaaaa");
    ASSERT_NE(positionsStream, nullptr);

    client->cancelPositionsStream(positionsStream);
}

TEST_F(Test_GrpcClient, Test_finishPositionsStream)
{
    const InSequence seq;

    const grpc::Status goodStatus(grpc::StatusCode::OK, "");
    const grpc::Status badStatus(grpc::StatusCode::INVALID_ARGUMENT, "");

    EXPECT_CALL(*rawGrpcClientMock, createPositionsStream(NotNull(), NotNull(), _)).WillOnce(Return(nullptr));

    std::shared_ptr<PositionsStream> positionsStream = client->createPositionsStream("aaaaa");
    ASSERT_NE(positionsStream, nullptr);

    EXPECT_CALL(*rawGrpcClientMock, finishPositionsStream(positionsStream)).WillOnce(Return(badStatus));

    client->finishPositionsStream(positionsStream);

    EXPECT_CALL(*rawGrpcClientMock, finishPositionsStream(positionsStream)).WillOnce(Return(goodStatus));

    client->finishPositionsStream(positionsStream);
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init)
