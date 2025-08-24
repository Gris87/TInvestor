#include "src/utils/messagebox/messageboxutils.h"

#include <gtest/gtest.h>



class Test_MessageBoxUtils : public ::testing::Test
{
protected:
    void SetUp() override
    {
        messageBoxUtils = new MessageBoxUtils();
    }

    void TearDown() override
    {
        delete messageBoxUtils;
    }

    MessageBoxUtils* messageBoxUtils;
};



TEST_F(Test_MessageBoxUtils, Test_constructor_and_destructor)
{
}

TEST_F(Test_MessageBoxUtils, Test_warning)
{
    // Nothing
}

TEST_F(Test_MessageBoxUtils, Test_question)
{
    // Nothing
}
