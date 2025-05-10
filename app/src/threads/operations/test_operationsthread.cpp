#include "src/threads/operations/operationsthread.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/user/iuserstorage_mock.h"



using ::testing::StrictMock;



class Test_OperationsThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        userStorageMock = new StrictMock<UserStorageMock>();
        grpcClientMock  = new StrictMock<GrpcClientMock>();

        thread = new OperationsThread(userStorageMock, grpcClientMock);
    }

    void TearDown() override
    {
        delete thread;
        delete userStorageMock;
        delete grpcClientMock;
    }

    OperationsThread*            thread;
    StrictMock<UserStorageMock>* userStorageMock;
    StrictMock<GrpcClientMock>*  grpcClientMock;
};



TEST_F(Test_OperationsThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_OperationsThread, Test_run)
{
    // thread->run();
}
