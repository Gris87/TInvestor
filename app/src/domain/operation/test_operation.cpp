#include "src/domain/operation/operation.h"

#include <QDebug>
#include <QJsonDocument>
#include <gtest/gtest.h>



// NOLINTBEGIN(readability-magic-numbers)
TEST(Test_Operation, Test_constructor_and_destructor)
{
    const Operation operation;

    // clang-format off
    ASSERT_EQ(operation.timestamp,           0);
    ASSERT_EQ(operation.instrumentId,        "");
    ASSERT_EQ(operation.description,         "");
    ASSERT_NEAR(operation.price,             0, 0.0001f);
    ASSERT_EQ(operation.quantity,            0);
    ASSERT_NEAR(operation.payment,           0, 0.0001f);
    ASSERT_NEAR(operation.commission,        0, 0.0001f);
    ASSERT_NEAR(operation.yield,             0, 0.0001f);
    ASSERT_EQ(operation.pricePrecision,      0);
    ASSERT_EQ(operation.paymentPrecision,    0);
    ASSERT_EQ(operation.commissionPrecision, 0);
    ASSERT_EQ(operation.yieldPrecision,      0);
    // clang-format on
}

TEST(Test_Operation, Test_copy_constructor)
{
    Operation operation;

    operation.timestamp           = 1;
    operation.instrumentId        = "a";
    operation.description         = "b";
    operation.price               = 2.0f;
    operation.quantity            = 3;
    operation.payment             = 4.0f;
    operation.commission          = 5.0f;
    operation.yield               = 6.0f;
    operation.pricePrecision      = 7;
    operation.paymentPrecision    = 8;
    operation.commissionPrecision = 9;
    operation.yieldPrecision      = 10;

    const Operation operation2(operation);

    // clang-format off
    ASSERT_EQ(operation2.timestamp,           1);
    ASSERT_EQ(operation2.instrumentId,        "a");
    ASSERT_EQ(operation2.description,         "b");
    ASSERT_NEAR(operation2.price,             2, 0.0001f);
    ASSERT_EQ(operation2.quantity,            3);
    ASSERT_NEAR(operation2.payment,           4, 0.0001f);
    ASSERT_NEAR(operation2.commission,        5, 0.0001f);
    ASSERT_NEAR(operation2.yield,             6, 0.0001f);
    ASSERT_EQ(operation2.pricePrecision,      7);
    ASSERT_EQ(operation2.paymentPrecision,    8);
    ASSERT_EQ(operation2.commissionPrecision, 9);
    ASSERT_EQ(operation2.yieldPrecision,      10);
    // clang-format on
}

TEST(Test_Operation, Test_assign)
{
    Operation operation;
    Operation operation2;

    operation.timestamp           = 1;
    operation.instrumentId        = "a";
    operation.description         = "b";
    operation.price               = 2.0f;
    operation.quantity            = 3;
    operation.payment             = 4.0f;
    operation.commission          = 5.0f;
    operation.yield               = 6.0f;
    operation.pricePrecision      = 7;
    operation.paymentPrecision    = 8;
    operation.commissionPrecision = 9;
    operation.yieldPrecision      = 10;

    operation2 = operation;

    // clang-format off
    ASSERT_EQ(operation2.timestamp,           1);
    ASSERT_EQ(operation2.instrumentId,        "a");
    ASSERT_EQ(operation2.description,         "b");
    ASSERT_NEAR(operation2.price,             2, 0.0001f);
    ASSERT_EQ(operation2.quantity,            3);
    ASSERT_NEAR(operation2.payment,           4, 0.0001f);
    ASSERT_NEAR(operation2.commission,        5, 0.0001f);
    ASSERT_NEAR(operation2.yield,             6, 0.0001f);
    ASSERT_EQ(operation2.pricePrecision,      7);
    ASSERT_EQ(operation2.paymentPrecision,    8);
    ASSERT_EQ(operation2.commissionPrecision, 9);
    ASSERT_EQ(operation2.yieldPrecision,      10);
    // clang-format on
}

TEST(Test_Operation, Test_fromJsonObject)
{
    Operation operation;

    // clang-format off
    ASSERT_EQ(operation.timestamp,           0);
    ASSERT_EQ(operation.instrumentId,        "");
    ASSERT_EQ(operation.description,         "");
    ASSERT_NEAR(operation.price,             0, 0.0001f);
    ASSERT_EQ(operation.quantity,            0);
    ASSERT_NEAR(operation.payment,           0, 0.0001f);
    ASSERT_NEAR(operation.commission,        0, 0.0001f);
    ASSERT_NEAR(operation.yield,             0, 0.0001f);
    ASSERT_EQ(operation.pricePrecision,      0);
    ASSERT_EQ(operation.paymentPrecision,    0);
    ASSERT_EQ(operation.commissionPrecision, 0);
    ASSERT_EQ(operation.yieldPrecision,      0);
    // clang-format on

    const QString content =
        R"({"commission":5,"commissionPrecision":9,"description":"b","instrumentId":"a","payment":4,"paymentPrecision":8,"price":2,"pricePrecision":7,"quantity":3,"timestamp":1,"yield":6,"yieldPrecision":10})";

    QJsonParseError     parseError;
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(content.toUtf8(), &parseError);

    ASSERT_EQ(parseError.error, QJsonParseError::NoError);
    operation.fromJsonObject(jsonDoc.object());

    // clang-format off
    ASSERT_EQ(operation.timestamp,           1);
    ASSERT_EQ(operation.instrumentId,        "a");
    ASSERT_EQ(operation.description,         "b");
    ASSERT_NEAR(operation.price,             2, 0.0001f);
    ASSERT_EQ(operation.quantity,            3);
    ASSERT_NEAR(operation.payment,           4, 0.0001f);
    ASSERT_NEAR(operation.commission,        5, 0.0001f);
    ASSERT_NEAR(operation.yield,             6, 0.0001f);
    ASSERT_EQ(operation.pricePrecision,      7);
    ASSERT_EQ(operation.paymentPrecision,    8);
    ASSERT_EQ(operation.commissionPrecision, 9);
    ASSERT_EQ(operation.yieldPrecision,      10);
    // clang-format on
}

TEST(Test_Operation, Test_toJsonObject)
{
    Operation operation;

    operation.timestamp           = 1;
    operation.instrumentId        = "a";
    operation.description         = "b";
    operation.price               = 2.0f;
    operation.quantity            = 3;
    operation.payment             = 4.0f;
    operation.commission          = 5.0f;
    operation.yield               = 6.0f;
    operation.pricePrecision      = 7;
    operation.paymentPrecision    = 8;
    operation.commissionPrecision = 9;
    operation.yieldPrecision      = 10;

    const QJsonObject   jsonObject = operation.toJsonObject();
    const QJsonDocument jsonDoc(jsonObject);

    const QString content = QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
    const QString expectedContent =
        R"({"commission":5,"commissionPrecision":9,"description":"b","instrumentId":"a","payment":4,"paymentPrecision":8,"price":2,"pricePrecision":7,"quantity":3,"timestamp":1,"yield":6,"yieldPrecision":10})";

    ASSERT_EQ(content, expectedContent);
}

TEST(Test_Operation, Test_equals)
{
    Operation operation;
    Operation operation2;

    operation.timestamp           = 1;
    operation.instrumentId        = "a";
    operation.description         = "b";
    operation.price               = 2.0f;
    operation.quantity            = 3;
    operation.payment             = 4.0f;
    operation.commission          = 5.0f;
    operation.yield               = 6.0f;
    operation.pricePrecision      = 7;
    operation.paymentPrecision    = 8;
    operation.commissionPrecision = 9;
    operation.yieldPrecision      = 10;

    operation2.timestamp           = 1;
    operation2.instrumentId        = "a";
    operation2.description         = "b";
    operation2.price               = 2.0f;
    operation2.quantity            = 3;
    operation2.payment             = 4.0f;
    operation2.commission          = 5.0f;
    operation2.yield               = 6.0f;
    operation2.pricePrecision      = 7;
    operation2.paymentPrecision    = 8;
    operation2.commissionPrecision = 9;
    operation2.yieldPrecision      = 10;

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

    operation2.pricePrecision = 77;
    ASSERT_NE(operation, operation2);
    operation2.pricePrecision = 7;
    ASSERT_EQ(operation, operation2);

    operation2.paymentPrecision = 88;
    ASSERT_NE(operation, operation2);
    operation2.paymentPrecision = 8;
    ASSERT_EQ(operation, operation2);

    operation2.commissionPrecision = 99;
    ASSERT_NE(operation, operation2);
    operation2.commissionPrecision = 9;
    ASSERT_EQ(operation, operation2);

    operation2.yieldPrecision = 100;
    ASSERT_NE(operation, operation2);
    operation2.yieldPrecision = 10;
    ASSERT_EQ(operation, operation2);
}
// NOLINTEND(readability-magic-numbers)
