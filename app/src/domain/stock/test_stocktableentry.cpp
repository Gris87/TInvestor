#include "src/domain/stock/stocktableentry.h"

#include <gtest/gtest.h>



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity, readability-magic-numbers)
class Test_StockTableEntry : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};



TEST_F(Test_StockTableEntry, Test_constructor_and_destructor)
{
    const StockTableEntry entry;

    // clang-format off
    ASSERT_EQ(entry.instrumentId,                "");
    ASSERT_EQ(entry.instrumentLogo,              nullptr);
    ASSERT_EQ(entry.instrumentTicker,            "");
    ASSERT_EQ(entry.instrumentName,              "");
    ASSERT_EQ(entry.forQualInvestorFlag,         false);
    ASSERT_EQ(entry.locked,                      false);
    ASSERT_NEAR(entry.price,                     0.0f, 0.0001f);
    ASSERT_NEAR(entry.dayChange,                 0.0f, 0.0001f);
    ASSERT_NEAR(entry.dateChange,                0.0f, 0.0001f);
    ASSERT_EQ(entry.turnover,                    0);
    ASSERT_NEAR(entry.payback,                   0.0f, 0.0001f);
    ASSERT_EQ(entry.dividends.createTimestamp,   0);
    ASSERT_EQ(entry.dividends.paymentTimestamp,  0);
    ASSERT_NEAR(entry.dividends.price,           0.0f, 0.0001f);
    ASSERT_NEAR(entry.dividends.yield,           0.0f, 0.0001f);
    ASSERT_EQ(entry.shorts.enabled,              false);
    ASSERT_EQ(entry.shorts.lastEnabledTimestamp, 0);
    ASSERT_NEAR(entry.dayStartPrice,             0.0f, 0.0001f);
    ASSERT_NEAR(entry.specifiedDatePrice,        0.0f, 0.0001f);
    ASSERT_EQ(entry.pricePrecision,              0);
    // clang-format on
}

TEST_F(Test_StockTableEntry, Test_copy_constructor)
{
    StockTableEntry entry;

    entry.instrumentId                = "a";
    entry.instrumentTicker            = "b";
    entry.instrumentName              = "c";
    entry.forQualInvestorFlag         = true;
    entry.locked                      = true;
    entry.price                       = 1.0f;
    entry.dayChange                   = 2.0f;
    entry.dateChange                  = 3.0f;
    entry.turnover                    = 4;
    entry.payback                     = 5.0f;
    entry.dividends.createTimestamp   = 6;
    entry.dividends.paymentTimestamp  = 7;
    entry.dividends.price             = 8.0f;
    entry.dividends.yield             = 9.0f;
    entry.shorts.enabled              = true;
    entry.shorts.lastEnabledTimestamp = 10;
    entry.dayStartPrice               = 11.0f;
    entry.specifiedDatePrice          = 12.0f;
    entry.pricePrecision              = 13;

    const StockTableEntry entry2(entry);

    // clang-format off
    ASSERT_EQ(entry2.instrumentId,                "a");
    ASSERT_EQ(entry2.instrumentLogo,              nullptr);
    ASSERT_EQ(entry2.instrumentTicker,            "b");
    ASSERT_EQ(entry2.instrumentName,              "c");
    ASSERT_EQ(entry2.forQualInvestorFlag,         true);
    ASSERT_EQ(entry2.locked,                      true);
    ASSERT_NEAR(entry2.price,                     1.0f, 0.0001f);
    ASSERT_NEAR(entry2.dayChange,                 2.0f, 0.0001f);
    ASSERT_NEAR(entry2.dateChange,                3.0f, 0.0001f);
    ASSERT_EQ(entry2.turnover,                    4);
    ASSERT_NEAR(entry2.payback,                   5.0f, 0.0001f);
    ASSERT_EQ(entry2.dividends.createTimestamp,   6);
    ASSERT_EQ(entry2.dividends.paymentTimestamp,  7);
    ASSERT_NEAR(entry2.dividends.price,           8.0f, 0.0001f);
    ASSERT_NEAR(entry2.dividends.yield,           9.0f, 0.0001f);
    ASSERT_EQ(entry2.shorts.enabled,              true);
    ASSERT_EQ(entry2.shorts.lastEnabledTimestamp, 10);
    ASSERT_NEAR(entry2.dayStartPrice,             11.0f, 0.0001f);
    ASSERT_NEAR(entry2.specifiedDatePrice,        12.0f, 0.0001f);
    ASSERT_EQ(entry2.pricePrecision,              13);
    // clang-format on
}

TEST_F(Test_StockTableEntry, Test_assign)
{
    StockTableEntry entry;
    StockTableEntry entry2;

    entry.instrumentId                = "a";
    entry.instrumentTicker            = "b";
    entry.instrumentName              = "c";
    entry.forQualInvestorFlag         = true;
    entry.locked                      = true;
    entry.price                       = 1.0f;
    entry.dayChange                   = 2.0f;
    entry.dateChange                  = 3.0f;
    entry.turnover                    = 4;
    entry.payback                     = 5.0f;
    entry.dividends.createTimestamp   = 6;
    entry.dividends.paymentTimestamp  = 7;
    entry.dividends.price             = 8.0f;
    entry.dividends.yield             = 9.0f;
    entry.shorts.enabled              = true;
    entry.shorts.lastEnabledTimestamp = 10;
    entry.dayStartPrice               = 11.0f;
    entry.specifiedDatePrice          = 12.0f;
    entry.pricePrecision              = 13;

    entry2 = entry;

    // clang-format off
    ASSERT_EQ(entry2.instrumentId,                "a");
    ASSERT_EQ(entry2.instrumentLogo,              nullptr);
    ASSERT_EQ(entry2.instrumentTicker,            "b");
    ASSERT_EQ(entry2.instrumentName,              "c");
    ASSERT_EQ(entry2.forQualInvestorFlag,         true);
    ASSERT_EQ(entry2.locked,                      true);
    ASSERT_NEAR(entry2.price,                     1.0f, 0.0001f);
    ASSERT_NEAR(entry2.dayChange,                 2.0f, 0.0001f);
    ASSERT_NEAR(entry2.dateChange,                3.0f, 0.0001f);
    ASSERT_EQ(entry2.turnover,                    4);
    ASSERT_NEAR(entry2.payback,                   5.0f, 0.0001f);
    ASSERT_EQ(entry2.dividends.createTimestamp,   6);
    ASSERT_EQ(entry2.dividends.paymentTimestamp,  7);
    ASSERT_NEAR(entry2.dividends.price,           8.0f, 0.0001f);
    ASSERT_NEAR(entry2.dividends.yield,           9.0f, 0.0001f);
    ASSERT_EQ(entry2.shorts.enabled,              true);
    ASSERT_EQ(entry2.shorts.lastEnabledTimestamp, 10);
    ASSERT_NEAR(entry2.dayStartPrice,             11.0f, 0.0001f);
    ASSERT_NEAR(entry2.specifiedDatePrice,        12.0f, 0.0001f);
    ASSERT_EQ(entry2.pricePrecision,              13);
    // clang-format on
}

TEST_F(Test_StockTableEntry, Test_equals)
{
    StockTableEntry entry;
    StockTableEntry entry2;

    entry.instrumentId                = "a";
    entry.instrumentTicker            = "b";
    entry.instrumentName              = "c";
    entry.forQualInvestorFlag         = true;
    entry.locked                      = true;
    entry.price                       = 1.0f;
    entry.dayChange                   = 2.0f;
    entry.dateChange                  = 3.0f;
    entry.turnover                    = 4;
    entry.payback                     = 5.0f;
    entry.dividends.createTimestamp   = 6;
    entry.dividends.paymentTimestamp  = 7;
    entry.dividends.price             = 8.0f;
    entry.dividends.yield             = 9.0f;
    entry.shorts.enabled              = true;
    entry.shorts.lastEnabledTimestamp = 10;
    entry.dayStartPrice               = 11.0f;
    entry.specifiedDatePrice          = 12.0f;
    entry.pricePrecision              = 13;

    entry2.instrumentId                = "a";
    entry2.instrumentTicker            = "b";
    entry2.instrumentName              = "c";
    entry2.forQualInvestorFlag         = true;
    entry2.locked                      = true;
    entry2.price                       = 1.0f;
    entry2.dayChange                   = 2.0f;
    entry2.dateChange                  = 3.0f;
    entry2.turnover                    = 4;
    entry2.payback                     = 5.0f;
    entry2.dividends.createTimestamp   = 6;
    entry2.dividends.paymentTimestamp  = 7;
    entry2.dividends.price             = 8.0f;
    entry2.dividends.yield             = 9.0f;
    entry2.shorts.enabled              = true;
    entry2.shorts.lastEnabledTimestamp = 10;
    entry2.dayStartPrice               = 11.0f;
    entry2.specifiedDatePrice          = 12.0f;
    entry2.pricePrecision              = 13;

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

    entry2.dividends.createTimestamp = 6000;
    ASSERT_NE(entry, entry2);
    entry2.dividends.createTimestamp = 6;
    ASSERT_EQ(entry, entry2);

    entry2.shorts.enabled = false;
    ASSERT_NE(entry, entry2);
    entry2.shorts.enabled = true;
    ASSERT_EQ(entry, entry2);

    entry2.dayStartPrice = 11000.0f;
    ASSERT_NE(entry, entry2);
    entry2.dayStartPrice = 11.0f;
    ASSERT_EQ(entry, entry2);

    entry2.specifiedDatePrice = 12000.0f;
    ASSERT_NE(entry, entry2);
    entry2.specifiedDatePrice = 12.0f;
    ASSERT_EQ(entry, entry2);

    entry2.pricePrecision = -13;
    ASSERT_NE(entry, entry2);
    entry2.pricePrecision = 13;
    ASSERT_EQ(entry, entry2);
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity, readability-magic-numbers)
