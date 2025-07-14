#include "src/threads/autopilotdecisionmaker/autopilotdecisionmakerthread.h"

#include <gtest/gtest.h>

#include "src/decisions/idecisionmaker_mock.h"
#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/stocks/istocksstorage_mock.h"



using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)
class Test_AutoPilotDecisionMakerThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        stocksStorageMock = new StrictMock<StocksStorageMock>();
        decisionMakerMock = new StrictMock<DecisionMakerMock>();
        grpcClientMock    = new StrictMock<GrpcClientMock>();

        thread = new AutoPilotDecisionMakerThread(stocksStorageMock, decisionMakerMock, grpcClientMock);
    }

    void TearDown() override
    {
        delete thread;
        delete stocksStorageMock;
        delete decisionMakerMock;
        delete grpcClientMock;
    }

    AutoPilotDecisionMakerThread*  thread;
    StrictMock<StocksStorageMock>* stocksStorageMock;
    StrictMock<DecisionMakerMock>* decisionMakerMock;
    StrictMock<GrpcClientMock>*    grpcClientMock;
};



TEST_F(Test_AutoPilotDecisionMakerThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_AutoPilotDecisionMakerThread, Test_run)
{
}

TEST_F(Test_AutoPilotDecisionMakerThread, Test_terminateThread)
{
    thread->terminateThread();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init)
