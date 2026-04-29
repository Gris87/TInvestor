#include "src/utils/exception/exception.h"

#include <gtest/gtest.h>



class Test_Exception : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};



TEST_F(Test_Exception, Test_throwException)
{
    lastThrownException = "";
    throwException("Blah-Blah");
    ASSERT_EQ(lastThrownException, "Blah-Blah");
}
