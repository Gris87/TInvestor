#include "src/domain/portfolio/portfolio.h"

#include <QDebug>
#include <gtest/gtest.h>



// NOLINTBEGIN(readability-magic-numbers)
TEST(Test_Portfolio, Test_constructor_and_destructor)
{
    const Portfolio portfolio;

    ASSERT_EQ(portfolio.positions.size(), 0);
}

TEST(Test_Portfolio, Test_copy_constructor)
{
    Portfolio portfolio;

    PortfolioItem item;

    item.quantity = 1;
    item.price    = 2.0f;
    item.avgPrice = 3.0f;

    portfolio.positions["share"]["aaaaa"] = item;

    const Portfolio portfolio2(portfolio);

    // clang-format off
    ASSERT_EQ(portfolio2.positions.size(),                       1);
    ASSERT_EQ(portfolio2.positions["share"].size(),              1);
    ASSERT_EQ(portfolio2.positions["share"]["aaaaa"].quantity,   1);
    ASSERT_NEAR(portfolio2.positions["share"]["aaaaa"].price,    2, 0.0001f);
    ASSERT_NEAR(portfolio2.positions["share"]["aaaaa"].avgPrice, 3, 0.0001f);
    // clang-format on
}

TEST(Test_Portfolio, Test_assign)
{
    Portfolio portfolio;
    Portfolio portfolio2;

    PortfolioItem item;

    item.quantity = 1;
    item.price    = 2.0f;
    item.avgPrice = 3.0f;

    portfolio.positions["share"]["aaaaa"] = item;

    portfolio2 = portfolio;

    // clang-format off
    ASSERT_EQ(portfolio2.positions.size(),                       1);
    ASSERT_EQ(portfolio2.positions["share"].size(),              1);
    ASSERT_EQ(portfolio2.positions["share"]["aaaaa"].quantity,   1);
    ASSERT_NEAR(portfolio2.positions["share"]["aaaaa"].price,    2, 0.0001f);
    ASSERT_NEAR(portfolio2.positions["share"]["aaaaa"].avgPrice, 3, 0.0001f);
    // clang-format on
}

TEST(Test_Portfolio, Test_equals)
{
    Portfolio portfolio;
    Portfolio portfolio2;

    PortfolioItem item;
    PortfolioItem item2;

    item.quantity = 1;
    item.price    = 2.0f;
    item.avgPrice = 3.0f;

    item2.quantity = 1;
    item2.price    = 2.0f;
    item2.avgPrice = 3.0f;

    portfolio.positions["share"]["aaaaa"]  = item;
    portfolio2.positions["share"]["aaaaa"] = item2;

    ASSERT_EQ(portfolio, portfolio2);

    portfolio2.positions["share"]["aaaaa"].quantity = 1000;
    ASSERT_NE(portfolio, portfolio2);
    portfolio2.positions["share"]["aaaaa"].quantity = 1;
    ASSERT_EQ(portfolio, portfolio2);
}
// NOLINTEND(readability-magic-numbers)
