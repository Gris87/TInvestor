#include "src/grpc/utils.h"

#include <gtest/gtest.h>



// NOLINTBEGIN(readability-magic-numbers)
class Test_GrpcUtils : public ::testing::Test
{
};



TEST_F(Test_GrpcUtils, Test_timeToTimestamp)
{
    google::protobuf::Timestamp timestamp;

    timestamp.set_seconds(1000);
    timestamp.set_nanos(500000000);

    ASSERT_EQ(timeToTimestamp(timestamp), 1000500);

    timestamp.set_seconds(5000);
    timestamp.set_nanos(0);

    ASSERT_EQ(timeToTimestamp(timestamp), 5000000);
}

TEST_F(Test_GrpcUtils, Test_quotationToFloat_tinkoff_money)
{
    tinkoff::MoneyValue money;

    money.set_units(1);
    money.set_nano(500000000);

    ASSERT_NEAR(quotationToFloat(money), 1.5f, 0.0001f);

    money.set_units(0);
    money.set_nano(10000000);

    ASSERT_NEAR(quotationToFloat(money), 0.01f, 0.0001f);

    money.set_units(10);
    money.set_nano(-10000000);

    ASSERT_NEAR(quotationToFloat(money), 9.99f, 0.0001f);

    money.set_units(-5);
    money.set_nano(-30000000);

    ASSERT_NEAR(quotationToFloat(money), -5.03f, 0.0001f);

    money.set_units(-5);
    money.set_nano(30000000);

    ASSERT_NEAR(quotationToFloat(money), -4.97f, 0.0001f);
}

TEST_F(Test_GrpcUtils, Test_quotationToFloat_tinkoff_quotation)
{
    tinkoff::Quotation quotation;

    quotation.set_units(1);
    quotation.set_nano(500000000);

    ASSERT_NEAR(quotationToFloat(quotation), 1.5f, 0.0001f);

    quotation.set_units(0);
    quotation.set_nano(10000000);

    ASSERT_NEAR(quotationToFloat(quotation), 0.01f, 0.0001f);

    quotation.set_units(10);
    quotation.set_nano(-10000000);

    ASSERT_NEAR(quotationToFloat(quotation), 9.99f, 0.0001f);

    quotation.set_units(-5);
    quotation.set_nano(-30000000);

    ASSERT_NEAR(quotationToFloat(quotation), -5.03f, 0.0001f);

    quotation.set_units(-5);
    quotation.set_nano(30000000);

    ASSERT_NEAR(quotationToFloat(quotation), -4.97f, 0.0001f);
}

TEST_F(Test_GrpcUtils, Test_quotationToFloat_quotation)
{
    // clang-format off
    ASSERT_NEAR(quotationToFloat(Quotation(1, 500000000)),  1.5f,   0.0001f);
    ASSERT_NEAR(quotationToFloat(Quotation(0, 10000000)),   0.01f,  0.0001f);
    ASSERT_NEAR(quotationToFloat(Quotation(10, -10000000)), 9.99f,  0.0001f);
    ASSERT_NEAR(quotationToFloat(Quotation(-5, -30000000)), -5.03f, 0.0001f);
    ASSERT_NEAR(quotationToFloat(Quotation(-5, 30000000)),  -4.97f, 0.0001f);
    // clang-format on
}

TEST_F(Test_GrpcUtils, Test_quotationToDouble_tinkoff_money)
{
    tinkoff::MoneyValue money;

    money.set_units(1);
    money.set_nano(500000000);

    ASSERT_NEAR(quotationToDouble(money), 1.5, 0.0001);

    money.set_units(0);
    money.set_nano(10000000);

    ASSERT_NEAR(quotationToDouble(money), 0.01, 0.0001);

    money.set_units(10);
    money.set_nano(-10000000);

    ASSERT_NEAR(quotationToDouble(money), 9.99, 0.0001);

    money.set_units(-5);
    money.set_nano(-30000000);

    ASSERT_NEAR(quotationToDouble(money), -5.03, 0.0001);

    money.set_units(-5);
    money.set_nano(30000000);

    ASSERT_NEAR(quotationToDouble(money), -4.97, 0.0001);
}

TEST_F(Test_GrpcUtils, Test_quotationToDouble_tinkoff_quotation)
{
    tinkoff::Quotation quotation;

    quotation.set_units(1);
    quotation.set_nano(500000000);

    ASSERT_NEAR(quotationToDouble(quotation), 1.5, 0.0001);

    quotation.set_units(0);
    quotation.set_nano(10000000);

    ASSERT_NEAR(quotationToDouble(quotation), 0.01, 0.0001);

    quotation.set_units(10);
    quotation.set_nano(-10000000);

    ASSERT_NEAR(quotationToDouble(quotation), 9.99, 0.0001);

    quotation.set_units(-5);
    quotation.set_nano(-30000000);

    ASSERT_NEAR(quotationToDouble(quotation), -5.03, 0.0001);

    quotation.set_units(-5);
    quotation.set_nano(30000000);

    ASSERT_NEAR(quotationToDouble(quotation), -4.97, 0.0001);
}

TEST_F(Test_GrpcUtils, Test_quotationToDouble_quotation)
{
    // clang-format off
    ASSERT_NEAR(quotationToDouble(Quotation(1, 500000000)),  1.5,   0.0001);
    ASSERT_NEAR(quotationToDouble(Quotation(0, 10000000)),   0.01,  0.0001);
    ASSERT_NEAR(quotationToDouble(Quotation(10, -10000000)), 9.99,  0.0001);
    ASSERT_NEAR(quotationToDouble(Quotation(-5, -30000000)), -5.03, 0.0001);
    ASSERT_NEAR(quotationToDouble(Quotation(-5, 30000000)),  -4.97, 0.0001);
    // clang-format on
}

TEST_F(Test_GrpcUtils, Test_quotationPrecision_tinkoff_money)
{
    tinkoff::MoneyValue money;

    money.set_units(1);
    money.set_nano(500000000);

    ASSERT_EQ(quotationPrecision(money), 2);

    money.set_units(0);
    money.set_nano(1000000);

    ASSERT_EQ(quotationPrecision(money), 3);

    money.set_units(0);
    money.set_nano(0);

    ASSERT_EQ(quotationPrecision(money), 2);

    money.set_units(-1);
    money.set_nano(-100000);

    ASSERT_EQ(quotationPrecision(money), 4);
}

TEST_F(Test_GrpcUtils, Test_quotationPrecision_tinkoff_quotation)
{
    tinkoff::Quotation quotation;

    quotation.set_units(1);
    quotation.set_nano(500000000);

    ASSERT_EQ(quotationPrecision(quotation), 2);

    quotation.set_units(0);
    quotation.set_nano(1000000);

    ASSERT_EQ(quotationPrecision(quotation), 3);

    quotation.set_units(0);
    quotation.set_nano(0);

    ASSERT_EQ(quotationPrecision(quotation), 2);

    quotation.set_units(-1);
    quotation.set_nano(-100000);

    ASSERT_EQ(quotationPrecision(quotation), 4);
}

TEST_F(Test_GrpcUtils, Test_quotationPrecision_quotation)
{
    // clang-format off
    ASSERT_EQ(quotationPrecision(Quotation(1, 500000000)), 2);
    ASSERT_EQ(quotationPrecision(Quotation(0, 1000000)),   3);
    ASSERT_EQ(quotationPrecision(Quotation(0, 0)),         2);
    ASSERT_EQ(quotationPrecision(Quotation(-1, -100000)),  4);
    // clang-format on
}

TEST_F(Test_GrpcUtils, Test_quotationConvert_tinkoff_money)
{
    tinkoff::MoneyValue money;

    money.set_units(1);
    money.set_nano(500000000);

    ASSERT_EQ(quotationConvert(money), Quotation(1, 500000000));

    money.set_units(0);
    money.set_nano(10000000);

    ASSERT_EQ(quotationConvert(money), Quotation(0, 10000000));

    money.set_units(-50);
    money.set_nano(-10000000);

    ASSERT_EQ(quotationConvert(money), Quotation(-50, -10000000));
}

TEST_F(Test_GrpcUtils, Test_quotationConvert_tinkoff_quotation)
{
    tinkoff::Quotation quotation;

    quotation.set_units(1);
    quotation.set_nano(500000000);

    ASSERT_EQ(quotationConvert(quotation), Quotation(1, 500000000));

    quotation.set_units(0);
    quotation.set_nano(10000000);

    ASSERT_EQ(quotationConvert(quotation), Quotation(0, 10000000));

    quotation.set_units(-50);
    quotation.set_nano(-10000000);

    ASSERT_EQ(quotationConvert(quotation), Quotation(-50, -10000000));
}

TEST_F(Test_GrpcUtils, Test_quotationFromDouble)
{
    // clang-format off
    ASSERT_EQ(quotationFromDouble(1.5),   Quotation(1, 500000000));
    ASSERT_EQ(quotationFromDouble(0.01),  Quotation(0, 10000000));
    ASSERT_EQ(quotationFromDouble(-10.3), Quotation(-10, -300000000));
    // clang-format on
}

TEST_F(Test_GrpcUtils, Test_quotationNegative_tinkoff_money)
{
    tinkoff::MoneyValue money;

    money.set_units(1);
    money.set_nano(500000000);

    ASSERT_EQ(quotationNegative(money), Quotation(-1, -500000000));

    money.set_units(-50);
    money.set_nano(-10000000);

    ASSERT_EQ(quotationNegative(money), Quotation(50, 10000000));
}

TEST_F(Test_GrpcUtils, Test_quotationNegative_tinkoff_quotation)
{
    tinkoff::Quotation quotation;

    quotation.set_units(1);
    quotation.set_nano(500000000);

    ASSERT_EQ(quotationNegative(quotation), Quotation(-1, -500000000));

    quotation.set_units(-50);
    quotation.set_nano(-10000000);

    ASSERT_EQ(quotationNegative(quotation), Quotation(50, 10000000));
}

TEST_F(Test_GrpcUtils, Test_quotationNegative_quotation)
{
    // clang-format off
    ASSERT_EQ(quotationNegative(Quotation(1, 500000000)),   Quotation(-1, -500000000));
    ASSERT_EQ(quotationNegative(Quotation(-50, -10000000)), Quotation(50, 10000000));
    // clang-format on
}

TEST_F(Test_GrpcUtils, Test_quotationSum_quotation_tinkoff_money)
{
    tinkoff::MoneyValue money;

    money.set_units(6);
    money.set_nano(500000000);

    ASSERT_EQ(quotationSum(Quotation(5, 400000000), money), Quotation(11, 900000000));

    money.set_units(6);
    money.set_nano(900000000);

    ASSERT_EQ(quotationSum(Quotation(5, 400000000), money), Quotation(12, 300000000));

    money.set_units(-2);
    money.set_nano(-500000000);

    ASSERT_EQ(quotationSum(Quotation(5, 400000000), money), Quotation(3, -100000000));

    money.set_units(5);
    money.set_nano(400000000);

    ASSERT_EQ(quotationSum(Quotation(-2, -500000000), money), Quotation(3, -100000000));
}

TEST_F(Test_GrpcUtils, Test_quotationSum_quotation_tinkoff_quotation)
{
    tinkoff::Quotation quotation;

    quotation.set_units(6);
    quotation.set_nano(500000000);

    ASSERT_EQ(quotationSum(Quotation(5, 400000000), quotation), Quotation(11, 900000000));

    quotation.set_units(6);
    quotation.set_nano(900000000);

    ASSERT_EQ(quotationSum(Quotation(5, 400000000), quotation), Quotation(12, 300000000));

    quotation.set_units(-2);
    quotation.set_nano(-500000000);

    ASSERT_EQ(quotationSum(Quotation(5, 400000000), quotation), Quotation(3, -100000000));

    quotation.set_units(5);
    quotation.set_nano(400000000);

    ASSERT_EQ(quotationSum(Quotation(-2, -500000000), quotation), Quotation(3, -100000000));
}

TEST_F(Test_GrpcUtils, Test_quotationSum_tinkoff_money_quotation)
{
    tinkoff::MoneyValue money;

    money.set_units(6);
    money.set_nano(500000000);

    ASSERT_EQ(quotationSum(money, Quotation(5, 400000000)), Quotation(11, 900000000));

    money.set_units(6);
    money.set_nano(900000000);

    ASSERT_EQ(quotationSum(money, Quotation(5, 400000000)), Quotation(12, 300000000));

    money.set_units(-2);
    money.set_nano(-500000000);

    ASSERT_EQ(quotationSum(money, Quotation(5, 400000000)), Quotation(3, -100000000));

    money.set_units(5);
    money.set_nano(400000000);

    ASSERT_EQ(quotationSum(money, Quotation(-2, -500000000)), Quotation(3, -100000000));
}

TEST_F(Test_GrpcUtils, Test_quotationSum_tinkoff_quotation_quotation)
{
    tinkoff::Quotation quotation;

    quotation.set_units(6);
    quotation.set_nano(500000000);

    ASSERT_EQ(quotationSum(quotation, Quotation(5, 400000000)), Quotation(11, 900000000));

    quotation.set_units(6);
    quotation.set_nano(900000000);

    ASSERT_EQ(quotationSum(quotation, Quotation(5, 400000000)), Quotation(12, 300000000));

    quotation.set_units(-2);
    quotation.set_nano(-500000000);

    ASSERT_EQ(quotationSum(quotation, Quotation(5, 400000000)), Quotation(3, -100000000));

    quotation.set_units(5);
    quotation.set_nano(400000000);

    ASSERT_EQ(quotationSum(quotation, Quotation(-2, -500000000)), Quotation(3, -100000000));
}

TEST_F(Test_GrpcUtils, Test_quotationSum_quotation_quotation)
{
    // clang-format off
    ASSERT_EQ(quotationSum(Quotation(6, 500000000),   Quotation(5, 400000000)),   Quotation(11, 900000000));
    ASSERT_EQ(quotationSum(Quotation(6, 900000000),   Quotation(5, 400000000)),   Quotation(12, 300000000));
    ASSERT_EQ(quotationSum(Quotation(-2, -500000000), Quotation(5, 400000000)),   Quotation(3, -100000000));
    ASSERT_EQ(quotationSum(Quotation(5, 400000000),   Quotation(-2, -500000000)), Quotation(3, -100000000));
    // clang-format on
}

TEST_F(Test_GrpcUtils, Test_quotationDiff_quotation_tinkoff_money)
{
    tinkoff::MoneyValue money;

    money.set_units(2);
    money.set_nano(500000000);

    ASSERT_EQ(quotationDiff(Quotation(5, 400000000), money), Quotation(3, -100000000));

    money.set_units(6);
    money.set_nano(900000000);

    ASSERT_EQ(quotationDiff(Quotation(5, 400000000), money), Quotation(-1, -500000000));

    money.set_units(-6);
    money.set_nano(-500000000);

    ASSERT_EQ(quotationDiff(Quotation(5, 400000000), money), Quotation(11, 900000000));

    money.set_units(5);
    money.set_nano(400000000);

    ASSERT_EQ(quotationDiff(Quotation(-6, -500000000), money), Quotation(-11, -900000000));
}

TEST_F(Test_GrpcUtils, Test_quotationDiff_quotation_tinkoff_quotation)
{
    tinkoff::Quotation quotation;

    quotation.set_units(2);
    quotation.set_nano(500000000);

    ASSERT_EQ(quotationDiff(Quotation(5, 400000000), quotation), Quotation(3, -100000000));

    quotation.set_units(6);
    quotation.set_nano(900000000);

    ASSERT_EQ(quotationDiff(Quotation(5, 400000000), quotation), Quotation(-1, -500000000));

    quotation.set_units(-6);
    quotation.set_nano(-500000000);

    ASSERT_EQ(quotationDiff(Quotation(5, 400000000), quotation), Quotation(11, 900000000));

    quotation.set_units(5);
    quotation.set_nano(400000000);

    ASSERT_EQ(quotationDiff(Quotation(-6, -500000000), quotation), Quotation(-11, -900000000));
}

TEST_F(Test_GrpcUtils, Test_quotationDiff_tinkoff_money_quotation)
{
    tinkoff::MoneyValue money;

    money.set_units(2);
    money.set_nano(500000000);

    ASSERT_EQ(quotationDiff(money, Quotation(5, 400000000)), Quotation(-3, 100000000));

    money.set_units(6);
    money.set_nano(900000000);

    ASSERT_EQ(quotationDiff(money, Quotation(5, 400000000)), Quotation(1, 500000000));

    money.set_units(-6);
    money.set_nano(-500000000);

    ASSERT_EQ(quotationDiff(money, Quotation(5, 400000000)), Quotation(-11, -900000000));

    money.set_units(5);
    money.set_nano(400000000);

    ASSERT_EQ(quotationDiff(money, Quotation(-6, -500000000)), Quotation(11, 900000000));
}

TEST_F(Test_GrpcUtils, Test_quotationDiff_tinkoff_quotation_quotation)
{
    tinkoff::Quotation quotation;

    quotation.set_units(2);
    quotation.set_nano(500000000);

    ASSERT_EQ(quotationDiff(quotation, Quotation(5, 400000000)), Quotation(-3, 100000000));

    quotation.set_units(6);
    quotation.set_nano(900000000);

    ASSERT_EQ(quotationDiff(quotation, Quotation(5, 400000000)), Quotation(1, 500000000));

    quotation.set_units(-6);
    quotation.set_nano(-500000000);

    ASSERT_EQ(quotationDiff(quotation, Quotation(5, 400000000)), Quotation(-11, -900000000));

    quotation.set_units(5);
    quotation.set_nano(400000000);

    ASSERT_EQ(quotationDiff(quotation, Quotation(-6, -500000000)), Quotation(11, 900000000));
}

TEST_F(Test_GrpcUtils, Test_quotationDiff_quotation_quotation)
{
    // clang-format off
    ASSERT_EQ(quotationDiff(Quotation(2, 500000000),   Quotation(5, 400000000)),   Quotation(-3, 100000000));
    ASSERT_EQ(quotationDiff(Quotation(6, 900000000),   Quotation(5, 400000000)),   Quotation(1, 500000000));
    ASSERT_EQ(quotationDiff(Quotation(-6, -500000000), Quotation(5, 400000000)),   Quotation(-11, -900000000));
    ASSERT_EQ(quotationDiff(Quotation(5, 400000000),   Quotation(-6, -500000000)), Quotation(11, 900000000));
    // clang-format on
}

TEST_F(Test_GrpcUtils, Test_quotationMultiply_tinkoff_money)
{
    tinkoff::MoneyValue money;

    money.set_units(1);
    money.set_nano(500000000);

    ASSERT_EQ(quotationMultiply(money, 2), Quotation(3, 0));

    money.set_units(0);
    money.set_nano(100000000);

    ASSERT_EQ(quotationMultiply(money, 1000), Quotation(100, 0));

    money.set_units(-5);
    money.set_nano(-300000000);

    ASSERT_EQ(quotationMultiply(money, 2), Quotation(-10, -600000000));

    money.set_units(-5);
    money.set_nano(-300000000);

    ASSERT_EQ(quotationMultiply(money, 5), Quotation(-26, -500000000));

    money.set_units(10);
    money.set_nano(-500000000);

    ASSERT_EQ(quotationMultiply(money, 5), Quotation(48, -500000000));
}

TEST_F(Test_GrpcUtils, Test_quotationMultiply_tinkoff_quotation)
{
    tinkoff::Quotation quotation;

    quotation.set_units(1);
    quotation.set_nano(500000000);

    ASSERT_EQ(quotationMultiply(quotation, 2), Quotation(3, 0));

    quotation.set_units(0);
    quotation.set_nano(100000000);

    ASSERT_EQ(quotationMultiply(quotation, 1000), Quotation(100, 0));

    quotation.set_units(-5);
    quotation.set_nano(-300000000);

    ASSERT_EQ(quotationMultiply(quotation, 2), Quotation(-10, -600000000));

    quotation.set_units(-5);
    quotation.set_nano(-300000000);

    ASSERT_EQ(quotationMultiply(quotation, 5), Quotation(-26, -500000000));

    quotation.set_units(10);
    quotation.set_nano(-500000000);

    ASSERT_EQ(quotationMultiply(quotation, 5), Quotation(48, -500000000));
}

TEST_F(Test_GrpcUtils, Test_quotationMultiply_quotation)
{
    // clang-format off
    ASSERT_EQ(quotationMultiply(Quotation(1, 500000000), 2),    Quotation(3, 0));
    ASSERT_EQ(quotationMultiply(Quotation(0, 100000000), 1000), Quotation(100, 0));
    ASSERT_EQ(quotationMultiply(Quotation(-5, -300000000), 2),  Quotation(-10, -600000000));
    ASSERT_EQ(quotationMultiply(Quotation(-5, -300000000), 5),  Quotation(-26, -500000000));
    ASSERT_EQ(quotationMultiply(Quotation(10, -500000000), 5),  Quotation(48, -500000000));
    // clang-format on
}

TEST_F(Test_GrpcUtils, Test_quotationDivide_tinkoff_money)
{
    tinkoff::MoneyValue money;

    money.set_units(1);
    money.set_nano(500000000);

    ASSERT_EQ(quotationDivide(money, 2), Quotation(0, 750000000));

    money.set_units(0);
    money.set_nano(100000000);

    ASSERT_EQ(quotationDivide(money, 1000), Quotation(0, 100000));

    money.set_units(-5);
    money.set_nano(-300000000);

    ASSERT_EQ(quotationDivide(money, 2), Quotation(-2, -650000000));

    money.set_units(-5);
    money.set_nano(-300000000);

    ASSERT_EQ(quotationDivide(money, 5), Quotation(-1, -60000000));

    money.set_units(10);
    money.set_nano(-500000000);

    ASSERT_EQ(quotationDivide(money, 5), Quotation(2, -100000000));
}

TEST_F(Test_GrpcUtils, Test_quotationDivide_tinkoff_quotation)
{
    tinkoff::Quotation quotation;

    quotation.set_units(1);
    quotation.set_nano(500000000);

    ASSERT_EQ(quotationDivide(quotation, 2), Quotation(0, 750000000));

    quotation.set_units(0);
    quotation.set_nano(100000000);

    ASSERT_EQ(quotationDivide(quotation, 1000), Quotation(0, 100000));

    quotation.set_units(-5);
    quotation.set_nano(-300000000);

    ASSERT_EQ(quotationDivide(quotation, 2), Quotation(-2, -650000000));

    quotation.set_units(-5);
    quotation.set_nano(-300000000);

    ASSERT_EQ(quotationDivide(quotation, 5), Quotation(-1, -60000000));

    quotation.set_units(10);
    quotation.set_nano(-500000000);

    ASSERT_EQ(quotationDivide(quotation, 5), Quotation(2, -100000000));
}

TEST_F(Test_GrpcUtils, Test_quotationDivide_quotation)
{
    // clang-format off
    ASSERT_EQ(quotationDivide(Quotation(1, 500000000), 2),    Quotation(0, 750000000));
    ASSERT_EQ(quotationDivide(Quotation(0, 100000000), 1000), Quotation(0, 100000));
    ASSERT_EQ(quotationDivide(Quotation(-5, -300000000), 2),  Quotation(-2, -650000000));
    ASSERT_EQ(quotationDivide(Quotation(-5, -300000000), 5),  Quotation(-1, -60000000));
    ASSERT_EQ(quotationDivide(Quotation(10, -500000000), 5),  Quotation(2, -100000000));
    // clang-format on
}
// NOLINTEND(readability-magic-numbers)
