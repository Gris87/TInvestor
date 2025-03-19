#include "src/threads/marketwaves/marketwavesthread.h"

#include <gtest/gtest.h>



class Test_MarketWavesThread : public ::testing::Test
{
protected:
    void SetUp()
    {
        thread = new MarketWavesThread();
    }

    void TearDown()
    {
        delete thread;
    }

    MarketWavesThread* thread;
};



TEST_F(Test_MarketWavesThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_MarketWavesThread, Test_run)
{
    thread->run();
}
