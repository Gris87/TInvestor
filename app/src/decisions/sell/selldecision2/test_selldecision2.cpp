#include "src/decisions/sell/selldecision2/selldecision2.h"

#include <gtest/gtest.h>

#include "src/config/decisions/idecisionmakerconfig_mock.h"
#include "src/config/decisions/sell/selldecision2config/iselldecision2config_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_SellDecision2 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        sellDecision2 = new SellDecision2();
    }

    void TearDown() override
    {
        delete sellDecision2;
    }

    SellDecision2* sellDecision2;
};



TEST_F(Test_SellDecision2, Test_constructor_and_destructor)
{
}

TEST_F(Test_SellDecision2, Test_makeDecision)
{
}
