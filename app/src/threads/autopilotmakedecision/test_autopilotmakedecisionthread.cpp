#include "src/threads/autopilotmakedecision/autopilotmakedecisionthread.h"

#include <gtest/gtest.h>

#include "src/config/iconfig_mock.h"
#include "src/storage/stocks/istocksstorage_mock.h"



using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)
class Test_AutoPilotMakeDecisionThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        configMock        = new StrictMock<ConfigMock>();
        stocksStorageMock = new StrictMock<StocksStorageMock>();

        thread = new AutoPilotMakeDecisionThread(configMock, stocksStorageMock);
    }

    void TearDown() override
    {
        delete thread;
        delete configMock;
        delete stocksStorageMock;
    }

    AutoPilotMakeDecisionThread*   thread;
    StrictMock<ConfigMock>*        configMock;
    StrictMock<StocksStorageMock>* stocksStorageMock;
};



TEST_F(Test_AutoPilotMakeDecisionThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_AutoPilotMakeDecisionThread, Test_run)
{
    thread->run();
}

TEST_F(Test_AutoPilotMakeDecisionThread, Test_terminateThread)
{
    thread->terminateThread();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init)
