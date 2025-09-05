#include "src/threads/bidirtrading/bidirtradingthreadfactory.h"

#include <gtest/gtest.h>



class Test_BiDirTradingThreadFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new BiDirTradingThreadFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    BiDirTradingThreadFactory* factory;
};



TEST_F(Test_BiDirTradingThreadFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_BiDirTradingThreadFactory, Test_newInstance)
{
    const IBiDirTradingThread* thread = factory->newInstance(nullptr);
    ASSERT_TRUE(thread != nullptr);

    delete thread;
}
