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

TEST(Test_LogFilter, Test_isActive)
{
    LogFilter filter;

    ASSERT_EQ(filter.isActive(), false);

    filter.level = LOG_LEVEL_ERROR;
    ASSERT_EQ(filter.isActive(), true);
    filter.level = LOG_LEVEL_VERBOSE;
    ASSERT_EQ(filter.isActive(), false);

    filter.ticker = "aaaaa";
    ASSERT_EQ(filter.isActive(), true);
    filter.ticker = "";
    ASSERT_EQ(filter.isActive(), false);
}

TEST(Test_LogFilter, Test_isFiltered)
{
    LogFilter filter;
    LogEntry  entry;

    filter.level  = LOG_LEVEL_DEBUG;
    filter.ticker = "SPB";

    entry.level            = LOG_LEVEL_DEBUG;
    entry.instrumentTicker = "SPBE";
    entry.instrumentName   = "SPB Market";

    ASSERT_EQ(filter.isFiltered(entry), true);

    entry.level            = LOG_LEVEL_VERBOSE;
    entry.instrumentTicker = "SPBE";
    entry.instrumentName   = "SPB Market";

    ASSERT_EQ(filter.isFiltered(entry), false);

    entry.level            = LOG_LEVEL_DEBUG;
    entry.instrumentTicker = "SEPB";
    entry.instrumentName   = "SEB Market";

    ASSERT_EQ(filter.isFiltered(entry), false);
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
