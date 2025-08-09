#include "src/decisions/buy/buydecision3/buydecision3.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision3config/ibuydecision3config_mock.h"
#include "src/config/decisions/idecisionmakerconfig_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_BuyDecision3 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        buyDecision3 = new BuyDecision3();
    }

    void TearDown() override
    {
        delete buyDecision3;
    }

    BuyDecision3* buyDecision3;
};



TEST_F(Test_BuyDecision3, Test_constructor_and_destructor)
{
}

TEST_F(Test_BuyDecision3, Test_makeDecision)
{
}
