#include "src/threads/simulatormakedecision/simulatormakedecisionthread.h"

#include <gtest/gtest.h>

#include "src/decisions/idecisionmaker_mock.h"
#include "src/storage/stocks/istocksstorage_mock.h"



using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)
class Test_SimulatorMakeDecisionThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        stocksStorageMock = new StrictMock<StocksStorageMock>();
        decisionMakerMock = new StrictMock<DecisionMakerMock>();

        thread = new SimulatorMakeDecisionThread(stocksStorageMock, decisionMakerMock);
    }

    void TearDown() override
    {
        delete thread;
        delete stocksStorageMock;
        delete decisionMakerMock;
    }

    SimulatorMakeDecisionThread*   thread;
    StrictMock<StocksStorageMock>* stocksStorageMock;
    StrictMock<DecisionMakerMock>* decisionMakerMock;
};



TEST_F(Test_SimulatorMakeDecisionThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_SimulatorMakeDecisionThread, Test_run)
{
}

TEST_F(Test_SimulatorMakeDecisionThread, Test_terminateThread)
{
    thread->terminateThread();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init)
