#include "src/threads/follow/followthread.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/threads/logs/ilogsthread_mock.h"



using ::testing::StrictMock;



class Test_FollowThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        instrumentsStorageMock = new StrictMock<InstrumentsStorageMock>();
        grpcClientMock         = new StrictMock<GrpcClientMock>();
        logsThreadMock         = new StrictMock<LogsThreadMock>();

        thread = new FollowThread(instrumentsStorageMock, grpcClientMock, logsThreadMock);
    }

    void TearDown() override
    {
        delete thread;
        delete instrumentsStorageMock;
        delete grpcClientMock;
        delete logsThreadMock;
    }

    FollowThread*                       thread;
    StrictMock<InstrumentsStorageMock>* instrumentsStorageMock;
    StrictMock<GrpcClientMock>*         grpcClientMock;
    StrictMock<LogsThreadMock>*         logsThreadMock;
};



TEST_F(Test_FollowThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_FollowThread, Test_run)
{
    // thread->run();
}
