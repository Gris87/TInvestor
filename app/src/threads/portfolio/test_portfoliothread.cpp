#include "src/threads/portfolio/portfoliothread.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"



using ::testing::StrictMock;



class Test_PortfolioThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        grpcClientMock = new StrictMock<GrpcClientMock>();

        thread = new PortfolioThread(grpcClientMock);
    }

    void TearDown() override
    {
        delete thread;
        delete grpcClientMock;
    }

    PortfolioThread*            thread;
    StrictMock<GrpcClientMock>* grpcClientMock;
};



TEST_F(Test_PortfolioThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_PortfolioThread, Test_run)
{
    // thread->run();
}
