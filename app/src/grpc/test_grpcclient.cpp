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

    ASSERT_NE(client->findStocks(QThread::currentThread()), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, findStocks(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*rawGrpcClientMock, findStocks(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(badStatus));

    ASSERT_EQ(client->findStocks(QThread::currentThread()), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, findStocks(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(true));

    ASSERT_EQ(client->findStocks(QThread::currentThread()), nullptr);
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

    ASSERT_NE(client->getOrderBook(QThread::currentThread(), "aaaaa"), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, getOrderBook(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*rawGrpcClientMock, getOrderBook(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(badStatus));

    ASSERT_EQ(client->getOrderBook(QThread::currentThread(), "aaaaa"), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, getOrderBook(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(true));

    ASSERT_EQ(client->getOrderBook(QThread::currentThread(), "aaaaa"), nullptr);
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

    QStringList uids;
    uids << "aaaaa";

    EXPECT_CALL(*rawGrpcClientMock, createMarketDataStream(NotNull(), NotNull())).WillOnce(Return(nullptr));

    std::shared_ptr<MarketDataStream> marketDataStream = client->createMarketDataStream();
    ASSERT_NE(marketDataStream, nullptr);

    EXPECT_CALL(*rawGrpcClientMock, writeMarketDataStream(marketDataStream, _)).WillOnce(Return(false));

    ASSERT_EQ(client->subscribeLastPrices(marketDataStream, uids), false);

    EXPECT_CALL(*rawGrpcClientMock, writeMarketDataStream(marketDataStream, _)).WillOnce(Return(true));

    ASSERT_EQ(client->subscribeLastPrices(marketDataStream, uids), true);
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

    ASSERT_EQ(client->subscribeOrderBook(marketDataStream, "aaaaa"), false);

    EXPECT_CALL(*rawGrpcClientMock, writeMarketDataStream(marketDataStream, _)).WillOnce(Return(true));

    ASSERT_EQ(client->subscribeOrderBook(marketDataStream, "aaaaa"), true);
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
// NOLINTEND(cppcoreguidelines-pro-type-member-init)
