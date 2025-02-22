#include "src/threads/pricecollect/pricecollectthread.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"



using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_PriceCollectThread : public ::testing::Test
{
protected:
    void SetUp()
    {
        grpcClientMock  = new StrictMock<GrpcClientMock>();

        thread = new PriceCollectThread(grpcClientMock);
    }

    void TearDown()
    {
        delete thread;
        delete grpcClientMock;
    }

    PriceCollectThread*          thread;
    StrictMock<GrpcClientMock>*  grpcClientMock;
};



TEST_F(Test_PriceCollectThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_PriceCollectThread, Test_run)
{
}
