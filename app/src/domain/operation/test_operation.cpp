#include "src/domain/operation/operation.h"

#include <QDebug>
#include <QJsonDocument>
#include <gtest/gtest.h>



// NOLINTBEGIN(readability-magic-numbers)
TEST(Test_Operation, Test_constructor_and_destructor)
{
    const Operation operation;

    // clang-format off
    ASSERT_EQ(operation.timestamp,                       0);
    ASSERT_EQ(operation.instrumentId,                    "");
    ASSERT_EQ(operation.description,                     "");
    ASSERT_NEAR(operation.price,                         0, 0.0001f);
    ASSERT_EQ(operation.quantity,                        0);
    ASSERT_NEAR(operation.payment,                       0, 0.0001f);
    ASSERT_NEAR(operation.commission,                    0, 0.0001f);
    ASSERT_NEAR(operation.yield,                         0, 0.0001f);
    ASSERT_EQ(operation.moneyOnAccountRemained.units,    0);
    ASSERT_EQ(operation.moneyOnAccountRemained.nano,     0);
    ASSERT_EQ(operation.moneyOnAccountTotal.units,       0);
    ASSERT_EQ(operation.moneyOnAccountTotal.nano,        0);
    ASSERT_EQ(operation.pricePrecision,                  0);
    ASSERT_EQ(operation.paymentPrecision,                0);
    ASSERT_EQ(operation.commissionPrecision,             0);
    ASSERT_EQ(operation.yieldPrecision,                  0);
    ASSERT_EQ(operation.moneyOnAccountRemainedPrecision, 0);
    ASSERT_EQ(operation.moneyOnAccountTotalPrecision,    0);
    // clang-format on
}

TEST(Test_Operation, Test_copy_constructor)
{
    Operation operation;

    operation.timestamp                       = 1;
    operation.instrumentId                    = "a";
    operation.description                     = "b";
    operation.price                           = 2.0f;
    operation.quantity                        = 3;
    operation.payment                         = 4.0f;
    operation.commission                      = 5.0f;
    operation.yield                           = 6.0f;
    operation.moneyOnAccountRemained.units    = 7;
    operation.moneyOnAccountRemained.nano     = 8;
    operation.moneyOnAccountTotal.units       = 9;
    operation.moneyOnAccountTotal.nano        = 10;
    operation.pricePrecision                  = 11;
    operation.paymentPrecision                = 12;
    operation.commissionPrecision             = 13;
    operation.yieldPrecision                  = 14;
    operation.moneyOnAccountRemainedPrecision = 15;
    operation.moneyOnAccountTotalPrecision    = 16;

    const Operation operation2(operation);

    // clang-format off
    ASSERT_EQ(operation2.timestamp,                       1);
    ASSERT_EQ(operation2.instrumentId,                    "a");
    ASSERT_EQ(operation2.description,                     "b");
    ASSERT_NEAR(operation2.price,                         2, 0.0001f);
    ASSERT_EQ(operation2.quantity,                        3);
    ASSERT_NEAR(operation2.payment,                       4, 0.0001f);
    ASSERT_NEAR(operation2.commission,                    5, 0.0001f);
    ASSERT_NEAR(operation2.yield,                         6, 0.0001f);
    ASSERT_EQ(operation2.moneyOnAccountRemained.units,    7);
    ASSERT_EQ(operation2.moneyOnAccountRemained.nano,     8);
    ASSERT_EQ(operation2.moneyOnAccountTotal.units,       9);
    ASSERT_EQ(operation2.moneyOnAccountTotal.nano,        10);
    ASSERT_EQ(operation2.pricePrecision,                  11);
    ASSERT_EQ(operation2.paymentPrecision,                12);
    ASSERT_EQ(operation2.commissionPrecision,             13);
    ASSERT_EQ(operation2.yieldPrecision,                  14);
    ASSERT_EQ(operation2.moneyOnAccountRemainedPrecision, 15);
    ASSERT_EQ(operation2.moneyOnAccountTotalPrecision,    16);
    // clang-format on
}

TEST(Test_Operation, Test_assign)
{
    Operation operation;
    Operation operation2;

    operation.timestamp                       = 1;
    operation.instrumentId                    = "a";
    operation.description                     = "b";
    operation.price                           = 2.0f;
    operation.quantity                        = 3;
    operation.payment                         = 4.0f;
    operation.commission                      = 5.0f;
    operation.yield                           = 6.0f;
    operation.moneyOnAccountRemained.units    = 7;
    operation.moneyOnAccountRemained.nano     = 8;
    operation.moneyOnAccountTotal.units       = 9;
    operation.moneyOnAccountTotal.nano        = 10;
    operation.pricePrecision                  = 11;
    operation.paymentPrecision                = 12;
    operation.commissionPrecision             = 13;
    operation.yieldPrecision                  = 14;
    operation.moneyOnAccountRemainedPrecision = 15;
    operation.moneyOnAccountTotalPrecision    = 16;

    operation2 = operation;

    // clang-format off
    ASSERT_EQ(operation2.timestamp,                       1);
    ASSERT_EQ(operation2.instrumentId,                    "a");
    ASSERT_EQ(operation2.description,                     "b");
    ASSERT_NEAR(operation2.price,                         2, 0.0001f);
    ASSERT_EQ(operation2.quantity,                        3);
    ASSERT_NEAR(operation2.payment,                       4, 0.0001f);
    ASSERT_NEAR(operation2.commission,                    5, 0.0001f);
    ASSERT_NEAR(operation2.yield,                         6, 0.0001f);
    ASSERT_EQ(operation2.moneyOnAccountRemained.units,    7);
    ASSERT_EQ(operation2.moneyOnAccountRemained.nano,     8);
    ASSERT_EQ(operation2.moneyOnAccountTotal.units,       9);
    ASSERT_EQ(operation2.moneyOnAccountTotal.nano,        10);
    ASSERT_EQ(operation2.pricePrecision,                  11);
    ASSERT_EQ(operation2.paymentPrecision,                12);
    ASSERT_EQ(operation2.commissionPrecision,             13);
    ASSERT_EQ(operation2.yieldPrecision,                  14);
    ASSERT_EQ(operation2.moneyOnAccountRemainedPrecision, 15);
    ASSERT_EQ(operation2.moneyOnAccountTotalPrecision,    16);
    // clang-format on
}

TEST(Test_Operation, Test_fromJsonObject)
{
    Operation operation;

    // clang-format off
    ASSERT_EQ(operation.timestamp,                       0);
    ASSERT_EQ(operation.instrumentId,                    "");
    ASSERT_EQ(operation.description,                     "");
    ASSERT_NEAR(operation.price,                         0, 0.0001f);
    ASSERT_EQ(operation.quantity,                        0);
    ASSERT_NEAR(operation.payment,                       0, 0.0001f);
    ASSERT_NEAR(operation.commission,                    0, 0.0001f);
    ASSERT_NEAR(operation.yield,                         0, 0.0001f);
    ASSERT_EQ(operation.moneyOnAccountRemained.units,    0);
    ASSERT_EQ(operation.moneyOnAccountRemained.nano,     0);
    ASSERT_EQ(operation.moneyOnAccountTotal.units,       0);
    ASSERT_EQ(operation.moneyOnAccountTotal.nano,        0);
    ASSERT_EQ(operation.pricePrecision,                  0);
    ASSERT_EQ(operation.paymentPrecision,                0);
    ASSERT_EQ(operation.commissionPrecision,             0);
    ASSERT_EQ(operation.yieldPrecision,                  0);
    ASSERT_EQ(operation.moneyOnAccountRemainedPrecision, 0);
    ASSERT_EQ(operation.moneyOnAccountTotalPrecision,    0);
    // clang-format on

    const QString content =
        R"({"commission":5,"commissionPrecision":13,"description":"b","instrumentId":"a","moneyOnAccountRemained":{"nano":8,"units":7},"moneyOnAccountRemainedPrecision":15,"moneyOnAccountTotal":{"nano":10,"units":9},"moneyOnAccountTotalPrecision":16,"payment":4,"paymentPrecision":12,"price":2,"pricePrecision":11,"quantity":3,"timestamp":1,"yield":6,"yieldPrecision":14})";

    QJsonParseError     parseError;
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(content.toUtf8(), &parseError);

    ASSERT_EQ(parseError.error, QJsonParseError::NoError);
    operation.fromJsonObject(jsonDoc.object());

    // clang-format off
    ASSERT_EQ(operation.timestamp,                       1);
    ASSERT_EQ(operation.instrumentId,                    "a");
    ASSERT_EQ(operation.description,                     "b");
    ASSERT_NEAR(operation.price,                         2, 0.0001f);
    ASSERT_EQ(operation.quantity,                        3);
    ASSERT_NEAR(operation.payment,                       4, 0.0001f);
    ASSERT_NEAR(operation.commission,                    5, 0.0001f);
    ASSERT_NEAR(operation.yield,                         6, 0.0001f);
    ASSERT_EQ(operation.moneyOnAccountRemained.units,    7);
    ASSERT_EQ(operation.moneyOnAccountRemained.nano,     8);
    ASSERT_EQ(operation.moneyOnAccountTotal.units,       9);
    ASSERT_EQ(operation.moneyOnAccountTotal.nano,        10);
    ASSERT_EQ(operation.pricePrecision,                  11);
    ASSERT_EQ(operation.paymentPrecision,                12);
    ASSERT_EQ(operation.commissionPrecision,             13);
    ASSERT_EQ(operation.yieldPrecision,                  14);
    ASSERT_EQ(operation.moneyOnAccountRemainedPrecision, 15);
    ASSERT_EQ(operation.moneyOnAccountTotalPrecision,    16);
    // clang-format on
}

TEST(Test_Operation, Test_toJsonObject)
{
    Operation operation;

    operation.timestamp                       = 1;
    operation.instrumentId                    = "a";
    operation.description                     = "b";
    operation.price                           = 2.0f;
    operation.quantity                        = 3;
    operation.payment                         = 4.0f;
    operation.commission                      = 5.0f;
    operation.yield                           = 6.0f;
    operation.moneyOnAccountRemained.units    = 7;
    operation.moneyOnAccountRemained.nano     = 8;
    operation.moneyOnAccountTotal.units       = 9;
    operation.moneyOnAccountTotal.nano        = 10;
    operation.pricePrecision                  = 11;
    operation.paymentPrecision                = 12;
    operation.commissionPrecision             = 13;
    operation.yieldPrecision                  = 14;
    operation.moneyOnAccountRemainedPrecision = 15;
    operation.moneyOnAccountTotalPrecision    = 16;

    const QJsonObject   jsonObject = operation.toJsonObject();
    const QJsonDocument jsonDoc(jsonObject);

    const QString content = QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
    const QString expectedContent =
        R"({"commission":5,"commissionPrecision":13,"description":"b","instrumentId":"a","moneyOnAccountRemained":{"nano":8,"units":7},"moneyOnAccountRemainedPrecision":15,"moneyOnAccountTotal":{"nano":10,"units":9},"moneyOnAccountTotalPrecision":16,"payment":4,"paymentPrecision":12,"price":2,"pricePrecision":11,"quantity":3,"timestamp":1,"yield":6,"yieldPrecision":14})";

    ASSERT_EQ(content, expectedContent);
}

TEST(Test_Operation, Test_equals)
{
    Operation operation;
    Operation operation2;

    operation.timestamp                       = 1;
    operation.instrumentId                    = "a";
    operation.description                     = "b";
    operation.price                           = 2.0f;
    operation.quantity                        = 3;
    operation.payment                         = 4.0f;
    operation.commission                      = 5.0f;
    operation.yield                           = 6.0f;
    operation.moneyOnAccountRemained.units    = 7;
    operation.moneyOnAccountRemained.nano     = 8;
    operation.moneyOnAccountTotal.units       = 9;
    operation.moneyOnAccountTotal.nano        = 10;
    operation.pricePrecision                  = 11;
    operation.paymentPrecision                = 12;
    operation.commissionPrecision             = 13;
    operation.yieldPrecision                  = 14;
    operation.moneyOnAccountRemainedPrecision = 15;
    operation.moneyOnAccountTotalPrecision    = 16;

    operation2.timestamp                       = 1;
    operation2.instrumentId                    = "a";
    operation2.description                     = "b";
    operation2.price                           = 2.0f;
    operation2.quantity                        = 3;
    operation2.payment                         = 4.0f;
    operation2.commission                      = 5.0f;
    operation2.yield                           = 6.0f;
    operation2.moneyOnAccountRemained.units    = 7;
    operation2.moneyOnAccountRemained.nano     = 8;
    operation2.moneyOnAccountTotal.units       = 9;
    operation2.moneyOnAccountTotal.nano        = 10;
    operation2.pricePrecision                  = 11;
    operation2.paymentPrecision                = 12;
    operation2.commissionPrecision             = 13;
    operation2.yieldPrecision                  = 14;
    operation2.moneyOnAccountRemainedPrecision = 15;
    operation2.moneyOnAccountTotalPrecision    = 16;

    ASSERT_EQ(operation, operation2);

    operation2.timestamp = 1000;
    ASSERT_NE(operation, operation2);
    operation2.timestamp = 1;
    ASSERT_EQ(operation, operation2);

    operation2.instrumentId = "aaaa";
    ASSERT_NE(operation, operation2);
    operation2.instrumentId = "a";
    ASSERT_EQ(operation, operation2);

    operation2.description = "bbbb";
    ASSERT_NE(operation, operation2);
    operation2.description = "b";
    ASSERT_EQ(operation, operation2);

    operation2.price = 2222.0f;
    ASSERT_NE(operation, operation2);
    operation2.price = 2.0f;
    ASSERT_EQ(operation, operation2);

    operation2.quantity = 3333;
    ASSERT_NE(operation, operation2);
    operation2.quantity = 3;
    ASSERT_EQ(operation, operation2);

    operation2.payment = 4444.0f;
    ASSERT_NE(operation, operation2);
    operation2.payment = 4.0f;
    ASSERT_EQ(operation, operation2);

    operation2.commission = 5555.0f;
    ASSERT_NE(operation, operation2);
    operation2.commission = 5.0f;
    ASSERT_EQ(operation, operation2);

    operation2.yield = 6666.0f;
    ASSERT_NE(operation, operation2);
    operation2.yield = 6.0f;
    ASSERT_EQ(operation, operation2);

    operation2.moneyOnAccountRemained.units = 7777;
    ASSERT_NE(operation, operation2);
    operation2.moneyOnAccountRemained.units = 7;
    ASSERT_EQ(operation, operation2);

    operation2.moneyOnAccountRemained.nano = 8888;
    ASSERT_NE(operation, operation2);
    operation2.moneyOnAccountRemained.nano = 8;
    ASSERT_EQ(operation, operation2);

    operation2.moneyOnAccountTotal.units = 9999;
    ASSERT_NE(operation, operation2);
    operation2.moneyOnAccountTotal.units = 9;
    ASSERT_EQ(operation, operation2);

    operation2.moneyOnAccountTotal.nano = 10000;
    ASSERT_NE(operation, operation2);
    operation2.moneyOnAccountTotal.nano = 10;
    ASSERT_EQ(operation, operation2);

    operation2.pricePrecision = 111;
    ASSERT_NE(operation, operation2);
    operation2.pricePrecision = 11;
    ASSERT_EQ(operation, operation2);

    operation2.paymentPrecision = 112;
    ASSERT_NE(operation, operation2);
    operation2.paymentPrecision = 12;
    ASSERT_EQ(operation, operation2);

    operation2.commissionPrecision = 113;
    ASSERT_NE(operation, operation2);
    operation2.commissionPrecision = 13;
    ASSERT_EQ(operation, operation2);

    operation2.yieldPrecision = 114;
    ASSERT_NE(operation, operation2);
    operation2.yieldPrecision = 14;
    ASSERT_EQ(operation, operation2);

    operation2.moneyOnAccountRemainedPrecision = 115;
    ASSERT_NE(operation, operation2);
    operation2.moneyOnAccountRemainedPrecision = 15;
    ASSERT_EQ(operation, operation2);

    operation2.moneyOnAccountTotalPrecision = 116;
    ASSERT_NE(operation, operation2);
    operation2.moneyOnAccountTotalPrecision = 16;
    ASSERT_EQ(operation, operation2);
}
// NOLINTEND(readability-magic-numbers)
