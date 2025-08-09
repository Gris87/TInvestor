#include "src/decisions/sell/selldecision3/selldecision3.h"

#include <gtest/gtest.h>

#include "src/config/decisions/idecisionmakerconfig_mock.h"
#include "src/config/decisions/sell/selldecision3config/iselldecision3config_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_SellDecision3 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        sellDecision3 = new SellDecision3();
    }

    void TearDown() override
    {
        delete sellDecision3;
    }

    SellDecision3* sellDecision3;
};



TEST_F(Test_SellDecision3, Test_constructor_and_destructor)
{
}

TEST_F(Test_SellDecision3, Test_makeDecision)
{
}
