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



class Test_GrpcClient : public ::testing::Test
{
protected:
    void SetUp()
    {
        userStorageMock   = new StrictMock<UserStorageMock>();
        rawGrpcClientMock = new StrictMock<RawGrpcClientMock>();
        timeUtilsMock     = new StrictMock<TimeUtilsMock>();

        client = new GrpcClient(userStorageMock, rawGrpcClientMock, timeUtilsMock);
    }

    void TearDown()
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
    InSequence seq;

    grpc::Status goodStatus(grpc::StatusCode::OK, "");
    grpc::Status resourceExhaustedStatus(grpc::StatusCode::RESOURCE_EXHAUSTED, "");
    grpc::Status badStatus(grpc::StatusCode::INVALID_ARGUMENT, "");

    EXPECT_CALL(*rawGrpcClientMock, getUserInfo(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*rawGrpcClientMock, getUserInfo(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(badStatus));

    ASSERT_EQ(client->getUserInfo(QThread::currentThread()), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, getUserInfo(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(goodStatus));

    ASSERT_NE(client->getUserInfo(QThread::currentThread()), nullptr);
}

TEST_F(Test_GrpcClient, Test_getAccounts)
{
    InSequence seq;

    grpc::Status goodStatus(grpc::StatusCode::OK, "");
    grpc::Status resourceExhaustedStatus(grpc::StatusCode::RESOURCE_EXHAUSTED, "");
    grpc::Status badStatus(grpc::StatusCode::INVALID_ARGUMENT, "");

    EXPECT_CALL(*rawGrpcClientMock, getAccounts(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*rawGrpcClientMock, getAccounts(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(badStatus));

    ASSERT_EQ(client->getAccounts(QThread::currentThread()), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, getAccounts(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(goodStatus));

    ASSERT_NE(client->getAccounts(QThread::currentThread()), nullptr);
}

TEST_F(Test_GrpcClient, Test_findStocks)
{
    InSequence seq;

    grpc::Status goodStatus(grpc::StatusCode::OK, "");
    grpc::Status resourceExhaustedStatus(grpc::StatusCode::RESOURCE_EXHAUSTED, "");
    grpc::Status badStatus(grpc::StatusCode::INVALID_ARGUMENT, "");

    EXPECT_CALL(*rawGrpcClientMock, findStocks(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*rawGrpcClientMock, findStocks(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(badStatus));

    ASSERT_EQ(client->findStocks(QThread::currentThread()), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, findStocks(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(goodStatus));

    ASSERT_NE(client->findStocks(QThread::currentThread()), nullptr);
}

TEST_F(Test_GrpcClient, Test_getCandles)
{
    InSequence seq;

    grpc::Status goodStatus(grpc::StatusCode::OK, "");
    grpc::Status resourceExhaustedStatus(grpc::StatusCode::RESOURCE_EXHAUSTED, "");
    grpc::Status badStatus(grpc::StatusCode::INVALID_ARGUMENT, "");

    EXPECT_CALL(*rawGrpcClientMock, getCandles(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(resourceExhaustedStatus));
    EXPECT_CALL(*timeUtilsMock, interruptibleSleep(5000, QThread::currentThread())).WillOnce(Return(false));
    EXPECT_CALL(*rawGrpcClientMock, getCandles(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(badStatus));

    ASSERT_EQ(client->getCandles(QThread::currentThread(), "aaaaa", 0, 1000), nullptr);

    EXPECT_CALL(*rawGrpcClientMock, getCandles(NotNull(), NotNull(), _, NotNull())).WillOnce(Return(goodStatus));

    ASSERT_NE(client->getCandles(QThread::currentThread(), "aaaaa", 0, 1000), nullptr);
}

TEST_F(Test_GrpcClient, Test_createMarketDataStream)
{
    InSequence seq;

    EXPECT_CALL(*rawGrpcClientMock, createMarketDataStream(NotNull(), NotNull())).WillOnce(Return(nullptr));

    std::shared_ptr<MarketDataStream> marketDataStream = client->createMarketDataStream();
    ASSERT_NE(marketDataStream, nullptr);
}

TEST_F(Test_GrpcClient, Test_subscribeLastPrices)
{
    InSequence seq;

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
    InSequence seq;

    EXPECT_CALL(*rawGrpcClientMock, createMarketDataStream(NotNull(), NotNull())).WillOnce(Return(nullptr));

    std::shared_ptr<MarketDataStream> marketDataStream = client->createMarketDataStream();
    ASSERT_NE(marketDataStream, nullptr);

    EXPECT_CALL(*rawGrpcClientMock, writeMarketDataStream(marketDataStream, _)).WillOnce(Return(false));

    ASSERT_EQ(client->unsubscribeLastPrices(marketDataStream), false);

    EXPECT_CALL(*rawGrpcClientMock, writeMarketDataStream(marketDataStream, _)).WillOnce(Return(true));

    ASSERT_EQ(client->unsubscribeLastPrices(marketDataStream), true);
}

TEST_F(Test_GrpcClient, Test_readMarketDataStream)
{
    InSequence seq;

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
    InSequence seq;

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
    InSequence seq;

    grpc::Status goodStatus(grpc::StatusCode::OK, "");
    grpc::Status badStatus(grpc::StatusCode::INVALID_ARGUMENT, "");

    EXPECT_CALL(*rawGrpcClientMock, createMarketDataStream(NotNull(), NotNull())).WillOnce(Return(nullptr));

    std::shared_ptr<MarketDataStream> marketDataStream = client->createMarketDataStream();
    ASSERT_NE(marketDataStream, nullptr);

    EXPECT_CALL(*rawGrpcClientMock, finishMarketDataStream(marketDataStream)).WillOnce(Return(badStatus));

    client->finishMarketDataStream(marketDataStream);

    EXPECT_CALL(*rawGrpcClientMock, finishMarketDataStream(marketDataStream)).WillOnce(Return(goodStatus));

    client->finishMarketDataStream(marketDataStream);
}
