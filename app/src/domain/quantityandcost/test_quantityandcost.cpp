#include "src/domain/quantityandcost/quantityandcost.h"

#include <gtest/gtest.h>



class Test_QuantityAndCost : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};



TEST_F(Test_QuantityAndCost, Test_constructor_and_destructor)
{
    const QuantityAndCost       quantityAndCost1;
    const QuantityAndCostDouble quantityAndCost2;

    // clang-format off
    ASSERT_EQ(quantityAndCost1.quantity,         0);
    ASSERT_EQ(quantityAndCost1.fifoItems.size(), 0);
    ASSERT_EQ(quantityAndCost1.costFifo.units,   0);
    ASSERT_EQ(quantityAndCost1.costFifo.nano,    0);
    ASSERT_EQ(quantityAndCost1.costWavg.units,   0);
    ASSERT_EQ(quantityAndCost1.costWavg.nano,    0);
    // clang-format on

    // clang-format off
    ASSERT_EQ(quantityAndCost2.quantity, 0);
    ASSERT_NEAR(quantityAndCost2.cost,   0, 0.0001);
    // clang-format on
}
