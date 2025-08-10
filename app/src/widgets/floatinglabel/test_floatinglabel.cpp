#include "src/widgets/floatinglabel/floatinglabel.h"

#include <gtest/gtest.h>



class Test_FloatingLabel : public ::testing::Test
{
protected:
    void SetUp() override
    {
        label = new FloatingLabel("+100.00%", QPoint(0, 0), QPoint(10, 10));
    }

    void TearDown() override
    {
        delete label;
    }

    FloatingLabel* label;
};



TEST_F(Test_FloatingLabel, Test_constructor_and_destructor)
{
}

TEST_F(Test_FloatingLabel, Test_deathTimerTicked)
{
    label->deathTimerTicked();
}
