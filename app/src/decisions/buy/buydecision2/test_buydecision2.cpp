#include "src/decisions/buy/buydecision2/buydecision2.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision2config/ibuydecision2config_mock.h"
#include "src/config/decisions/idecisionmakerconfig_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_BuyDecision2 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        buyDecision2 = new BuyDecision2();
    }

    void TearDown() override
    {
        delete buyDecision2;
    }

    BuyDecision2* buyDecision2;
};



TEST_F(Test_BuyDecision2, Test_constructor_and_destructor)
{
}

TEST_F(Test_BuyDecision2, Test_makeDecision)
{
}
