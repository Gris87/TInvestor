#include "src/threads/detectshorts/detectshortsthread.h"

#include <gtest/gtest.h>

#include "src/storage/stocks/istocksstorage_mock.h"



using ::testing::Ge;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_DetectShortsThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        stocksStorageMock = new StrictMock<StocksStorageMock>();

        thread = new DetectShortsThread(stocksStorageMock);
    }

    void TearDown() override
    {
        delete thread;
        delete stocksStorageMock;
    }

    DetectShortsThread*            thread;
    StrictMock<StocksStorageMock>* stocksStorageMock;
};



TEST_F(Test_DetectShortsThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_DetectShortsThread, Test_run)
{
}

TEST_F(Test_DetectShortsThread, Test_terminateThread)
{
    thread->terminateThread();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
