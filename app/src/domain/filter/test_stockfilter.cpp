#include "src/domain/filter/stockfilter.h"

#include <gtest/gtest.h>



// NOLINTBEGIN(readability-function-cognitive-complexity, readability-magic-numbers)
TEST(Test_StockFilter, Test_constructor_and_destructor)
{
    const StockFilter filter;

    // clang-format off
    ASSERT_EQ(filter.useTicker,            false);
    ASSERT_EQ(filter.ticker,               "");
    ASSERT_EQ(filter.useQualInvestor,      false);
    ASSERT_EQ(filter.qualInvestor,         QUAL_INVESTOR_SHOW_ALL);
    ASSERT_EQ(filter.usePrice,             false);
    ASSERT_NEAR(filter.priceFrom,          0.0f, 0.0001f);
    ASSERT_NEAR(filter.priceTo,            0.0f, 0.0001f);
    ASSERT_EQ(filter.useDayStartChange,    false);
    ASSERT_NEAR(filter.dayStartChangeFrom, 0.0f, 0.0001f);
    ASSERT_NEAR(filter.dayStartChangeTo,   0.0f, 0.0001f);
    ASSERT_EQ(filter.useDateChange,        false);
    ASSERT_NEAR(filter.dateChangeFrom,     0.0f, 0.0001f);
    ASSERT_NEAR(filter.dateChangeTo,       0.0f, 0.0001f);
    ASSERT_EQ(filter.useTurnover,          false);
    ASSERT_EQ(filter.turnoverFrom,         0);
    ASSERT_EQ(filter.turnoverTo,           1000000000000);
    ASSERT_EQ(filter.usePayback,           false);
    ASSERT_NEAR(filter.paybackFrom,        0.0f, 0.0001f);
    ASSERT_NEAR(filter.paybackTo,          100.0f, 0.0001f);
    // clang-format on
}

TEST(Test_StockFilter, Test_copy_constructor)
{
    StockFilter filter;

    filter.useTicker          = true;
    filter.ticker             = "BLAH";
    filter.useQualInvestor    = true;
    filter.qualInvestor       = QUAL_INVESTOR_ONLY_WITHOUT_STATUS;
    filter.usePrice           = true;
    filter.priceFrom          = 1.0f;
    filter.priceTo            = 2.0f;
    filter.useDayStartChange  = true;
    filter.dayStartChangeFrom = 3.0f;
    filter.dayStartChangeTo   = 4.0f;
    filter.useDateChange      = true;
    filter.dateChangeFrom     = 5.0f;
    filter.dateChangeTo       = 6.0f;
    filter.useTurnover        = true;
    filter.turnoverFrom       = 7;
    filter.turnoverTo         = 8;
    filter.usePayback         = true;
    filter.paybackFrom        = 9.0f;
    filter.paybackTo          = 10.0f;

    const StockFilter filter2(filter);

    // clang-format off
    ASSERT_EQ(filter2.useTicker,            true);
    ASSERT_EQ(filter2.ticker,               "BLAH");
    ASSERT_EQ(filter2.useQualInvestor,      true);
    ASSERT_EQ(filter2.qualInvestor,         QUAL_INVESTOR_ONLY_WITHOUT_STATUS);
    ASSERT_EQ(filter2.usePrice,             true);
    ASSERT_NEAR(filter2.priceFrom,          1.0f, 0.0001f);
    ASSERT_NEAR(filter2.priceTo,            2.0f, 0.0001f);
    ASSERT_EQ(filter2.useDayStartChange,    true);
    ASSERT_NEAR(filter2.dayStartChangeFrom, 3.0f, 0.0001f);
    ASSERT_NEAR(filter2.dayStartChangeTo,   4.0f, 0.0001f);
    ASSERT_EQ(filter2.useDateChange,        true);
    ASSERT_NEAR(filter2.dateChangeFrom,     5.0f, 0.0001f);
    ASSERT_NEAR(filter2.dateChangeTo,       6.0f, 0.0001f);
    ASSERT_EQ(filter2.useTurnover,          true);
    ASSERT_EQ(filter2.turnoverFrom,         7);
    ASSERT_EQ(filter2.turnoverTo,           8);
    ASSERT_EQ(filter2.usePayback,           true);
    ASSERT_NEAR(filter2.paybackFrom,        9.0f, 0.0001f);
    ASSERT_NEAR(filter2.paybackTo,          10.0f, 0.0001f);
    // clang-format on
}

TEST(Test_StockFilter, Test_assign)
{
    StockFilter filter;
    StockFilter filter2;

    filter.useTicker          = true;
    filter.ticker             = "BLAH";
    filter.useQualInvestor    = true;
    filter.qualInvestor       = QUAL_INVESTOR_ONLY_WITHOUT_STATUS;
    filter.usePrice           = true;
    filter.priceFrom          = 1.0f;
    filter.priceTo            = 2.0f;
    filter.useDayStartChange  = true;
    filter.dayStartChangeFrom = 3.0f;
    filter.dayStartChangeTo   = 4.0f;
    filter.useDateChange      = true;
    filter.dateChangeFrom     = 5.0f;
    filter.dateChangeTo       = 6.0f;
    filter.useTurnover        = true;
    filter.turnoverFrom       = 7;
    filter.turnoverTo         = 8;
    filter.usePayback         = true;
    filter.paybackFrom        = 9.0f;
    filter.paybackTo          = 10.0f;

    filter2 = filter;

    // clang-format off
    ASSERT_EQ(filter2.useTicker,            true);
    ASSERT_EQ(filter2.ticker,               "BLAH");
    ASSERT_EQ(filter2.useQualInvestor,      true);
    ASSERT_EQ(filter2.qualInvestor,         QUAL_INVESTOR_ONLY_WITHOUT_STATUS);
    ASSERT_EQ(filter2.usePrice,             true);
    ASSERT_NEAR(filter2.priceFrom,          1.0f, 0.0001f);
    ASSERT_NEAR(filter2.priceTo,            2.0f, 0.0001f);
    ASSERT_EQ(filter2.useDayStartChange,    true);
    ASSERT_NEAR(filter2.dayStartChangeFrom, 3.0f, 0.0001f);
    ASSERT_NEAR(filter2.dayStartChangeTo,   4.0f, 0.0001f);
    ASSERT_EQ(filter2.useDateChange,        true);
    ASSERT_NEAR(filter2.dateChangeFrom,     5.0f, 0.0001f);
    ASSERT_NEAR(filter2.dateChangeTo,       6.0f, 0.0001f);
    ASSERT_EQ(filter2.useTurnover,          true);
    ASSERT_EQ(filter2.turnoverFrom,         7);
    ASSERT_EQ(filter2.turnoverTo,           8);
    ASSERT_EQ(filter2.usePayback,           true);
    ASSERT_NEAR(filter2.paybackFrom,        9.0f, 0.0001f);
    ASSERT_NEAR(filter2.paybackTo,          10.0f, 0.0001f);
    // clang-format on
}

TEST(Test_StockFilter, Test_isActive)
{
    StockFilter filter;

    ASSERT_EQ(filter.isActive(), false);

    filter.useTicker = true;
    ASSERT_EQ(filter.isActive(), false);
    filter.ticker = "aaa";
    ASSERT_EQ(filter.isActive(), true);
    filter.useTicker = false;
    ASSERT_EQ(filter.isActive(), false);
    filter.ticker = "";
    ASSERT_EQ(filter.isActive(), false);

    filter.useQualInvestor = true;
    ASSERT_EQ(filter.isActive(), false);
    filter.qualInvestor = QUAL_INVESTOR_ONLY_WITH_STATUS;
    ASSERT_EQ(filter.isActive(), true);
    filter.useQualInvestor = false;
    ASSERT_EQ(filter.isActive(), false);
    filter.qualInvestor = QUAL_INVESTOR_SHOW_ALL;
    ASSERT_EQ(filter.isActive(), false);

    filter.usePrice = true;
    ASSERT_EQ(filter.isActive(), true);
    filter.usePrice = false;
    ASSERT_EQ(filter.isActive(), false);

    filter.useDayStartChange = true;
    ASSERT_EQ(filter.isActive(), true);
    filter.useDayStartChange = false;
    ASSERT_EQ(filter.isActive(), false);

    filter.useDateChange = true;
    ASSERT_EQ(filter.isActive(), true);
    filter.useDateChange = false;
    ASSERT_EQ(filter.isActive(), false);

    filter.useTurnover = true;
    ASSERT_EQ(filter.isActive(), true);
    filter.useTurnover = false;
    ASSERT_EQ(filter.isActive(), false);

    filter.usePayback = true;
    ASSERT_EQ(filter.isActive(), true);
    filter.usePayback = false;
    ASSERT_EQ(filter.isActive(), false);
}

TEST(Test_StockFilter, Test_isFiltered)
{
    StockFilter filter;

    filter.useTicker          = true;
    filter.ticker             = "SPB";
    filter.useQualInvestor    = true;
    filter.qualInvestor       = QUAL_INVESTOR_ONLY_WITHOUT_STATUS;
    filter.usePrice           = true;
    filter.priceFrom          = 250.0f;
    filter.priceTo            = 350.0f;
    filter.useDayStartChange  = true;
    filter.dayStartChangeFrom = 2.0f;
    filter.dayStartChangeTo   = 5.0f;
    filter.useDateChange      = true;
    filter.dateChangeFrom     = 50.0f;
    filter.dateChangeTo       = 80.0f;
    filter.useTurnover        = true;
    filter.turnoverFrom       = 150;
    filter.turnoverTo         = 200;
    filter.usePayback         = true;
    filter.paybackFrom        = 70.0f;
    filter.paybackTo          = 95.0f;

    // clang-format off
    ASSERT_EQ(filter.isFiltered("SEPB", "SEB Market", false, 300.0f, 3.5f, 75.0f, 170, 90.0f), false);
    ASSERT_EQ(filter.isFiltered("SPBE", "SPB Market", true, 300.0f, 3.5f, 75.0f, 170, 90.0f), false);
    ASSERT_EQ(filter.isFiltered("SPBE", "SPB Market", false, 200.0f, 3.5f, 75.0f, 170, 90.0f), false);
    ASSERT_EQ(filter.isFiltered("SPBE", "SPB Market", false, 400.0f, 3.5f, 75.0f, 170, 90.0f), false);
    ASSERT_EQ(filter.isFiltered("SPBE", "SPB Market", false, 300.0f, 1.0f, 75.0f, 170, 90.0f), false);
    ASSERT_EQ(filter.isFiltered("SPBE", "SPB Market", false, 300.0f, 8.0f, 75.0f, 170, 90.0f), false);
    ASSERT_EQ(filter.isFiltered("SPBE", "SPB Market", false, 300.0f, 3.5f, 40.0f, 170, 90.0f), false);
    ASSERT_EQ(filter.isFiltered("SPBE", "SPB Market", false, 300.0f, 3.5f, 90.0f, 170, 90.0f), false);
    ASSERT_EQ(filter.isFiltered("SPBE", "SPB Market", false, 300.0f, 3.5f, 75.0f, 100, 90.0f), false);
    ASSERT_EQ(filter.isFiltered("SPBE", "SPB Market", false, 300.0f, 3.5f, 75.0f, 250, 90.0f), false);
    ASSERT_EQ(filter.isFiltered("SPBE", "SPB Market", false, 300.0f, 3.5f, 75.0f, 170, 60.0f), false);
    ASSERT_EQ(filter.isFiltered("SPBE", "SPB Market", false, 300.0f, 3.5f, 75.0f, 170, 99.0f), false);
    ASSERT_EQ(filter.isFiltered("SEPB", "SPB Market", false, 300.0f, 3.5f, 75.0f, 170, 90.0f), true);
    ASSERT_EQ(filter.isFiltered("SPBE", "SEB Market", false, 300.0f, 3.5f, 75.0f, 170, 90.0f), true);
    ASSERT_EQ(filter.isFiltered("SPBE", "SPB Market", false, 300.0f, 3.5f, 75.0f, 170, 90.0f), true);
    // clang-format on
}

TEST(Test_StockFilter, Test_equals)
{
    StockFilter filter;
    StockFilter filter2;

    filter.useTicker          = true;
    filter.ticker             = "BLAH";
    filter.useQualInvestor    = true;
    filter.qualInvestor       = QUAL_INVESTOR_ONLY_WITHOUT_STATUS;
    filter.usePrice           = true;
    filter.priceFrom          = 1.0f;
    filter.priceTo            = 2.0f;
    filter.useDayStartChange  = true;
    filter.dayStartChangeFrom = 3.0f;
    filter.dayStartChangeTo   = 4.0f;
    filter.useDateChange      = true;
    filter.dateChangeFrom     = 5.0f;
    filter.dateChangeTo       = 6.0f;
    filter.useTurnover        = true;
    filter.turnoverFrom       = 7;
    filter.turnoverTo         = 8;
    filter.usePayback         = true;
    filter.paybackFrom        = 9.0f;
    filter.paybackTo          = 10.0f;

    filter2.useTicker          = true;
    filter2.ticker             = "BLAH";
    filter2.useQualInvestor    = true;
    filter2.qualInvestor       = QUAL_INVESTOR_ONLY_WITHOUT_STATUS;
    filter2.usePrice           = true;
    filter2.priceFrom          = 1.0f;
    filter2.priceTo            = 2.0f;
    filter2.useDayStartChange  = true;
    filter2.dayStartChangeFrom = 3.0f;
    filter2.dayStartChangeTo   = 4.0f;
    filter2.useDateChange      = true;
    filter2.dateChangeFrom     = 5.0f;
    filter2.dateChangeTo       = 6.0f;
    filter2.useTurnover        = true;
    filter2.turnoverFrom       = 7;
    filter2.turnoverTo         = 8;
    filter2.usePayback         = true;
    filter2.paybackFrom        = 9.0f;
    filter2.paybackTo          = 10.0f;

    ASSERT_EQ(filter, filter2);

    filter2.useTicker = false;
    ASSERT_NE(filter, filter2);
    filter2.useTicker = true;
    ASSERT_EQ(filter, filter2);

    filter2.ticker = "HOOYAK";
    ASSERT_NE(filter, filter2);
    filter2.ticker = "BLAH";
    ASSERT_EQ(filter, filter2);

    filter2.useQualInvestor = false;
    ASSERT_NE(filter, filter2);
    filter2.useQualInvestor = true;
    ASSERT_EQ(filter, filter2);

    filter2.qualInvestor = QUAL_INVESTOR_ONLY_WITH_STATUS;
    ASSERT_NE(filter, filter2);
    filter2.qualInvestor = QUAL_INVESTOR_ONLY_WITHOUT_STATUS;
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

    filter2.useTurnover = false;
    ASSERT_NE(filter, filter2);
    filter2.useTurnover = true;
    ASSERT_EQ(filter, filter2);

    filter2.turnoverFrom = 100;
    ASSERT_NE(filter, filter2);
    filter2.turnoverFrom = 7;
    ASSERT_EQ(filter, filter2);

    filter2.turnoverTo = 100;
    ASSERT_NE(filter, filter2);
    filter2.turnoverTo = 8;
    ASSERT_EQ(filter, filter2);

    filter2.usePayback = false;
    ASSERT_NE(filter, filter2);
    filter2.usePayback = true;
    ASSERT_EQ(filter, filter2);

    filter2.paybackFrom = 100.0f;
    ASSERT_NE(filter, filter2);
    filter2.paybackFrom = 9.0f;
    ASSERT_EQ(filter, filter2);

    filter2.paybackTo = 100.0f;
    ASSERT_NE(filter, filter2);
    filter2.paybackTo = 10.0f;
    ASSERT_EQ(filter, filter2);
}
// NOLINTEND(readability-function-cognitive-complexity, readability-magic-numbers)
