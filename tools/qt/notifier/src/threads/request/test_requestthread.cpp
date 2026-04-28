#include "src/threads/request/requestthread.h"

#include <gtest/gtest.h>

#include "src/config/iconfig_mock.h"



using ::testing::Ge;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_RequestThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        configMock = new StrictMock<ConfigMock>();

        thread = new RequestThread(configMock);
    }

    void TearDown() override
    {
        delete thread;
        delete configMock;
    }

    RequestThread*          thread;
    StrictMock<ConfigMock>* configMock;
};



TEST_F(Test_RequestThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_RequestThread, Test_run)
{
}

TEST_F(Test_RequestThread, Test_terminateThread)
{
    thread->terminateThread();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
