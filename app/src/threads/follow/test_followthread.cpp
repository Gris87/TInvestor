#include "src/threads/follow/followthread.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/instruments/iinstrumentsstorage_mock.h"



using ::testing::StrictMock;



class Test_FollowThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        instrumentsStorageMock = new StrictMock<InstrumentsStorageMock>();
        grpcClientMock         = new StrictMock<GrpcClientMock>();

        thread = new FollowThread(instrumentsStorageMock, grpcClientMock);
    }

    void TearDown() override
    {
        delete thread;
        delete instrumentsStorageMock;
        delete grpcClientMock;
    }

    FollowThread*                       thread;
    StrictMock<InstrumentsStorageMock>* instrumentsStorageMock;
    StrictMock<GrpcClientMock>*         grpcClientMock;
};



TEST_F(Test_FollowThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_FollowThread, Test_run)
{
    // thread->run();
}
