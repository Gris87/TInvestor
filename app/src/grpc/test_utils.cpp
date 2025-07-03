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
    Quotation quotation;

    quotation.units = 1;
    quotation.nano  = 500000000;

    ASSERT_NEAR(quotationToFloat(quotation), 1.5f, 0.0001f);

    quotation.units = 0;
    quotation.nano  = 10000000;

    ASSERT_NEAR(quotationToFloat(quotation), 0.01f, 0.0001f);

    quotation.units = 10;
    quotation.nano  = -10000000;

    ASSERT_NEAR(quotationToFloat(quotation), 9.99f, 0.0001f);

    quotation.units = -5;
    quotation.nano  = -30000000;

    ASSERT_NEAR(quotationToFloat(quotation), -5.03f, 0.0001f);

    quotation.units = -5;
    quotation.nano  = 30000000;

    ASSERT_NEAR(quotationToFloat(quotation), -4.97f, 0.0001f);
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
    Quotation quotation;

    quotation.units = 1;
    quotation.nano  = 500000000;

    ASSERT_NEAR(quotationToDouble(quotation), 1.5, 0.0001);

    quotation.units = 0;
    quotation.nano  = 10000000;

    ASSERT_NEAR(quotationToDouble(quotation), 0.01, 0.0001);

    quotation.units = 10;
    quotation.nano  = -10000000;

    ASSERT_NEAR(quotationToDouble(quotation), 9.99, 0.0001);

    quotation.units = -5;
    quotation.nano  = -30000000;

    ASSERT_NEAR(quotationToDouble(quotation), -5.03, 0.0001);

    quotation.units = -5;
    quotation.nano  = 30000000;

    ASSERT_NEAR(quotationToDouble(quotation), -4.97, 0.0001);
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
    Quotation quotation;

    quotation.units = 1;
    quotation.nano  = 500000000;

    ASSERT_EQ(quotationPrecision(quotation), 2);

    quotation.units = 0;
    quotation.nano  = 1000000;

    ASSERT_EQ(quotationPrecision(quotation), 3);

    quotation.units = 0;
    quotation.nano  = 0;

    ASSERT_EQ(quotationPrecision(quotation), 2);

    quotation.units = -1;
    quotation.nano  = -100000;

    ASSERT_EQ(quotationPrecision(quotation), 4);
}

TEST_F(Test_GrpcUtils, Test_quotationConvert_tinkoff_money)
{
    tinkoff::MoneyValue money;
    Quotation           res;

    money.set_units(1);
    money.set_nano(500000000);

    res = quotationConvert(money);

    ASSERT_EQ(res.units, 1);
    ASSERT_EQ(res.nano, 500000000);

    money.set_units(0);
    money.set_nano(10000000);

    res = quotationConvert(money);

    ASSERT_EQ(res.units, 0);
    ASSERT_EQ(res.nano, 10000000);

    money.set_units(-50);
    money.set_nano(-10000000);

    res = quotationConvert(money);

    ASSERT_EQ(res.units, -50);
    ASSERT_EQ(res.nano, -10000000);
}

TEST_F(Test_GrpcUtils, Test_quotationConvert_tinkoff_quotation)
{
    tinkoff::Quotation quotation;
    Quotation          res;

    quotation.set_units(1);
    quotation.set_nano(500000000);

    res = quotationConvert(quotation);

    ASSERT_EQ(res.units, 1);
    ASSERT_EQ(res.nano, 500000000);

    quotation.set_units(0);
    quotation.set_nano(10000000);

    res = quotationConvert(quotation);

    ASSERT_EQ(res.units, 0);
    ASSERT_EQ(res.nano, 10000000);

    quotation.set_units(-50);
    quotation.set_nano(-10000000);

    res = quotationConvert(quotation);

    ASSERT_EQ(res.units, -50);
    ASSERT_EQ(res.nano, -10000000);
}

TEST_F(Test_GrpcUtils, Test_quotationFromDouble)
{
    Quotation res;

    res = quotationFromDouble(1.5);

    ASSERT_EQ(res.units, 1);
    ASSERT_EQ(res.nano, 500000000);

    res = quotationFromDouble(0.01);

    ASSERT_EQ(res.units, 0);
    ASSERT_EQ(res.nano, 10000000);

    res = quotationFromDouble(-10.3);

    ASSERT_EQ(res.units, -10);
    ASSERT_EQ(res.nano, -300000000);
}

TEST_F(Test_GrpcUtils, Test_quotationNegative_tinkoff_money)
{
    tinkoff::MoneyValue money;
    Quotation           res;

    money.set_units(1);
    money.set_nano(500000000);

    res = quotationNegative(money);

    ASSERT_EQ(res.units, -1);
    ASSERT_EQ(res.nano, -500000000);

    money.set_units(-50);
    money.set_nano(-10000000);

    res = quotationNegative(money);

    ASSERT_EQ(res.units, 50);
    ASSERT_EQ(res.nano, 10000000);
}

TEST_F(Test_GrpcUtils, Test_quotationNegative_tinkoff_quotation)
{
    tinkoff::Quotation quotation;
    Quotation          res;

    quotation.set_units(1);
    quotation.set_nano(500000000);

    res = quotationNegative(quotation);

    ASSERT_EQ(res.units, -1);
    ASSERT_EQ(res.nano, -500000000);

    quotation.set_units(-50);
    quotation.set_nano(-10000000);

    res = quotationNegative(quotation);

    ASSERT_EQ(res.units, 50);
    ASSERT_EQ(res.nano, 10000000);
}

TEST_F(Test_GrpcUtils, Test_quotationNegative_quotation)
{
    Quotation quotation;
    Quotation res;

    quotation.units = 1;
    quotation.nano  = 500000000;

    res = quotationNegative(quotation);

    ASSERT_EQ(res.units, -1);
    ASSERT_EQ(res.nano, -500000000);

    quotation.units = -50;
    quotation.nano  = -10000000;

    res = quotationNegative(quotation);

    ASSERT_EQ(res.units, 50);
    ASSERT_EQ(res.nano, 10000000);
}

TEST_F(Test_GrpcUtils, Test_quotationSum_quotation_tinkoff_money)
{
    Quotation           quotation;
    tinkoff::MoneyValue money;
    Quotation           res;

    quotation.units = 5;
    quotation.nano  = 400000000;

    money.set_units(6);
    money.set_nano(500000000);

    res = quotationSum(quotation, money);

    ASSERT_EQ(res.units, 11);
    ASSERT_EQ(res.nano, 900000000);

    quotation.units = 5;
    quotation.nano  = 400000000;

    money.set_units(6);
    money.set_nano(900000000);

    res = quotationSum(quotation, money);

    ASSERT_EQ(res.units, 12);
    ASSERT_EQ(res.nano, 300000000);

    quotation.units = 5;
    quotation.nano  = 400000000;

    money.set_units(-2);
    money.set_nano(-500000000);

    res = quotationSum(quotation, money);

    ASSERT_EQ(res.units, 3);
    ASSERT_EQ(res.nano, -100000000);

    quotation.units = -2;
    quotation.nano  = -500000000;

    money.set_units(5);
    money.set_nano(400000000);

    res = quotationSum(quotation, money);

    ASSERT_EQ(res.units, 3);
    ASSERT_EQ(res.nano, -100000000);
}

TEST_F(Test_GrpcUtils, Test_quotationSum_quotation_tinkoff_quotation)
{
    Quotation          quotation1;
    tinkoff::Quotation quotation2;
    Quotation          res;

    quotation1.units = 5;
    quotation1.nano  = 400000000;

    quotation2.set_units(6);
    quotation2.set_nano(500000000);

    res = quotationSum(quotation1, quotation2);

    ASSERT_EQ(res.units, 11);
    ASSERT_EQ(res.nano, 900000000);

    quotation1.units = 5;
    quotation1.nano  = 400000000;

    quotation2.set_units(6);
    quotation2.set_nano(900000000);

    res = quotationSum(quotation1, quotation2);

    ASSERT_EQ(res.units, 12);
    ASSERT_EQ(res.nano, 300000000);

    quotation1.units = 5;
    quotation1.nano  = 400000000;

    quotation2.set_units(-2);
    quotation2.set_nano(-500000000);

    res = quotationSum(quotation1, quotation2);

    ASSERT_EQ(res.units, 3);
    ASSERT_EQ(res.nano, -100000000);

    quotation1.units = -2;
    quotation1.nano  = -500000000;

    quotation2.set_units(5);
    quotation2.set_nano(400000000);

    res = quotationSum(quotation1, quotation2);

    ASSERT_EQ(res.units, 3);
    ASSERT_EQ(res.nano, -100000000);
}

TEST_F(Test_GrpcUtils, Test_quotationSum_tinkoff_money_quotation)
{
    tinkoff::MoneyValue money;
    Quotation           quotation;
    Quotation           res;

    quotation.units = 5;
    quotation.nano  = 400000000;

    money.set_units(6);
    money.set_nano(500000000);

    res = quotationSum(money, quotation);

    ASSERT_EQ(res.units, 11);
    ASSERT_EQ(res.nano, 900000000);

    quotation.units = 5;
    quotation.nano  = 400000000;

    money.set_units(6);
    money.set_nano(900000000);

    res = quotationSum(money, quotation);

    ASSERT_EQ(res.units, 12);
    ASSERT_EQ(res.nano, 300000000);

    quotation.units = 5;
    quotation.nano  = 400000000;

    money.set_units(-2);
    money.set_nano(-500000000);

    res = quotationSum(money, quotation);

    ASSERT_EQ(res.units, 3);
    ASSERT_EQ(res.nano, -100000000);

    quotation.units = -2;
    quotation.nano  = -500000000;

    money.set_units(5);
    money.set_nano(400000000);

    res = quotationSum(money, quotation);

    ASSERT_EQ(res.units, 3);
    ASSERT_EQ(res.nano, -100000000);
}

TEST_F(Test_GrpcUtils, Test_quotationSum_tinkoff_quotation_quotation)
{
    tinkoff::Quotation quotation1;
    Quotation          quotation2;
    Quotation          res;

    quotation2.units = 5;
    quotation2.nano  = 400000000;

    quotation1.set_units(6);
    quotation1.set_nano(500000000);

    res = quotationSum(quotation1, quotation2);

    ASSERT_EQ(res.units, 11);
    ASSERT_EQ(res.nano, 900000000);

    quotation2.units = 5;
    quotation2.nano  = 400000000;

    quotation1.set_units(6);
    quotation1.set_nano(900000000);

    res = quotationSum(quotation1, quotation2);

    ASSERT_EQ(res.units, 12);
    ASSERT_EQ(res.nano, 300000000);

    quotation2.units = 5;
    quotation2.nano  = 400000000;

    quotation1.set_units(-2);
    quotation1.set_nano(-500000000);

    res = quotationSum(quotation1, quotation2);

    ASSERT_EQ(res.units, 3);
    ASSERT_EQ(res.nano, -100000000);

    quotation2.units = -2;
    quotation2.nano  = -500000000;

    quotation1.set_units(5);
    quotation1.set_nano(400000000);

    res = quotationSum(quotation1, quotation2);

    ASSERT_EQ(res.units, 3);
    ASSERT_EQ(res.nano, -100000000);
}

TEST_F(Test_GrpcUtils, Test_quotationSum_quotation_quotation)
{
    Quotation quotation1;
    Quotation quotation2;
    Quotation res;

    quotation1.units = 5;
    quotation1.nano  = 400000000;

    quotation2.units = 6;
    quotation2.nano  = 500000000;

    res = quotationSum(quotation1, quotation2);

    ASSERT_EQ(res.units, 11);
    ASSERT_EQ(res.nano, 900000000);

    quotation1.units = 5;
    quotation1.nano  = 400000000;

    quotation2.units = 6;
    quotation2.nano  = 900000000;

    res = quotationSum(quotation1, quotation2);

    ASSERT_EQ(res.units, 12);
    ASSERT_EQ(res.nano, 300000000);

    quotation1.units = 5;
    quotation1.nano  = 400000000;

    quotation2.units = -2;
    quotation2.nano  = -500000000;

    res = quotationSum(quotation1, quotation2);

    ASSERT_EQ(res.units, 3);
    ASSERT_EQ(res.nano, -100000000);

    quotation1.units = -2;
    quotation1.nano  = -500000000;

    quotation2.units = 5;
    quotation2.nano  = 400000000;

    res = quotationSum(quotation1, quotation2);

    ASSERT_EQ(res.units, 3);
    ASSERT_EQ(res.nano, -100000000);
}

TEST_F(Test_GrpcUtils, Test_quotationDiff_quotation_tinkoff_money)
{
    Quotation           quotation;
    tinkoff::MoneyValue money;
    Quotation           res;

    quotation.units = 5;
    quotation.nano  = 400000000;

    money.set_units(2);
    money.set_nano(500000000);

    res = quotationDiff(quotation, money);

    ASSERT_EQ(res.units, 3);
    ASSERT_EQ(res.nano, -100000000);

    quotation.units = 5;
    quotation.nano  = 400000000;

    money.set_units(6);
    money.set_nano(900000000);

    res = quotationDiff(quotation, money);

    ASSERT_EQ(res.units, -1);
    ASSERT_EQ(res.nano, -500000000);

    quotation.units = 5;
    quotation.nano  = 400000000;

    money.set_units(-6);
    money.set_nano(-500000000);

    res = quotationDiff(quotation, money);

    ASSERT_EQ(res.units, 11);
    ASSERT_EQ(res.nano, 900000000);

    quotation.units = -6;
    quotation.nano  = -500000000;

    money.set_units(5);
    money.set_nano(400000000);

    res = quotationDiff(quotation, money);

    ASSERT_EQ(res.units, -11);
    ASSERT_EQ(res.nano, -900000000);
}

TEST_F(Test_GrpcUtils, Test_quotationDiff_quotation_tinkoff_quotation)
{
    Quotation          quotation1;
    tinkoff::Quotation quotation2;
    Quotation          res;

    quotation1.units = 5;
    quotation1.nano  = 400000000;

    quotation2.set_units(2);
    quotation2.set_nano(500000000);

    res = quotationDiff(quotation1, quotation2);

    ASSERT_EQ(res.units, 3);
    ASSERT_EQ(res.nano, -100000000);

    quotation1.units = 5;
    quotation1.nano  = 400000000;

    quotation2.set_units(6);
    quotation2.set_nano(900000000);

    res = quotationDiff(quotation1, quotation2);

    ASSERT_EQ(res.units, -1);
    ASSERT_EQ(res.nano, -500000000);

    quotation1.units = 5;
    quotation1.nano  = 400000000;

    quotation2.set_units(-6);
    quotation2.set_nano(-500000000);

    res = quotationDiff(quotation1, quotation2);

    ASSERT_EQ(res.units, 11);
    ASSERT_EQ(res.nano, 900000000);

    quotation1.units = -6;
    quotation1.nano  = -500000000;

    quotation2.set_units(5);
    quotation2.set_nano(400000000);

    res = quotationDiff(quotation1, quotation2);

    ASSERT_EQ(res.units, -11);
    ASSERT_EQ(res.nano, -900000000);
}

TEST_F(Test_GrpcUtils, Test_quotationDiff_tinkoff_money_quotation)
{
    tinkoff::MoneyValue money;
    Quotation           quotation;
    Quotation           res;

    quotation.units = 5;
    quotation.nano  = 400000000;

    money.set_units(2);
    money.set_nano(500000000);

    res = quotationDiff(money, quotation);

    ASSERT_EQ(res.units, -3);
    ASSERT_EQ(res.nano, 100000000);

    quotation.units = 5;
    quotation.nano  = 400000000;

    money.set_units(6);
    money.set_nano(900000000);

    res = quotationDiff(money, quotation);

    ASSERT_EQ(res.units, 1);
    ASSERT_EQ(res.nano, 500000000);

    quotation.units = 5;
    quotation.nano  = 400000000;

    money.set_units(-6);
    money.set_nano(-500000000);

    res = quotationDiff(money, quotation);

    ASSERT_EQ(res.units, -11);
    ASSERT_EQ(res.nano, -900000000);

    quotation.units = -6;
    quotation.nano  = -500000000;

    money.set_units(5);
    money.set_nano(400000000);

    res = quotationDiff(money, quotation);

    ASSERT_EQ(res.units, 11);
    ASSERT_EQ(res.nano, 900000000);
}

TEST_F(Test_GrpcUtils, Test_quotationDiff_tinkoff_quotation_quotation)
{
    tinkoff::Quotation quotation1;
    Quotation          quotation2;
    Quotation          res;

    quotation2.units = 5;
    quotation2.nano  = 400000000;

    quotation1.set_units(2);
    quotation1.set_nano(500000000);

    res = quotationDiff(quotation1, quotation2);

    ASSERT_EQ(res.units, -3);
    ASSERT_EQ(res.nano, 100000000);

    quotation2.units = 5;
    quotation2.nano  = 400000000;

    quotation1.set_units(6);
    quotation1.set_nano(900000000);

    res = quotationDiff(quotation1, quotation2);

    ASSERT_EQ(res.units, 1);
    ASSERT_EQ(res.nano, 500000000);

    quotation2.units = 5;
    quotation2.nano  = 400000000;

    quotation1.set_units(-6);
    quotation1.set_nano(-500000000);

    res = quotationDiff(quotation1, quotation2);

    ASSERT_EQ(res.units, -11);
    ASSERT_EQ(res.nano, -900000000);

    quotation2.units = -6;
    quotation2.nano  = -500000000;

    quotation1.set_units(5);
    quotation1.set_nano(400000000);

    res = quotationDiff(quotation1, quotation2);

    ASSERT_EQ(res.units, 11);
    ASSERT_EQ(res.nano, 900000000);
}

TEST_F(Test_GrpcUtils, Test_quotationDiff_quotation_quotation)
{
    Quotation quotation1;
    Quotation quotation2;
    Quotation res;

    quotation2.units = 5;
    quotation2.nano  = 400000000;

    quotation1.units = 2;
    quotation1.nano  = 500000000;

    res = quotationDiff(quotation1, quotation2);

    ASSERT_EQ(res.units, -3);
    ASSERT_EQ(res.nano, 100000000);

    quotation2.units = 5;
    quotation2.nano  = 400000000;

    quotation1.units = 6;
    quotation1.nano  = 900000000;

    res = quotationDiff(quotation1, quotation2);

    ASSERT_EQ(res.units, 1);
    ASSERT_EQ(res.nano, 500000000);

    quotation2.units = 5;
    quotation2.nano  = 400000000;

    quotation1.units = -6;
    quotation1.nano  = -500000000;

    res = quotationDiff(quotation1, quotation2);

    ASSERT_EQ(res.units, -11);
    ASSERT_EQ(res.nano, -900000000);

    quotation2.units = -6;
    quotation2.nano  = -500000000;

    quotation1.units = 5;
    quotation1.nano  = 400000000;

    res = quotationDiff(quotation1, quotation2);

    ASSERT_EQ(res.units, 11);
    ASSERT_EQ(res.nano, 900000000);
}

TEST_F(Test_GrpcUtils, Test_quotationMultiply_tinkoff_money)
{
    tinkoff::MoneyValue money;
    Quotation           res;

    money.set_units(1);
    money.set_nano(500000000);

    res = quotationMultiply(money, 2);

    ASSERT_EQ(res.units, 3);
    ASSERT_EQ(res.nano, 0);

    money.set_units(0);
    money.set_nano(100000000);

    res = quotationMultiply(money, 1000);

    ASSERT_EQ(res.units, 100);
    ASSERT_EQ(res.nano, 0);

    money.set_units(-5);
    money.set_nano(-300000000);

    res = quotationMultiply(money, 2);

    ASSERT_EQ(res.units, -10);
    ASSERT_EQ(res.nano, -600000000);

    money.set_units(-5);
    money.set_nano(-300000000);

    res = quotationMultiply(money, 5);

    ASSERT_EQ(res.units, -26);
    ASSERT_EQ(res.nano, -500000000);

    money.set_units(10);
    money.set_nano(-500000000);

    res = quotationMultiply(money, 5);

    ASSERT_EQ(res.units, 48);
    ASSERT_EQ(res.nano, -500000000);
}

TEST_F(Test_GrpcUtils, Test_quotationMultiply_tinkoff_quotation)
{
    tinkoff::Quotation quotation;
    Quotation          res;

    quotation.set_units(1);
    quotation.set_nano(500000000);

    res = quotationMultiply(quotation, 2);

    ASSERT_EQ(res.units, 3);
    ASSERT_EQ(res.nano, 0);

    quotation.set_units(0);
    quotation.set_nano(100000000);

    res = quotationMultiply(quotation, 1000);

    ASSERT_EQ(res.units, 100);
    ASSERT_EQ(res.nano, 0);

    quotation.set_units(-5);
    quotation.set_nano(-300000000);

    res = quotationMultiply(quotation, 2);

    ASSERT_EQ(res.units, -10);
    ASSERT_EQ(res.nano, -600000000);

    quotation.set_units(-5);
    quotation.set_nano(-300000000);

    res = quotationMultiply(quotation, 5);

    ASSERT_EQ(res.units, -26);
    ASSERT_EQ(res.nano, -500000000);

    quotation.set_units(10);
    quotation.set_nano(-500000000);

    res = quotationMultiply(quotation, 5);

    ASSERT_EQ(res.units, 48);
    ASSERT_EQ(res.nano, -500000000);
}

TEST_F(Test_GrpcUtils, Test_quotationMultiply_quotation)
{
    Quotation quotation;
    Quotation res;

    quotation.units = 1;
    quotation.nano  = 500000000;

    res = quotationMultiply(quotation, 2);

    ASSERT_EQ(res.units, 3);
    ASSERT_EQ(res.nano, 0);

    quotation.units = 0;
    quotation.nano  = 100000000;

    res = quotationMultiply(quotation, 1000);

    ASSERT_EQ(res.units, 100);
    ASSERT_EQ(res.nano, 0);

    quotation.units = -5;
    quotation.nano  = -300000000;

    res = quotationMultiply(quotation, 2);

    ASSERT_EQ(res.units, -10);
    ASSERT_EQ(res.nano, -600000000);

    quotation.units = -5;
    quotation.nano  = -300000000;

    res = quotationMultiply(quotation, 5);

    ASSERT_EQ(res.units, -26);
    ASSERT_EQ(res.nano, -500000000);

    quotation.units = 10;
    quotation.nano  = -500000000;

    res = quotationMultiply(quotation, 5);

    ASSERT_EQ(res.units, 48);
    ASSERT_EQ(res.nano, -500000000);
}

TEST_F(Test_GrpcUtils, Test_quotationDivide_tinkoff_money)
{
    tinkoff::MoneyValue money;
    Quotation           res;

    money.set_units(1);
    money.set_nano(500000000);

    res = quotationDivide(money, 2);

    ASSERT_EQ(res.units, 0);
    ASSERT_EQ(res.nano, 750000000);

    money.set_units(0);
    money.set_nano(100000000);

    res = quotationDivide(money, 1000);

    ASSERT_EQ(res.units, 0);
    ASSERT_EQ(res.nano, 100000);

    money.set_units(-5);
    money.set_nano(-300000000);

    res = quotationDivide(money, 2);

    ASSERT_EQ(res.units, -2);
    ASSERT_EQ(res.nano, -650000000);

    money.set_units(-5);
    money.set_nano(-300000000);

    res = quotationDivide(money, 5);

    ASSERT_EQ(res.units, -1);
    ASSERT_EQ(res.nano, -60000000);

    money.set_units(10);
    money.set_nano(-500000000);

    res = quotationDivide(money, 5);

    ASSERT_EQ(res.units, 2);
    ASSERT_EQ(res.nano, -100000000);
}

TEST_F(Test_GrpcUtils, Test_quotationDivide_tinkoff_quotation)
{
    tinkoff::Quotation quotation;
    Quotation          res;

    quotation.set_units(1);
    quotation.set_nano(500000000);

    res = quotationDivide(quotation, 2);

    ASSERT_EQ(res.units, 0);
    ASSERT_EQ(res.nano, 750000000);

    quotation.set_units(0);
    quotation.set_nano(100000000);

    res = quotationDivide(quotation, 1000);

    ASSERT_EQ(res.units, 0);
    ASSERT_EQ(res.nano, 100000);

    quotation.set_units(-5);
    quotation.set_nano(-300000000);

    res = quotationDivide(quotation, 2);

    ASSERT_EQ(res.units, -2);
    ASSERT_EQ(res.nano, -650000000);

    quotation.set_units(-5);
    quotation.set_nano(-300000000);

    res = quotationDivide(quotation, 5);

    ASSERT_EQ(res.units, -1);
    ASSERT_EQ(res.nano, -60000000);

    quotation.set_units(10);
    quotation.set_nano(-500000000);

    res = quotationDivide(quotation, 5);

    ASSERT_EQ(res.units, 2);
    ASSERT_EQ(res.nano, -100000000);
}

TEST_F(Test_GrpcUtils, Test_quotationDivide_quotation)
{
    Quotation quotation;
    Quotation res;

    quotation.units = 1;
    quotation.nano  = 500000000;

    res = quotationDivide(quotation, 2);

    ASSERT_EQ(res.units, 0);
    ASSERT_EQ(res.nano, 750000000);

    quotation.units = 0;
    quotation.nano  = 100000000;

    res = quotationDivide(quotation, 1000);

    ASSERT_EQ(res.units, 0);
    ASSERT_EQ(res.nano, 100000);

    quotation.units = -5;
    quotation.nano  = -300000000;

    res = quotationDivide(quotation, 2);

    ASSERT_EQ(res.units, -2);
    ASSERT_EQ(res.nano, -650000000);

    quotation.units = -5;
    quotation.nano  = -300000000;

    res = quotationDivide(quotation, 5);

    ASSERT_EQ(res.units, -1);
    ASSERT_EQ(res.nano, -60000000);

    quotation.units = 10;
    quotation.nano  = -500000000;

    res = quotationDivide(quotation, 5);

    ASSERT_EQ(res.units, 2);
    ASSERT_EQ(res.nano, -100000000);
}
// NOLINTEND(readability-magic-numbers)
