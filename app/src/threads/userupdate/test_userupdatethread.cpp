#include "src/threads/userupdate/userupdatethread.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/user/iuserstorage_mock.h"



using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_UserUpdateThread : public ::testing::Test
{
protected:
    void SetUp()
    {
        userStorageMock = new StrictMock<UserStorageMock>();
        grpcClientMock  = new StrictMock<GrpcClientMock>();

        thread = new UserUpdateThread(userStorageMock, grpcClientMock);
    }

    void TearDown()
    {
        delete thread;
        delete userStorageMock;
        delete grpcClientMock;
    }

    UserUpdateThread*            thread;
    StrictMock<UserStorageMock>* userStorageMock;
    StrictMock<GrpcClientMock>*  grpcClientMock;
};



TEST_F(Test_UserUpdateThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_UserUpdateThread, Test_run)
{
}
