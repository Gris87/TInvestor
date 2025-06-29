#include "src/utils/exception/exception.h"

#include <gtest/gtest.h>



TEST(Test_Exception, Test_throwException)
{
    lastThrownException = "";
    throwException("Blah-Blah");
    ASSERT_EQ(lastThrownException, "Blah-Blah");
}
