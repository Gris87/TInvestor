#include "src/domain/filter/filter.h"

#include <gtest/gtest.h>



TEST(Test_Filter, Test_constructor_and_destructor)
{
    Filter filter;

    // clang-format off
    ASSERT_EQ(filter.useTicker,            false);
    ASSERT_EQ(filter.ticker,               "");
    ASSERT_EQ(filter.usePrice,             false);
    ASSERT_NEAR(filter.priceFrom,          0.0f, 0.0001f);
    ASSERT_NEAR(filter.priceTo,            0.0f, 0.0001f);
    ASSERT_EQ(filter.useDayStartChange,    false);
    ASSERT_NEAR(filter.dayStartChangeFrom, 0.0f, 0.0001f);
    ASSERT_NEAR(filter.dayStartChangeTo,   0.0f, 0.0001f);
    ASSERT_EQ(filter.useDateChange,        false);
    ASSERT_NEAR(filter.dateChangeFrom,     0.0f, 0.0001f);
    ASSERT_NEAR(filter.dateChangeTo,       0.0f, 0.0001f);
    ASSERT_EQ(filter.usePayback,           false);
    ASSERT_NEAR(filter.paybackFrom,        0.0f, 0.0001f);
    ASSERT_NEAR(filter.paybackTo,          100.0f, 0.0001f);
    // clang-format on
}

TEST(Test_Filter, Test_copy_constructor)
{
    Filter filter;

    filter.useTicker          = true;
    filter.ticker             = "BLAH";
    filter.usePrice           = true;
    filter.priceFrom          = 1.0f;
    filter.priceTo            = 2.0f;
    filter.useDayStartChange  = true;
    filter.dayStartChangeFrom = 3.0f;
    filter.dayStartChangeTo   = 4.0f;
    filter.useDateChange      = true;
    filter.dateChangeFrom     = 5.0f;
    filter.dateChangeTo       = 6.0f;
    filter.usePayback         = true;
    filter.paybackFrom        = 7.0f;
    filter.paybackTo          = 8.0f;

    Filter filter2(filter);

    // clang-format off
    ASSERT_EQ(filter2.useTicker,            true);
    ASSERT_EQ(filter2.ticker,               "BLAH");
    ASSERT_EQ(filter2.usePrice,             true);
    ASSERT_NEAR(filter2.priceFrom,          1.0f, 0.0001f);
    ASSERT_NEAR(filter2.priceTo,            2.0f, 0.0001f);
    ASSERT_EQ(filter2.useDayStartChange,    true);
    ASSERT_NEAR(filter2.dayStartChangeFrom, 3.0f, 0.0001f);
    ASSERT_NEAR(filter2.dayStartChangeTo,   4.0f, 0.0001f);
    ASSERT_EQ(filter2.useDateChange,        true);
    ASSERT_NEAR(filter2.dateChangeFrom,     5.0f, 0.0001f);
    ASSERT_NEAR(filter2.dateChangeTo,       6.0f, 0.0001f);
    ASSERT_EQ(filter2.usePayback,           true);
    ASSERT_NEAR(filter2.paybackFrom,        7.0f, 0.0001f);
    ASSERT_NEAR(filter2.paybackTo,          8.0f, 0.0001f);
    // clang-format on
}

TEST(Test_Filter, Test_assign)
{
    Filter filter;
    Filter filter2;

    filter.useTicker          = true;
    filter.ticker             = "BLAH";
    filter.usePrice           = true;
    filter.priceFrom          = 1.0f;
    filter.priceTo            = 2.0f;
    filter.useDayStartChange  = true;
    filter.dayStartChangeFrom = 3.0f;
    filter.dayStartChangeTo   = 4.0f;
    filter.useDateChange      = true;
    filter.dateChangeFrom     = 5.0f;
    filter.dateChangeTo       = 6.0f;
    filter.usePayback         = true;
    filter.paybackFrom        = 7.0f;
    filter.paybackTo          = 8.0f;

    filter2 = filter;

    // clang-format off
    ASSERT_EQ(filter2.useTicker,            true);
    ASSERT_EQ(filter2.ticker,               "BLAH");
    ASSERT_EQ(filter2.usePrice,             true);
    ASSERT_NEAR(filter2.priceFrom,          1.0f, 0.0001f);
    ASSERT_NEAR(filter2.priceTo,            2.0f, 0.0001f);
    ASSERT_EQ(filter2.useDayStartChange,    true);
    ASSERT_NEAR(filter2.dayStartChangeFrom, 3.0f, 0.0001f);
    ASSERT_NEAR(filter2.dayStartChangeTo,   4.0f, 0.0001f);
    ASSERT_EQ(filter2.useDateChange,        true);
    ASSERT_NEAR(filter2.dateChangeFrom,     5.0f, 0.0001f);
    ASSERT_NEAR(filter2.dateChangeTo,       6.0f, 0.0001f);
    ASSERT_EQ(filter2.usePayback,           true);
    ASSERT_NEAR(filter2.paybackFrom,        7.0f, 0.0001f);
    ASSERT_NEAR(filter2.paybackTo,          8.0f, 0.0001f);
    // clang-format on
}

TEST(Test_Filter, Test_isFiltered)
{
    Filter filter;

    filter.useTicker          = true;
    filter.ticker             = "SPB";
    filter.usePrice           = true;
    filter.priceFrom          = 250.0f;
    filter.priceTo            = 350.0f;
    filter.useDayStartChange  = true;
    filter.dayStartChangeFrom = 2.0f;
    filter.dayStartChangeTo   = 5.0f;
    filter.useDateChange      = true;
    filter.dateChangeFrom     = 50.0f;
    filter.dateChangeTo       = 80.0f;
    filter.usePayback         = true;
    filter.paybackFrom        = 70.0f;
    filter.paybackTo          = 95.0f;

    // clang-format off
    ASSERT_EQ(filter.isFiltered("SEPB", 300.0f, 3.5f, 75.0f, 90.0f), false);
    ASSERT_EQ(filter.isFiltered("SPBE", 200.0f, 3.5f, 75.0f, 90.0f), false);
    ASSERT_EQ(filter.isFiltered("SPBE", 400.0f, 3.5f, 75.0f, 90.0f), false);
    ASSERT_EQ(filter.isFiltered("SPBE", 300.0f, 1.0f, 75.0f, 90.0f), false);
    ASSERT_EQ(filter.isFiltered("SPBE", 300.0f, 8.0f, 75.0f, 90.0f), false);
    ASSERT_EQ(filter.isFiltered("SPBE", 300.0f, 3.5f, 40.0f, 90.0f), false);
    ASSERT_EQ(filter.isFiltered("SPBE", 300.0f, 3.5f, 90.0f, 90.0f), false);
    ASSERT_EQ(filter.isFiltered("SPBE", 300.0f, 3.5f, 75.0f, 60.0f), false);
    ASSERT_EQ(filter.isFiltered("SPBE", 300.0f, 3.5f, 75.0f, 99.0f), false);
    ASSERT_EQ(filter.isFiltered("SPBE", 300.0f, 3.5f, 75.0f, 90.0f), true);
    // clang-format on
}

TEST(Test_Filter, Test_equals)
{
    Filter filter;
    Filter filter2;

    filter.useTicker          = true;
    filter.ticker             = "BLAH";
    filter.usePrice           = true;
    filter.priceFrom          = 1.0f;
    filter.priceTo            = 2.0f;
    filter.useDayStartChange  = true;
    filter.dayStartChangeFrom = 3.0f;
    filter.dayStartChangeTo   = 4.0f;
    filter.useDateChange      = true;
    filter.dateChangeFrom     = 5.0f;
    filter.dateChangeTo       = 6.0f;
    filter.usePayback         = true;
    filter.paybackFrom        = 7.0f;
    filter.paybackTo          = 8.0f;

    filter2.useTicker          = true;
    filter2.ticker             = "BLAH";
    filter2.usePrice           = true;
    filter2.priceFrom          = 1.0f;
    filter2.priceTo            = 2.0f;
    filter2.useDayStartChange  = true;
    filter2.dayStartChangeFrom = 3.0f;
    filter2.dayStartChangeTo   = 4.0f;
    filter2.useDateChange      = true;
    filter2.dateChangeFrom     = 5.0f;
    filter2.dateChangeTo       = 6.0f;
    filter2.usePayback         = true;
    filter2.paybackFrom        = 7.0f;
    filter2.paybackTo          = 8.0f;

    ASSERT_EQ(filter, filter2);

    filter2.useTicker = false;
    ASSERT_NE(filter, filter2);
    filter2.useTicker = true;
    ASSERT_EQ(filter, filter2);

    filter2.ticker = "HOOYAK";
    ASSERT_NE(filter, filter2);
    filter2.ticker = "BLAH";
    ASSERT_EQ(filter, filter2);

    filter2.usePrice = false;
    ASSERT_NE(filter, filter2);
    filter2.usePrice = true;
    ASSERT_EQ(filter, filter2);

    filter2.priceFrom = 100.0f;
    ASSERT_NE(filter, filter2);
    filter2.priceFrom = 1.0f;
    ASSERT_EQ(filter, filter2);

    filter2.priceTo = 100.0f;
    ASSERT_NE(filter, filter2);
    filter2.priceTo = 2.0f;
    ASSERT_EQ(filter, filter2);

    filter2.useDayStartChange = false;
    ASSERT_NE(filter, filter2);
    filter2.useDayStartChange = true;
    ASSERT_EQ(filter, filter2);

    filter2.dayStartChangeFrom = 100.0f;
    ASSERT_NE(filter, filter2);
    filter2.dayStartChangeFrom = 3.0f;
    ASSERT_EQ(filter, filter2);

    filter2.dayStartChangeTo = 100.0f;
    ASSERT_NE(filter, filter2);
    filter2.dayStartChangeTo = 4.0f;
    ASSERT_EQ(filter, filter2);

    filter2.useDateChange = false;
    ASSERT_NE(filter, filter2);
    filter2.useDateChange = true;
    ASSERT_EQ(filter, filter2);

    filter2.dateChangeFrom = 100.0f;
    ASSERT_NE(filter, filter2);
    filter2.dateChangeFrom = 5.0f;
    ASSERT_EQ(filter, filter2);

    filter2.dateChangeTo = 100.0f;
    ASSERT_NE(filter, filter2);
    filter2.dateChangeTo = 6.0f;
    ASSERT_EQ(filter, filter2);

    filter2.usePayback = false;
    ASSERT_NE(filter, filter2);
    filter2.usePayback = true;
    ASSERT_EQ(filter, filter2);

    filter2.paybackFrom = 100.0f;
    ASSERT_NE(filter, filter2);
    filter2.paybackFrom = 7.0f;
    ASSERT_EQ(filter, filter2);

    filter2.paybackTo = 100.0f;
    ASSERT_NE(filter, filter2);
    filter2.paybackTo = 8.0f;
    ASSERT_EQ(filter, filter2);
}
