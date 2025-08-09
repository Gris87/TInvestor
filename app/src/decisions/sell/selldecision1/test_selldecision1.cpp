#include "src/decisions/sell/selldecision1/selldecision1.h"

#include <gtest/gtest.h>

#include "src/config/decisions/idecisionmakerconfig_mock.h"
#include "src/config/decisions/sell/selldecision1config/iselldecision1config_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_SellDecision1 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        sellDecision1 = new SellDecision1();
    }

    void TearDown() override
    {
        delete sellDecision1;
    }

    SellDecision1* sellDecision1;
};



TEST_F(Test_SellDecision1, Test_constructor_and_destructor)
{
}

TEST_F(Test_SellDecision1, Test_makeDecision)
{
}
