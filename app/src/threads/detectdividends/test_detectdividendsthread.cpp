#include "src/threads/detectdividends/detectdividendsthread.h"

#include <gtest/gtest.h>

#include "src/storage/stocks/istocksstorage_mock.h"



using ::testing::Ge;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_DetectDividendsThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        stocksStorageMock = new StrictMock<StocksStorageMock>();

        thread = new DetectDividendsThread(stocksStorageMock);
    }

    void TearDown() override
    {
        delete thread;
        delete stocksStorageMock;
    }

    DetectDividendsThread*         thread;
    StrictMock<StocksStorageMock>* stocksStorageMock;
};



TEST_F(Test_DetectDividendsThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_DetectDividendsThread, Test_run)
{
}

TEST_F(Test_DetectDividendsThread, Test_terminateThread)
{
    thread->terminateThread();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
