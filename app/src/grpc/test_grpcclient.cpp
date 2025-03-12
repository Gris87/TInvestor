#include "src/grpc/grpcclient.h"

#include <gtest/gtest.h>

#include "src/storage/user/iuserstorage_mock.h"
#include "src/utils/timeutils/itimeutils_mock.h"



using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_GrpcClient : public ::testing::Test
{
protected:
    void SetUp()
    {
        userStorageMock = new StrictMock<UserStorageMock>();
        timeUtilsMock   = new StrictMock<TimeUtilsMock>();

        client = new GrpcClient(userStorageMock, timeUtilsMock);
    }

    void TearDown()
    {
        delete client;
        delete userStorageMock;
        delete timeUtilsMock;
    }

    GrpcClient*                  client;
    StrictMock<UserStorageMock>* userStorageMock;
    StrictMock<TimeUtilsMock>*   timeUtilsMock;
};



TEST_F(Test_GrpcClient, Test_constructor_and_destructor)
{
}
