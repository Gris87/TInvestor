#include "src/grpc/rawgrpcclient.h"

#include <gtest/gtest.h>



class Test_RawGrpcClient : public ::testing::Test
{
protected:
    void SetUp()
    {
        client = new RawGrpcClient();
    }

    void TearDown()
    {
        delete client;
    }

    RawGrpcClient* client;
};



TEST_F(Test_RawGrpcClient, Test_constructor_and_destructor)
{
}
