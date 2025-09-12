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
