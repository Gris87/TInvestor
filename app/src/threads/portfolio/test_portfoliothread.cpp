#include "src/threads/portfolio/portfoliothread.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/logos/ilogosstorage_mock.h"



using ::testing::StrictMock;



class Test_PortfolioThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        instrumentsStorageMock = new StrictMock<InstrumentsStorageMock>();
        logosStorageMock       = new StrictMock<LogosStorageMock>();
        grpcClientMock         = new StrictMock<GrpcClientMock>();

        thread = new PortfolioThread(instrumentsStorageMock, logosStorageMock, grpcClientMock);
    }

    void TearDown() override
    {
        delete thread;
        delete instrumentsStorageMock;
        delete logosStorageMock;
        delete grpcClientMock;
    }

    PortfolioThread*                    thread;
    StrictMock<InstrumentsStorageMock>* instrumentsStorageMock;
    StrictMock<LogosStorageMock>*       logosStorageMock;
    StrictMock<GrpcClientMock>*         grpcClientMock;
};



TEST_F(Test_PortfolioThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_PortfolioThread, Test_run)
{
    // thread->run();
}
