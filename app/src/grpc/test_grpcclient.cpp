#include "src/grpc/grpcclient.h"

#include <gtest/gtest.h>



TEST(Test_GrpcClient, Test_constructor_and_destructor)
{
    GrpcClient client;
}

TEST(Test_GrpcClient, Test_connect)
{
    GrpcClient client;

    client.connect();
}
