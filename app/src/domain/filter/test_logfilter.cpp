#include "src/domain/filter/logfilter.h"

#include <gtest/gtest.h>



// NOLINTBEGIN(readability-function-cognitive-complexity, readability-magic-numbers)
TEST(Test_LogFilter, Test_constructor_and_destructor)
{
    const LogFilter filter;

    // clang-format off
    ASSERT_EQ(filter.level,  LOG_LEVEL_VERBOSE);
    ASSERT_EQ(filter.ticker, "");
    // clang-format on
}

TEST(Test_LogFilter, Test_copy_constructor)
{
    LogFilter filter;

    filter.level  = LOG_LEVEL_DEBUG;
    filter.ticker = "BLAH";

    const LogFilter filter2(filter);

    // clang-format off
    ASSERT_EQ(filter2.level,  LOG_LEVEL_DEBUG);
    ASSERT_EQ(filter2.ticker, "BLAH");
    // clang-format on
}

TEST(Test_LogFilter, Test_assign)
{
    LogFilter filter;
    LogFilter filter2;

    filter.level  = LOG_LEVEL_DEBUG;
    filter.ticker = "BLAH";

    filter2 = filter;

    // clang-format off
    ASSERT_EQ(filter2.level,  LOG_LEVEL_DEBUG);
    ASSERT_EQ(filter2.ticker, "BLAH");
    // clang-format on
}

TEST(Test_LogFilter, Test_isFiltered)
{
    LogFilter filter;

    filter.level  = LOG_LEVEL_DEBUG;
    filter.ticker = "SPB";

    // clang-format off
    ASSERT_EQ(filter.isFiltered(LOG_LEVEL_VERBOSE, "SPBE", "SPB Market"), false);
    ASSERT_EQ(filter.isFiltered(LOG_LEVEL_DEBUG,   "SEPB", "SEB Market"), false);
    ASSERT_EQ(filter.isFiltered(LOG_LEVEL_DEBUG,   "SPBE", "SPB Market"), true);
    // clang-format on
}

TEST(Test_LogFilter, Test_equals)
{
    LogFilter filter;
    LogFilter filter2;

    filter.level  = LOG_LEVEL_DEBUG;
    filter.ticker = "BLAH";

    filter2.level  = LOG_LEVEL_DEBUG;
    filter2.ticker = "BLAH";

    ASSERT_EQ(filter, filter2);

    filter2.level = LOG_LEVEL_ERROR;
    ASSERT_NE(filter, filter2);
    filter2.level = LOG_LEVEL_DEBUG;
    ASSERT_EQ(filter, filter2);

    filter2.ticker = "HOOYAK";
    ASSERT_NE(filter, filter2);
    filter2.ticker = "BLAH";
    ASSERT_EQ(filter, filter2);
}
// NOLINTEND(readability-function-cognitive-complexity, readability-magic-numbers)
