#include "src/utils/style/darkpalette.h"

#include <gtest/gtest.h>



class Test_DarkPalette : public ::testing::Test
{
protected:
    void SetUp() override
    {
        palette = new DarkPalette();
    }

    void TearDown() override
    {
        delete palette;
    }

    DarkPalette* palette;
};



TEST_F(Test_DarkPalette, Test_constructor_and_destructor)
{
}
