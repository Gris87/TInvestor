#include "src/utils/bollindger/bollindger.h"

#include <gtest/gtest.h>



class Test_Bollindger : public ::testing::Test
{
protected:
    void SetUp() override
    {
        bollindger = new Bollindger();
    }

    void TearDown() override
    {
        delete bollindger;
    }

    Bollindger* bollindger;
};



TEST_F(Test_Bollindger, Test_constructor_and_destructor)
{
}
