#include "src/grpc/grpcclient.h"

#include <gtest/gtest.h>

#include "src/storage/user/iuserstorage_mock.h"



using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_GrpcClient : public ::testing::Test
{
protected:
    void SetUp()
    {
        userStorageMock = new StrictMock<UserStorageMock>();

        client = new GrpcClient(userStorageMock);
    }

    void TearDown()
    {
        delete client;
        delete userStorageMock;
    }

    GrpcClient*                  client;
    StrictMock<UserStorageMock>* userStorageMock;
};



TEST_F(Test_GrpcClient, Test_constructor_and_destructor)
{
}
