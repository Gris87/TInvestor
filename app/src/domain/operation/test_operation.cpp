#include "src/domain/operation/operation.h"

#include <QDebug>
#include <QJsonDocument>
#include <gtest/gtest.h>



// NOLINTBEGIN(readability-magic-numbers)
TEST(Test_Operation, Test_constructor_and_destructor)
{
    const Operation operation;

    // clang-format off
    ASSERT_EQ(operation.timestamp,                    0);
    ASSERT_EQ(operation.instrumentId,                 "");
    ASSERT_EQ(operation.description,                  "");
    ASSERT_NEAR(operation.price,                      0, 0.0001f);
    ASSERT_EQ(operation.avgPrice.units,               0);
    ASSERT_EQ(operation.avgPrice.nano,                0);
    ASSERT_EQ(operation.quantity,                     0);
    ASSERT_EQ(operation.remainedQuantity,             0);
    ASSERT_NEAR(operation.payment,                    0, 0.0001f);
    ASSERT_NEAR(operation.commission,                 0, 0.0001f);
    ASSERT_NEAR(operation.yield,                      0, 0.0001f);
    ASSERT_NEAR(operation.yieldWithCommission,        0, 0.0001f);
    ASSERT_NEAR(operation.yieldWithCommissionPercent, 0, 0.0001f);
    ASSERT_EQ(operation.remainedMoney.units,          0);
    ASSERT_EQ(operation.remainedMoney.nano,           0);
    ASSERT_EQ(operation.totalMoney.units,             0);
    ASSERT_EQ(operation.totalMoney.nano,              0);
    ASSERT_EQ(operation.pricePrecision,               0);
    ASSERT_EQ(operation.avgPricePrecision,            0);
    ASSERT_EQ(operation.paymentPrecision,             0);
    ASSERT_EQ(operation.commissionPrecision,          0);
    ASSERT_EQ(operation.yieldPrecision,               0);
    ASSERT_EQ(operation.yieldWithCommissionPrecision, 0);
    ASSERT_EQ(operation.remainedMoneyPrecision,       0);
    ASSERT_EQ(operation.totalMoneyPrecision,          0);
    // clang-format on
}

TEST(Test_Operation, Test_copy_constructor)
{
    Operation operation;

    operation.timestamp                    = 1;
    operation.instrumentId                 = "a";
    operation.description                  = "b";
    operation.price                        = 2.0f;
    operation.avgPrice.units               = 3;
    operation.avgPrice.nano                = 4;
    operation.quantity                     = 5;
    operation.remainedQuantity             = 6;
    operation.payment                      = 7.0f;
    operation.commission                   = 8.0f;
    operation.yield                        = 9.0f;
    operation.yieldWithCommission          = 10.0f;
    operation.yieldWithCommissionPercent   = 11.0f;
    operation.remainedMoney.units          = 12;
    operation.remainedMoney.nano           = 13;
    operation.totalMoney.units             = 14;
    operation.totalMoney.nano              = 15;
    operation.pricePrecision               = 16;
    operation.avgPricePrecision            = 17;
    operation.paymentPrecision             = 18;
    operation.commissionPrecision          = 19;
    operation.yieldPrecision               = 20;
    operation.yieldWithCommissionPrecision = 21;
    operation.remainedMoneyPrecision       = 22;
    operation.totalMoneyPrecision          = 23;

    const Operation operation2(operation);

    // clang-format off
    ASSERT_EQ(operation2.timestamp,                    1);
    ASSERT_EQ(operation2.instrumentId,                 "a");
    ASSERT_EQ(operation2.description,                  "b");
    ASSERT_NEAR(operation2.price,                      2, 0.0001f);
    ASSERT_EQ(operation2.avgPrice.units,               3);
    ASSERT_EQ(operation2.avgPrice.nano,                4);
    ASSERT_EQ(operation2.quantity,                     5);
    ASSERT_EQ(operation2.remainedQuantity,             6);
    ASSERT_NEAR(operation2.payment,                    7, 0.0001f);
    ASSERT_NEAR(operation2.commission,                 8, 0.0001f);
    ASSERT_NEAR(operation2.yield,                      9, 0.0001f);
    ASSERT_NEAR(operation2.yieldWithCommission,        10, 0.0001f);
    ASSERT_NEAR(operation2.yieldWithCommissionPercent, 11, 0.0001f);
    ASSERT_EQ(operation2.remainedMoney.units,          12);
    ASSERT_EQ(operation2.remainedMoney.nano,           13);
    ASSERT_EQ(operation2.totalMoney.units,             14);
    ASSERT_EQ(operation2.totalMoney.nano,              15);
    ASSERT_EQ(operation2.pricePrecision,               16);
    ASSERT_EQ(operation2.avgPricePrecision,            17);
    ASSERT_EQ(operation2.paymentPrecision,             18);
    ASSERT_EQ(operation2.commissionPrecision,          19);
    ASSERT_EQ(operation2.yieldPrecision,               20);
    ASSERT_EQ(operation2.yieldWithCommissionPrecision, 21);
    ASSERT_EQ(operation2.remainedMoneyPrecision,       22);
    ASSERT_EQ(operation2.totalMoneyPrecision,          23);
    // clang-format on
}

TEST(Test_Operation, Test_assign)
{
    Operation operation;
    Operation operation2;

    operation.timestamp                    = 1;
    operation.instrumentId                 = "a";
    operation.description                  = "b";
    operation.price                        = 2.0f;
    operation.avgPrice.units               = 3;
    operation.avgPrice.nano                = 4;
    operation.quantity                     = 5;
    operation.remainedQuantity             = 6;
    operation.payment                      = 7.0f;
    operation.commission                   = 8.0f;
    operation.yield                        = 9.0f;
    operation.yieldWithCommission          = 10.0f;
    operation.yieldWithCommissionPercent   = 11.0f;
    operation.remainedMoney.units          = 12;
    operation.remainedMoney.nano           = 13;
    operation.totalMoney.units             = 14;
    operation.totalMoney.nano              = 15;
    operation.pricePrecision               = 16;
    operation.avgPricePrecision            = 17;
    operation.paymentPrecision             = 18;
    operation.commissionPrecision          = 19;
    operation.yieldPrecision               = 20;
    operation.yieldWithCommissionPrecision = 21;
    operation.remainedMoneyPrecision       = 22;
    operation.totalMoneyPrecision          = 23;

    operation2 = operation;

    // clang-format off
    ASSERT_EQ(operation2.timestamp,                    1);
    ASSERT_EQ(operation2.instrumentId,                 "a");
    ASSERT_EQ(operation2.description,                  "b");
    ASSERT_NEAR(operation2.price,                      2, 0.0001f);
    ASSERT_EQ(operation2.avgPrice.units,               3);
    ASSERT_EQ(operation2.avgPrice.nano,                4);
    ASSERT_EQ(operation2.quantity,                     5);
    ASSERT_EQ(operation2.remainedQuantity,             6);
    ASSERT_NEAR(operation2.payment,                    7, 0.0001f);
    ASSERT_NEAR(operation2.commission,                 8, 0.0001f);
    ASSERT_NEAR(operation2.yield,                      9, 0.0001f);
    ASSERT_NEAR(operation2.yieldWithCommission,        10, 0.0001f);
    ASSERT_NEAR(operation2.yieldWithCommissionPercent, 11, 0.0001f);
    ASSERT_EQ(operation2.remainedMoney.units,          12);
    ASSERT_EQ(operation2.remainedMoney.nano,           13);
    ASSERT_EQ(operation2.totalMoney.units,             14);
    ASSERT_EQ(operation2.totalMoney.nano,              15);
    ASSERT_EQ(operation2.pricePrecision,               16);
    ASSERT_EQ(operation2.avgPricePrecision,            17);
    ASSERT_EQ(operation2.paymentPrecision,             18);
    ASSERT_EQ(operation2.commissionPrecision,          19);
    ASSERT_EQ(operation2.yieldPrecision,               20);
    ASSERT_EQ(operation2.yieldWithCommissionPrecision, 21);
    ASSERT_EQ(operation2.remainedMoneyPrecision,       22);
    ASSERT_EQ(operation2.totalMoneyPrecision,          23);
    // clang-format on
}

TEST(Test_Operation, Test_fromJsonObject)
{
    Operation operation;

    // clang-format off
    ASSERT_EQ(operation.timestamp,                    0);
    ASSERT_EQ(operation.instrumentId,                 "");
    ASSERT_EQ(operation.description,                  "");
    ASSERT_NEAR(operation.price,                      0, 0.0001f);
    ASSERT_EQ(operation.avgPrice.units,               0);
    ASSERT_EQ(operation.avgPrice.nano,                0);
    ASSERT_EQ(operation.quantity,                     0);
    ASSERT_EQ(operation.remainedQuantity,             0);
    ASSERT_NEAR(operation.payment,                    0, 0.0001f);
    ASSERT_NEAR(operation.commission,                 0, 0.0001f);
    ASSERT_NEAR(operation.yield,                      0, 0.0001f);
    ASSERT_NEAR(operation.yieldWithCommission,        0, 0.0001f);
    ASSERT_NEAR(operation.yieldWithCommissionPercent, 0, 0.0001f);
    ASSERT_EQ(operation.remainedMoney.units,          0);
    ASSERT_EQ(operation.remainedMoney.nano,           0);
    ASSERT_EQ(operation.totalMoney.units,             0);
    ASSERT_EQ(operation.totalMoney.nano,              0);
    ASSERT_EQ(operation.pricePrecision,               0);
    ASSERT_EQ(operation.avgPricePrecision,            0);
    ASSERT_EQ(operation.paymentPrecision,             0);
    ASSERT_EQ(operation.commissionPrecision,          0);
    ASSERT_EQ(operation.yieldPrecision,               0);
    ASSERT_EQ(operation.yieldWithCommissionPrecision, 0);
    ASSERT_EQ(operation.remainedMoneyPrecision,       0);
    ASSERT_EQ(operation.totalMoneyPrecision,          0);
    // clang-format on

    const QString content =
        R"({"avgPrice":{"nano":4,"units":3},"avgPricePrecision":17,"commission":8,"commissionPrecision":19,"description":"b","instrumentId":"a","payment":7,"paymentPrecision":18,"price":2,"pricePrecision":16,"quantity":5,"remainedMoney":{"nano":13,"units":12},"remainedMoneyPrecision":22,"remainedQuantity":6,"timestamp":1,"totalMoney":{"nano":15,"units":14},"totalMoneyPrecision":23,"yield":9,"yieldPrecision":20,"yieldWithCommission":10,"yieldWithCommissionPercent":11,"yieldWithCommissionPrecision":21})";

    QJsonParseError     parseError;
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(content.toUtf8(), &parseError);

    ASSERT_EQ(parseError.error, QJsonParseError::NoError);
    operation.fromJsonObject(jsonDoc.object());

    // clang-format off
    ASSERT_EQ(operation.timestamp,                    1);
    ASSERT_EQ(operation.instrumentId,                 "a");
    ASSERT_EQ(operation.description,                  "b");
    ASSERT_NEAR(operation.price,                      2, 0.0001f);
    ASSERT_EQ(operation.avgPrice.units,               3);
    ASSERT_EQ(operation.avgPrice.nano,                4);
    ASSERT_EQ(operation.quantity,                     5);
    ASSERT_EQ(operation.remainedQuantity,             6);
    ASSERT_NEAR(operation.payment,                    7, 0.0001f);
    ASSERT_NEAR(operation.commission,                 8, 0.0001f);
    ASSERT_NEAR(operation.yield,                      9, 0.0001f);
    ASSERT_NEAR(operation.yieldWithCommission,        10, 0.0001f);
    ASSERT_NEAR(operation.yieldWithCommissionPercent, 11, 0.0001f);
    ASSERT_EQ(operation.remainedMoney.units,          12);
    ASSERT_EQ(operation.remainedMoney.nano,           13);
    ASSERT_EQ(operation.totalMoney.units,             14);
    ASSERT_EQ(operation.totalMoney.nano,              15);
    ASSERT_EQ(operation.pricePrecision,               16);
    ASSERT_EQ(operation.avgPricePrecision,            17);
    ASSERT_EQ(operation.paymentPrecision,             18);
    ASSERT_EQ(operation.commissionPrecision,          19);
    ASSERT_EQ(operation.yieldPrecision,               20);
    ASSERT_EQ(operation.yieldWithCommissionPrecision, 21);
    ASSERT_EQ(operation.remainedMoneyPrecision,       22);
    ASSERT_EQ(operation.totalMoneyPrecision,          23);
    // clang-format on
}

TEST(Test_Operation, Test_toJsonObject)
{
    Operation operation;

    operation.timestamp                    = 1;
    operation.instrumentId                 = "a";
    operation.description                  = "b";
    operation.price                        = 2.0f;
    operation.avgPrice.units               = 3;
    operation.avgPrice.nano                = 4;
    operation.quantity                     = 5;
    operation.remainedQuantity             = 6;
    operation.payment                      = 7.0f;
    operation.commission                   = 8.0f;
    operation.yield                        = 9.0f;
    operation.yieldWithCommission          = 10.0f;
    operation.yieldWithCommissionPercent   = 11.0f;
    operation.remainedMoney.units          = 12;
    operation.remainedMoney.nano           = 13;
    operation.totalMoney.units             = 14;
    operation.totalMoney.nano              = 15;
    operation.pricePrecision               = 16;
    operation.avgPricePrecision            = 17;
    operation.paymentPrecision             = 18;
    operation.commissionPrecision          = 19;
    operation.yieldPrecision               = 20;
    operation.yieldWithCommissionPrecision = 21;
    operation.remainedMoneyPrecision       = 22;
    operation.totalMoneyPrecision          = 23;

    const QJsonObject   jsonObject = operation.toJsonObject();
    const QJsonDocument jsonDoc(jsonObject);

    const QString content = QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
    const QString expectedContent =
        R"({"avgPrice":{"nano":4,"units":3},"avgPricePrecision":17,"commission":8,"commissionPrecision":19,"description":"b","instrumentId":"a","payment":7,"paymentPrecision":18,"price":2,"pricePrecision":16,"quantity":5,"remainedMoney":{"nano":13,"units":12},"remainedMoneyPrecision":22,"remainedQuantity":6,"timestamp":1,"totalMoney":{"nano":15,"units":14},"totalMoneyPrecision":23,"yield":9,"yieldPrecision":20,"yieldWithCommission":10,"yieldWithCommissionPercent":11,"yieldWithCommissionPrecision":21})";

    ASSERT_EQ(content, expectedContent);
}

TEST(Test_Operation, Test_equals)
{
    Operation operation;
    Operation operation2;

    operation.timestamp                    = 1;
    operation.instrumentId                 = "a";
    operation.description                  = "b";
    operation.price                        = 2.0f;
    operation.avgPrice.units               = 3;
    operation.avgPrice.nano                = 4;
    operation.quantity                     = 5;
    operation.remainedQuantity             = 6;
    operation.payment                      = 7.0f;
    operation.commission                   = 8.0f;
    operation.yield                        = 9.0f;
    operation.yieldWithCommission          = 10.0f;
    operation.yieldWithCommissionPercent   = 11.0f;
    operation.remainedMoney.units          = 12;
    operation.remainedMoney.nano           = 13;
    operation.totalMoney.units             = 14;
    operation.totalMoney.nano              = 15;
    operation.pricePrecision               = 16;
    operation.avgPricePrecision            = 17;
    operation.paymentPrecision             = 18;
    operation.commissionPrecision          = 19;
    operation.yieldPrecision               = 20;
    operation.yieldWithCommissionPrecision = 21;
    operation.remainedMoneyPrecision       = 22;
    operation.totalMoneyPrecision          = 23;

    operation2.timestamp                    = 1;
    operation2.instrumentId                 = "a";
    operation2.description                  = "b";
    operation2.price                        = 2.0f;
    operation2.avgPrice.units               = 3;
    operation2.avgPrice.nano                = 4;
    operation2.quantity                     = 5;
    operation2.remainedQuantity             = 6;
    operation2.payment                      = 7.0f;
    operation2.commission                   = 8.0f;
    operation2.yield                        = 9.0f;
    operation2.yieldWithCommission          = 10.0f;
    operation2.yieldWithCommissionPercent   = 11.0f;
    operation2.remainedMoney.units          = 12;
    operation2.remainedMoney.nano           = 13;
    operation2.totalMoney.units             = 14;
    operation2.totalMoney.nano              = 15;
    operation2.pricePrecision               = 16;
    operation2.avgPricePrecision            = 17;
    operation2.paymentPrecision             = 18;
    operation2.commissionPrecision          = 19;
    operation2.yieldPrecision               = 20;
    operation2.yieldWithCommissionPrecision = 21;
    operation2.remainedMoneyPrecision       = 22;
    operation2.totalMoneyPrecision          = 23;

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

    operation2.avgPrice.units = 3333;
    ASSERT_NE(operation, operation2);
    operation2.avgPrice.units = 3;
    ASSERT_EQ(operation, operation2);

    operation2.avgPrice.nano = 4444;
    ASSERT_NE(operation, operation2);
    operation2.avgPrice.nano = 4;
    ASSERT_EQ(operation, operation2);

    operation2.quantity = 5555;
    ASSERT_NE(operation, operation2);
    operation2.quantity = 5;
    ASSERT_EQ(operation, operation2);

    operation2.remainedQuantity = 6666;
    ASSERT_NE(operation, operation2);
    operation2.remainedQuantity = 6;
    ASSERT_EQ(operation, operation2);

    operation2.payment = 7777.0f;
    ASSERT_NE(operation, operation2);
    operation2.payment = 7.0f;
    ASSERT_EQ(operation, operation2);

    operation2.commission = 8888.0f;
    ASSERT_NE(operation, operation2);
    operation2.commission = 8.0f;
    ASSERT_EQ(operation, operation2);

    operation2.yield = 9999.0f;
    ASSERT_NE(operation, operation2);
    operation2.yield = 9.0f;
    ASSERT_EQ(operation, operation2);

    operation2.yieldWithCommission = 10000.0f;
    ASSERT_NE(operation, operation2);
    operation2.yieldWithCommission = 10.0f;
    ASSERT_EQ(operation, operation2);

    operation2.yieldWithCommissionPercent = 11111.0f;
    ASSERT_NE(operation, operation2);
    operation2.yieldWithCommissionPercent = 11.0f;
    ASSERT_EQ(operation, operation2);

    operation2.remainedMoney.units = 12222;
    ASSERT_NE(operation, operation2);
    operation2.remainedMoney.units = 12;
    ASSERT_EQ(operation, operation2);

    operation2.remainedMoney.nano = 13333;
    ASSERT_NE(operation, operation2);
    operation2.remainedMoney.nano = 13;
    ASSERT_EQ(operation, operation2);

    operation2.totalMoney.units = 14444;
    ASSERT_NE(operation, operation2);
    operation2.totalMoney.units = 14;
    ASSERT_EQ(operation, operation2);

    operation2.totalMoney.nano = 15555;
    ASSERT_NE(operation, operation2);
    operation2.totalMoney.nano = 15;
    ASSERT_EQ(operation, operation2);

    operation2.pricePrecision = 116;
    ASSERT_NE(operation, operation2);
    operation2.pricePrecision = 16;
    ASSERT_EQ(operation, operation2);

    operation2.avgPricePrecision = 117;
    ASSERT_NE(operation, operation2);
    operation2.avgPricePrecision = 17;
    ASSERT_EQ(operation, operation2);

    operation2.paymentPrecision = 118;
    ASSERT_NE(operation, operation2);
    operation2.paymentPrecision = 18;
    ASSERT_EQ(operation, operation2);

    operation2.commissionPrecision = 119;
    ASSERT_NE(operation, operation2);
    operation2.commissionPrecision = 19;
    ASSERT_EQ(operation, operation2);

    operation2.yieldPrecision = 120;
    ASSERT_NE(operation, operation2);
    operation2.yieldPrecision = 20;
    ASSERT_EQ(operation, operation2);

    operation2.yieldWithCommissionPrecision = 121;
    ASSERT_NE(operation, operation2);
    operation2.yieldWithCommissionPrecision = 21;
    ASSERT_EQ(operation, operation2);

    operation2.remainedMoneyPrecision = 122;
    ASSERT_NE(operation, operation2);
    operation2.remainedMoneyPrecision = 22;
    ASSERT_EQ(operation, operation2);

    operation2.totalMoneyPrecision = 123;
    ASSERT_NE(operation, operation2);
    operation2.totalMoneyPrecision = 23;
    ASSERT_EQ(operation, operation2);
}
// NOLINTEND(readability-magic-numbers)
