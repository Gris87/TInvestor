#include "src/domain/logo/logo.h"

#include <gtest/gtest.h>



TEST(Test_Logo, Test_constructor_and_destructor)
{
    const Logo logo;

    ASSERT_TRUE(logo.pixmap.isNull());
}
