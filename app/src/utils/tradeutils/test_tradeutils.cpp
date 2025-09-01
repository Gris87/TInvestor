#include "src/utils/tradeutils/tradeutils.h"

#include <gtest/gtest.h>



class Test_TradeUtils : public ::testing::Test
{
protected:
    void SetUp() override
    {
        tradeUtils = new TradeUtils();
    }

    void TearDown() override
    {
        delete tradeUtils;
    }

    TradeUtils* tradeUtils;
};



TEST_F(Test_TradeUtils, Test_constructor_and_destructor)
{
}
