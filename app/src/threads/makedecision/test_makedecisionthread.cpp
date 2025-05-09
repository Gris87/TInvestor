#include "src/threads/makedecision/makedecisionthread.h"

#include <gtest/gtest.h>

#include "src/config/iconfig_mock.h"
#include "src/storage/stocks/istocksstorage_mock.h"



using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)
class Test_MakeDecisionThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        configMock         = new StrictMock<ConfigMock>();
        stocksStorageMock  = new StrictMock<StocksStorageMock>();

        thread = new MakeDecisionThread(configMock, stocksStorageMock);
    }

    void TearDown() override
    {
        delete thread;
        delete configMock;
        delete stocksStorageMock;
    }

    MakeDecisionThread*             thread;
    StrictMock<ConfigMock>*         configMock;
    StrictMock<StocksStorageMock>*  stocksStorageMock;
};



TEST_F(Test_MakeDecisionThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_MakeDecisionThread, Test_run)
{
    thread->run();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init)
