#include "src/decisions/sell/selldecision4/selldecision4.h"

#include <gtest/gtest.h>

#include "src/config/decisions/idecisionmakerconfig_mock.h"
#include "src/config/decisions/sell/selldecision4config/iselldecision4config_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_SellDecision4 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        sellDecision4 = new SellDecision4();
    }

    void TearDown() override
    {
        delete sellDecision4;
    }

    SellDecision4* sellDecision4;
};



TEST_F(Test_SellDecision4, Test_constructor_and_destructor)
{
}

TEST_F(Test_SellDecision4, Test_makeDecision)
{
}
