#include "src/utils/notifier/notifier.h"

#include <gtest/gtest.h>



class Test_Notifier : public ::testing::Test
{
protected:
    void SetUp() override
    {
        notifier = new Notifier();
    }

    void TearDown() override
    {
        delete notifier;
    }

    Notifier* notifier;
};



TEST_F(Test_Notifier, Test_constructor_and_destructor)
{
}
