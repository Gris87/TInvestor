#include "src/domain/stock/stocktableentry.h"

#include <gtest/gtest.h>



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity, readability-magic-numbers)
TEST(Test_StockTableEntry, Test_constructor_and_destructor)
{
    const StockTableEntry entry;

    // clang-format off
    ASSERT_EQ(entry.instrumentId,         "");
    ASSERT_EQ(entry.instrumentLogo,       nullptr);
    ASSERT_EQ(entry.instrumentTicker,     "");
    ASSERT_EQ(entry.instrumentName,       "");
    ASSERT_EQ(entry.forQualInvestorFlag,  false);
    ASSERT_EQ(entry.locked,               false);
    ASSERT_NEAR(entry.price,              0.0f, 0.0001f);
    ASSERT_NEAR(entry.dayChange,          0.0f, 0.0001f);
    ASSERT_NEAR(entry.dateChange,         0.0f, 0.0001f);
    ASSERT_EQ(entry.turnover,             0);
    ASSERT_NEAR(entry.payback,            0.0f, 0.0001f);
    ASSERT_NEAR(entry.dayStartPrice,      0.0f, 0.0001f);
    ASSERT_NEAR(entry.specifiedDatePrice, 0.0f, 0.0001f);
    ASSERT_EQ(entry.pricePrecision,       0);
    // clang-format on
}

TEST(Test_StockTableEntry, Test_copy_constructor)
{
    StockTableEntry entry;

    entry.instrumentId        = "a";
    entry.instrumentTicker    = "b";
    entry.instrumentName      = "c";
    entry.forQualInvestorFlag = true;
    entry.locked              = true;
    entry.price               = 1.0f;
    entry.dayChange           = 2.0f;
    entry.dateChange          = 3.0f;
    entry.turnover            = 4;
    entry.payback             = 5.0f;
    entry.dayStartPrice       = 6.0f;
    entry.specifiedDatePrice  = 7.0f;
    entry.pricePrecision      = 8;

    const StockTableEntry entry2(entry);

    // clang-format off
    ASSERT_EQ(entry2.instrumentId,         "a");
    ASSERT_EQ(entry2.instrumentLogo,       nullptr);
    ASSERT_EQ(entry2.instrumentTicker,     "b");
    ASSERT_EQ(entry2.instrumentName,       "c");
    ASSERT_EQ(entry2.forQualInvestorFlag,  true);
    ASSERT_EQ(entry2.locked,               true);
    ASSERT_NEAR(entry2.price,              1.0f, 0.0001f);
    ASSERT_NEAR(entry2.dayChange,          2.0f, 0.0001f);
    ASSERT_NEAR(entry2.dateChange,         3.0f, 0.0001f);
    ASSERT_EQ(entry2.turnover,             4);
    ASSERT_NEAR(entry2.payback,            5.0f, 0.0001f);
    ASSERT_NEAR(entry2.dayStartPrice,      6.0f, 0.0001f);
    ASSERT_NEAR(entry2.specifiedDatePrice, 7.0f, 0.0001f);
    ASSERT_EQ(entry2.pricePrecision,       8);
    // clang-format on
}

TEST(Test_StockTableEntry, Test_assign)
{
    StockTableEntry entry;
    StockTableEntry entry2;

    entry.instrumentId        = "a";
    entry.instrumentTicker    = "b";
    entry.instrumentName      = "c";
    entry.forQualInvestorFlag = true;
    entry.locked              = true;
    entry.price               = 1.0f;
    entry.dayChange           = 2.0f;
    entry.dateChange          = 3.0f;
    entry.turnover            = 4;
    entry.payback             = 5.0f;
    entry.dayStartPrice       = 6.0f;
    entry.specifiedDatePrice  = 7.0f;
    entry.pricePrecision      = 8;

    entry2 = entry;

    // clang-format off
    ASSERT_EQ(entry2.instrumentId,         "a");
    ASSERT_EQ(entry2.instrumentLogo,       nullptr);
    ASSERT_EQ(entry2.instrumentTicker,     "b");
    ASSERT_EQ(entry2.instrumentName,       "c");
    ASSERT_EQ(entry2.forQualInvestorFlag,  true);
    ASSERT_EQ(entry2.locked,               true);
    ASSERT_NEAR(entry2.price,              1.0f, 0.0001f);
    ASSERT_NEAR(entry2.dayChange,          2.0f, 0.0001f);
    ASSERT_NEAR(entry2.dateChange,         3.0f, 0.0001f);
    ASSERT_EQ(entry2.turnover,             4);
    ASSERT_NEAR(entry2.payback,            5.0f, 0.0001f);
    ASSERT_NEAR(entry2.dayStartPrice,      6.0f, 0.0001f);
    ASSERT_NEAR(entry2.specifiedDatePrice, 7.0f, 0.0001f);
    ASSERT_EQ(entry2.pricePrecision,       8);
    // clang-format on
}

TEST(Test_StockTableEntry, Test_equals)
{
    StockTableEntry entry;
    StockTableEntry entry2;

    entry.instrumentId        = "a";
    entry.instrumentTicker    = "b";
    entry.instrumentName      = "c";
    entry.forQualInvestorFlag = true;
    entry.locked              = true;
    entry.price               = 1.0f;
    entry.dayChange           = 2.0f;
    entry.dateChange          = 3.0f;
    entry.turnover            = 4;
    entry.payback             = 5.0f;
    entry.dayStartPrice       = 6.0f;
    entry.specifiedDatePrice  = 7.0f;
    entry.pricePrecision      = 8;

    entry2.instrumentId        = "a";
    entry2.instrumentTicker    = "b";
    entry2.instrumentName      = "c";
    entry2.forQualInvestorFlag = true;
    entry2.locked              = true;
    entry2.price               = 1.0f;
    entry2.dayChange           = 2.0f;
    entry2.dateChange          = 3.0f;
    entry2.turnover            = 4;
    entry2.payback             = 5.0f;
    entry2.dayStartPrice       = 6.0f;
    entry2.specifiedDatePrice  = 7.0f;
    entry2.pricePrecision      = 8;

    ASSERT_EQ(entry, entry2);

    entry2.instrumentId = "aaaa";
    ASSERT_NE(entry, entry2);
    entry2.instrumentId = "a";
    ASSERT_EQ(entry, entry2);

    entry2.instrumentTicker = "bbbb";
    ASSERT_NE(entry, entry2);
    entry2.instrumentTicker = "b";
    ASSERT_EQ(entry, entry2);

    entry2.instrumentName = "cccc";
    ASSERT_NE(entry, entry2);
    entry2.instrumentName = "c";
    ASSERT_EQ(entry, entry2);

    entry2.forQualInvestorFlag = false;
    ASSERT_NE(entry, entry2);
    entry2.forQualInvestorFlag = true;
    ASSERT_EQ(entry, entry2);

    entry2.locked = false;
    ASSERT_NE(entry, entry2);
    entry2.locked = true;
    ASSERT_EQ(entry, entry2);

    entry2.price = 1000.0f;
    ASSERT_NE(entry, entry2);
    entry2.price = 1.0f;
    ASSERT_EQ(entry, entry2);

    entry2.dayChange = 2000.0f;
    ASSERT_NE(entry, entry2);
    entry2.dayChange = 2.0f;
    ASSERT_EQ(entry, entry2);

    entry2.dateChange = 3000.0f;
    ASSERT_NE(entry, entry2);
    entry2.dateChange = 3.0f;
    ASSERT_EQ(entry, entry2);

    entry2.turnover = 4000;
    ASSERT_NE(entry, entry2);
    entry2.turnover = 4;
    ASSERT_EQ(entry, entry2);

    entry2.payback = 5000.0f;
    ASSERT_NE(entry, entry2);
    entry2.payback = 5.0f;
    ASSERT_EQ(entry, entry2);

    entry2.dayStartPrice = 6000.0f;
    ASSERT_NE(entry, entry2);
    entry2.dayStartPrice = 6.0f;
    ASSERT_EQ(entry, entry2);

    entry2.specifiedDatePrice = 7000.0f;
    ASSERT_NE(entry, entry2);
    entry2.specifiedDatePrice = 7.0f;
    ASSERT_EQ(entry, entry2);

    entry2.pricePrecision = -8;
    ASSERT_NE(entry, entry2);
    entry2.pricePrecision = 8;
    ASSERT_EQ(entry, entry2);
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity, readability-magic-numbers)
