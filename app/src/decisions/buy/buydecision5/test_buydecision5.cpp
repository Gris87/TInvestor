#include "src/decisions/buy/buydecision5/buydecision5.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision5config/ibuydecision5config_mock.h"
#include "src/config/decisions/idecisionmakerconfig_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_BuyDecision5 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        buyDecision5 = new BuyDecision5();
    }

    void TearDown() override
    {
        delete buyDecision5;
    }

    BuyDecision5* buyDecision5;
};



TEST_F(Test_BuyDecision5, Test_constructor_and_destructor)
{
}

TEST_F(Test_BuyDecision5, Test_makeDecision)
{
}

TEST_F(Test_BuyDecision5, Test_asapMode)
{
    ASSERT_EQ(buyDecision5->asapMode(), ASAP_MODE_IMMEDIATELY_TRADE);
}
