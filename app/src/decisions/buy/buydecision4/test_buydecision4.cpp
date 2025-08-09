#include "src/decisions/buy/buydecision4/buydecision4.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision4config/ibuydecision4config_mock.h"
#include "src/config/decisions/idecisionmakerconfig_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_BuyDecision4 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        buyDecision4 = new BuyDecision4();
    }

    void TearDown() override
    {
        delete buyDecision4;
    }

    BuyDecision4* buyDecision4;
};



TEST_F(Test_BuyDecision4, Test_constructor_and_destructor)
{
}

TEST_F(Test_BuyDecision4, Test_makeDecision)
{
}
