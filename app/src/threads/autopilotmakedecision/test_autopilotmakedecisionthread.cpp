#include "src/threads/autopilotmakedecision/autopilotmakedecisionthread.h"

#include <gtest/gtest.h>

#include "src/decisions/idecisionmaker_mock.h"
#include "src/storage/stocks/istocksstorage_mock.h"



using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)
class Test_AutoPilotMakeDecisionThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        stocksStorageMock = new StrictMock<StocksStorageMock>();
        decisionMakerMock = new StrictMock<DecisionMakerMock>();

        thread = new AutoPilotMakeDecisionThread(stocksStorageMock, decisionMakerMock);
    }

    void TearDown() override
    {
        delete thread;
        delete stocksStorageMock;
        delete decisionMakerMock;
    }

    AutoPilotMakeDecisionThread*   thread;
    StrictMock<StocksStorageMock>* stocksStorageMock;
    StrictMock<DecisionMakerMock>* decisionMakerMock;
};



TEST_F(Test_AutoPilotMakeDecisionThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_AutoPilotMakeDecisionThread, Test_run)
{
}

TEST_F(Test_AutoPilotMakeDecisionThread, Test_terminateThread)
{
    thread->terminateThread();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init)
