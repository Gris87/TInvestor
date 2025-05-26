#include "src/domain/operation/operation.h"

#include <QDebug>
#include <QJsonDocument>
#include <gtest/gtest.h>



// NOLINTBEGIN(readability-magic-numbers)
TEST(Test_Operation, Test_constructor_and_destructor)
{
    const Operation operation;

    // clang-format off
    ASSERT_EQ(operation.timestamp,                         0);
    ASSERT_EQ(operation.instrumentId,                      "");
    ASSERT_EQ(operation.description,                       "");
    ASSERT_NEAR(operation.price,                           0, 0.0001f);
    ASSERT_NEAR(operation.avgPrice,                        0, 0.0001f);
    ASSERT_EQ(operation.quantity,                          0);
    ASSERT_EQ(operation.remainedQuantity,                  0);
    ASSERT_NEAR(operation.payment,                         0, 0.0001f);
    ASSERT_NEAR(operation.avgCost,                         0, 0.0001f);
    ASSERT_EQ(operation.cost.units,                        0);
    ASSERT_EQ(operation.cost.nano,                         0);
    ASSERT_NEAR(operation.commission,                      0, 0.0001f);
    ASSERT_NEAR(operation.yield,                           0, 0.0001f);
    ASSERT_NEAR(operation.yieldWithCommission,             0, 0.0001f);
    ASSERT_NEAR(operation.yieldWithCommissionPercent,      0, 0.0001f);
    ASSERT_EQ(operation.inputMoney.units,                  0);
    ASSERT_EQ(operation.inputMoney.nano,                   0);
    ASSERT_EQ(operation.maxInputMoney.units,               0);
    ASSERT_EQ(operation.maxInputMoney.nano,                0);
    ASSERT_EQ(operation.totalYieldWithCommission.units,    0);
    ASSERT_EQ(operation.totalYieldWithCommission.nano,     0);
    ASSERT_NEAR(operation.totalYieldWithCommissionPercent, 0, 0.0001f);
    ASSERT_EQ(operation.remainedMoney.units,               0);
    ASSERT_EQ(operation.remainedMoney.nano,                0);
    ASSERT_EQ(operation.totalMoney.units,                  0);
    ASSERT_EQ(operation.totalMoney.nano,                   0);
    ASSERT_EQ(operation.paymentPrecision,                  0);
    ASSERT_EQ(operation.commissionPrecision,               0);
    // clang-format on
}

TEST(Test_Operation, Test_copy_constructor)
{
    Operation operation;

    operation.timestamp                       = 1;
    operation.instrumentId                    = "a";
    operation.description                     = "b";
    operation.price                           = 2.0f;
    operation.avgPrice                        = 3;
    operation.quantity                        = 4;
    operation.remainedQuantity                = 5;
    operation.payment                         = 6.0f;
    operation.avgCost                         = 7.0f;
    operation.cost.units                      = 8;
    operation.cost.nano                       = 9;
    operation.commission                      = 10.0f;
    operation.yield                           = 11.0f;
    operation.yieldWithCommission             = 12.0f;
    operation.yieldWithCommissionPercent      = 13.0f;
    operation.inputMoney.units                = 14;
    operation.inputMoney.nano                 = 15;
    operation.maxInputMoney.units             = 16;
    operation.maxInputMoney.nano              = 17;
    operation.totalYieldWithCommission.units  = 18;
    operation.totalYieldWithCommission.nano   = 19;
    operation.totalYieldWithCommissionPercent = 20;
    operation.remainedMoney.units             = 21;
    operation.remainedMoney.nano              = 22;
    operation.totalMoney.units                = 23;
    operation.totalMoney.nano                 = 24;
    operation.paymentPrecision                = 25;
    operation.commissionPrecision             = 26;

    const Operation operation2(operation);

    // clang-format off
    ASSERT_EQ(operation2.timestamp,                         1);
    ASSERT_EQ(operation2.instrumentId,                      "a");
    ASSERT_EQ(operation2.description,                       "b");
    ASSERT_NEAR(operation2.price,                           2, 0.0001f);
    ASSERT_NEAR(operation2.avgPrice,                        3, 0.0001f);
    ASSERT_EQ(operation2.quantity,                          4);
    ASSERT_EQ(operation2.remainedQuantity,                  5);
    ASSERT_NEAR(operation2.payment,                         6, 0.0001f);
    ASSERT_NEAR(operation2.avgCost,                         7, 0.0001f);
    ASSERT_EQ(operation2.cost.units,                        8);
    ASSERT_EQ(operation2.cost.nano,                         9);
    ASSERT_NEAR(operation2.commission,                      10, 0.0001f);
    ASSERT_NEAR(operation2.yield,                           11, 0.0001f);
    ASSERT_NEAR(operation2.yieldWithCommission,             12, 0.0001f);
    ASSERT_NEAR(operation2.yieldWithCommissionPercent,      13, 0.0001f);
    ASSERT_EQ(operation2.inputMoney.units,                  14);
    ASSERT_EQ(operation2.inputMoney.nano,                   15);
    ASSERT_EQ(operation2.maxInputMoney.units,               16);
    ASSERT_EQ(operation2.maxInputMoney.nano,                17);
    ASSERT_EQ(operation2.totalYieldWithCommission.units,    18);
    ASSERT_EQ(operation2.totalYieldWithCommission.nano,     19);
    ASSERT_NEAR(operation2.totalYieldWithCommissionPercent, 20, 0.0001f);
    ASSERT_EQ(operation2.remainedMoney.units,               21);
    ASSERT_EQ(operation2.remainedMoney.nano,                22);
    ASSERT_EQ(operation2.totalMoney.units,                  23);
    ASSERT_EQ(operation2.totalMoney.nano,                   24);
    ASSERT_EQ(operation2.paymentPrecision,                  25);
    ASSERT_EQ(operation2.commissionPrecision,               26);
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
    operation.avgPrice                        = 3;
    operation.quantity                        = 4;
    operation.remainedQuantity                = 5;
    operation.payment                         = 6.0f;
    operation.avgCost                         = 7.0f;
    operation.cost.units                      = 8;
    operation.cost.nano                       = 9;
    operation.commission                      = 10.0f;
    operation.yield                           = 11.0f;
    operation.yieldWithCommission             = 12.0f;
    operation.yieldWithCommissionPercent      = 13.0f;
    operation.inputMoney.units                = 14;
    operation.inputMoney.nano                 = 15;
    operation.maxInputMoney.units             = 16;
    operation.maxInputMoney.nano              = 17;
    operation.totalYieldWithCommission.units  = 18;
    operation.totalYieldWithCommission.nano   = 19;
    operation.totalYieldWithCommissionPercent = 20;
    operation.remainedMoney.units             = 21;
    operation.remainedMoney.nano              = 22;
    operation.totalMoney.units                = 23;
    operation.totalMoney.nano                 = 24;
    operation.paymentPrecision                = 25;
    operation.commissionPrecision             = 26;

    operation2 = operation;

    // clang-format off
    ASSERT_EQ(operation2.timestamp,                         1);
    ASSERT_EQ(operation2.instrumentId,                      "a");
    ASSERT_EQ(operation2.description,                       "b");
    ASSERT_NEAR(operation2.price,                           2, 0.0001f);
    ASSERT_NEAR(operation2.avgPrice,                        3, 0.0001f);
    ASSERT_EQ(operation2.quantity,                          4);
    ASSERT_EQ(operation2.remainedQuantity,                  5);
    ASSERT_NEAR(operation2.payment,                         6, 0.0001f);
    ASSERT_NEAR(operation2.avgCost,                         7, 0.0001f);
    ASSERT_EQ(operation2.cost.units,                        8);
    ASSERT_EQ(operation2.cost.nano,                         9);
    ASSERT_NEAR(operation2.commission,                      10, 0.0001f);
    ASSERT_NEAR(operation2.yield,                           11, 0.0001f);
    ASSERT_NEAR(operation2.yieldWithCommission,             12, 0.0001f);
    ASSERT_NEAR(operation2.yieldWithCommissionPercent,      13, 0.0001f);
    ASSERT_EQ(operation2.inputMoney.units,                  14);
    ASSERT_EQ(operation2.inputMoney.nano,                   15);
    ASSERT_EQ(operation2.maxInputMoney.units,               16);
    ASSERT_EQ(operation2.maxInputMoney.nano,                17);
    ASSERT_EQ(operation2.totalYieldWithCommission.units,    18);
    ASSERT_EQ(operation2.totalYieldWithCommission.nano,     19);
    ASSERT_NEAR(operation2.totalYieldWithCommissionPercent, 20, 0.0001f);
    ASSERT_EQ(operation2.remainedMoney.units,               21);
    ASSERT_EQ(operation2.remainedMoney.nano,                22);
    ASSERT_EQ(operation2.totalMoney.units,                  23);
    ASSERT_EQ(operation2.totalMoney.nano,                   24);
    ASSERT_EQ(operation2.paymentPrecision,                  25);
    ASSERT_EQ(operation2.commissionPrecision,               26);
    // clang-format on
}

TEST(Test_Operation, Test_fromJsonObject)
{
    Operation operation;

    // clang-format off
    ASSERT_EQ(operation.timestamp,                         0);
    ASSERT_EQ(operation.instrumentId,                      "");
    ASSERT_EQ(operation.description,                       "");
    ASSERT_NEAR(operation.price,                           0, 0.0001f);
    ASSERT_NEAR(operation.avgPrice,                        0, 0.0001f);
    ASSERT_EQ(operation.quantity,                          0);
    ASSERT_EQ(operation.remainedQuantity,                  0);
    ASSERT_NEAR(operation.payment,                         0, 0.0001f);
    ASSERT_NEAR(operation.avgCost,                         0, 0.0001f);
    ASSERT_EQ(operation.cost.units,                        0);
    ASSERT_EQ(operation.cost.nano,                         0);
    ASSERT_NEAR(operation.commission,                      0, 0.0001f);
    ASSERT_NEAR(operation.yield,                           0, 0.0001f);
    ASSERT_NEAR(operation.yieldWithCommission,             0, 0.0001f);
    ASSERT_NEAR(operation.yieldWithCommissionPercent,      0, 0.0001f);
    ASSERT_EQ(operation.inputMoney.units,                  0);
    ASSERT_EQ(operation.inputMoney.nano,                   0);
    ASSERT_EQ(operation.maxInputMoney.units,               0);
    ASSERT_EQ(operation.maxInputMoney.nano,                0);
    ASSERT_EQ(operation.totalYieldWithCommission.units,    0);
    ASSERT_EQ(operation.totalYieldWithCommission.nano,     0);
    ASSERT_NEAR(operation.totalYieldWithCommissionPercent, 0, 0.0001f);
    ASSERT_EQ(operation.remainedMoney.units,               0);
    ASSERT_EQ(operation.remainedMoney.nano,                0);
    ASSERT_EQ(operation.totalMoney.units,                  0);
    ASSERT_EQ(operation.totalMoney.nano,                   0);
    ASSERT_EQ(operation.paymentPrecision,                  0);
    ASSERT_EQ(operation.commissionPrecision,               0);
    // clang-format on

    const QString content =
        R"({"avgCost":7,"avgPrice":3,"commission":10,"commissionPrecision":26,"cost":{"nano":9,"units":8},"description":"b","inputMoney":{"nano":15,"units":14},"instrumentId":"a","maxInputMoney":{"nano":17,"units":16},"payment":6,"paymentPrecision":25,"price":2,"quantity":4,"remainedMoney":{"nano":22,"units":21},"remainedQuantity":5,"timestamp":1,"totalMoney":{"nano":24,"units":23},"totalYieldWithCommission":{"nano":19,"units":18},"totalYieldWithCommissionPercent":20,"yield":11,"yieldWithCommission":12,"yieldWithCommissionPercent":13})";

    QJsonParseError     parseError;
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(content.toUtf8(), &parseError);

    ASSERT_EQ(parseError.error, QJsonParseError::NoError);
    operation.fromJsonObject(jsonDoc.object());

    // clang-format off
    ASSERT_EQ(operation.timestamp,                         1);
    ASSERT_EQ(operation.instrumentId,                      "a");
    ASSERT_EQ(operation.description,                       "b");
    ASSERT_NEAR(operation.price,                           2, 0.0001f);
    ASSERT_NEAR(operation.avgPrice,                        3, 0.0001f);
    ASSERT_EQ(operation.quantity,                          4);
    ASSERT_EQ(operation.remainedQuantity,                  5);
    ASSERT_NEAR(operation.payment,                         6, 0.0001f);
    ASSERT_NEAR(operation.avgCost,                         7, 0.0001f);
    ASSERT_EQ(operation.cost.units,                        8);
    ASSERT_EQ(operation.cost.nano,                         9);
    ASSERT_NEAR(operation.commission,                      10, 0.0001f);
    ASSERT_NEAR(operation.yield,                           11, 0.0001f);
    ASSERT_NEAR(operation.yieldWithCommission,             12, 0.0001f);
    ASSERT_NEAR(operation.yieldWithCommissionPercent,      13, 0.0001f);
    ASSERT_EQ(operation.inputMoney.units,                  14);
    ASSERT_EQ(operation.inputMoney.nano,                   15);
    ASSERT_EQ(operation.maxInputMoney.units,               16);
    ASSERT_EQ(operation.maxInputMoney.nano,                17);
    ASSERT_EQ(operation.totalYieldWithCommission.units,    18);
    ASSERT_EQ(operation.totalYieldWithCommission.nano,     19);
    ASSERT_NEAR(operation.totalYieldWithCommissionPercent, 20, 0.0001f);
    ASSERT_EQ(operation.remainedMoney.units,               21);
    ASSERT_EQ(operation.remainedMoney.nano,                22);
    ASSERT_EQ(operation.totalMoney.units,                  23);
    ASSERT_EQ(operation.totalMoney.nano,                   24);
    ASSERT_EQ(operation.paymentPrecision,                  25);
    ASSERT_EQ(operation.commissionPrecision,               26);
    // clang-format on
}

TEST(Test_Operation, Test_toJsonObject)
{
    Operation operation;

    operation.timestamp                       = 1;
    operation.instrumentId                    = "a";
    operation.description                     = "b";
    operation.price                           = 2.0f;
    operation.avgPrice                        = 3;
    operation.quantity                        = 4;
    operation.remainedQuantity                = 5;
    operation.payment                         = 6.0f;
    operation.avgCost                         = 7.0f;
    operation.cost.units                      = 8;
    operation.cost.nano                       = 9;
    operation.commission                      = 10.0f;
    operation.yield                           = 11.0f;
    operation.yieldWithCommission             = 12.0f;
    operation.yieldWithCommissionPercent      = 13.0f;
    operation.inputMoney.units                = 14;
    operation.inputMoney.nano                 = 15;
    operation.maxInputMoney.units             = 16;
    operation.maxInputMoney.nano              = 17;
    operation.totalYieldWithCommission.units  = 18;
    operation.totalYieldWithCommission.nano   = 19;
    operation.totalYieldWithCommissionPercent = 20;
    operation.remainedMoney.units             = 21;
    operation.remainedMoney.nano              = 22;
    operation.totalMoney.units                = 23;
    operation.totalMoney.nano                 = 24;
    operation.paymentPrecision                = 25;
    operation.commissionPrecision             = 26;

    const QJsonObject   jsonObject = operation.toJsonObject();
    const QJsonDocument jsonDoc(jsonObject);

    const QString content = QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
    const QString expectedContent =
        R"({"avgCost":7,"avgPrice":3,"commission":10,"commissionPrecision":26,"cost":{"nano":9,"units":8},"description":"b","inputMoney":{"nano":15,"units":14},"instrumentId":"a","maxInputMoney":{"nano":17,"units":16},"payment":6,"paymentPrecision":25,"price":2,"quantity":4,"remainedMoney":{"nano":22,"units":21},"remainedQuantity":5,"timestamp":1,"totalMoney":{"nano":24,"units":23},"totalYieldWithCommission":{"nano":19,"units":18},"totalYieldWithCommissionPercent":20,"yield":11,"yieldWithCommission":12,"yieldWithCommissionPercent":13})";

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
    operation.avgPrice                        = 3;
    operation.quantity                        = 4;
    operation.remainedQuantity                = 5;
    operation.payment                         = 6.0f;
    operation.avgCost                         = 7.0f;
    operation.cost.units                      = 8;
    operation.cost.nano                       = 9;
    operation.commission                      = 10.0f;
    operation.yield                           = 11.0f;
    operation.yieldWithCommission             = 12.0f;
    operation.yieldWithCommissionPercent      = 13.0f;
    operation.inputMoney.units                = 14;
    operation.inputMoney.nano                 = 15;
    operation.maxInputMoney.units             = 16;
    operation.maxInputMoney.nano              = 17;
    operation.totalYieldWithCommission.units  = 18;
    operation.totalYieldWithCommission.nano   = 19;
    operation.totalYieldWithCommissionPercent = 20;
    operation.remainedMoney.units             = 21;
    operation.remainedMoney.nano              = 22;
    operation.totalMoney.units                = 23;
    operation.totalMoney.nano                 = 24;
    operation.paymentPrecision                = 25;
    operation.commissionPrecision             = 26;

    operation2.timestamp                       = 1;
    operation2.instrumentId                    = "a";
    operation2.description                     = "b";
    operation2.price                           = 2.0f;
    operation2.avgPrice                        = 3;
    operation2.quantity                        = 4;
    operation2.remainedQuantity                = 5;
    operation2.payment                         = 6.0f;
    operation2.avgCost                         = 7.0f;
    operation2.cost.units                      = 8;
    operation2.cost.nano                       = 9;
    operation2.commission                      = 10.0f;
    operation2.yield                           = 11.0f;
    operation2.yieldWithCommission             = 12.0f;
    operation2.yieldWithCommissionPercent      = 13.0f;
    operation2.inputMoney.units                = 14;
    operation2.inputMoney.nano                 = 15;
    operation2.maxInputMoney.units             = 16;
    operation2.maxInputMoney.nano              = 17;
    operation2.totalYieldWithCommission.units  = 18;
    operation2.totalYieldWithCommission.nano   = 19;
    operation2.totalYieldWithCommissionPercent = 20;
    operation2.remainedMoney.units             = 21;
    operation2.remainedMoney.nano              = 22;
    operation2.totalMoney.units                = 23;
    operation2.totalMoney.nano                 = 24;
    operation2.paymentPrecision                = 25;
    operation2.commissionPrecision             = 26;

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

    operation2.avgPrice = 3333.0f;
    ASSERT_NE(operation, operation2);
    operation2.avgPrice = 3.0f;
    ASSERT_EQ(operation, operation2);

    operation2.quantity = 4444;
    ASSERT_NE(operation, operation2);
    operation2.quantity = 4;
    ASSERT_EQ(operation, operation2);

    operation2.remainedQuantity = 5555;
    ASSERT_NE(operation, operation2);
    operation2.remainedQuantity = 5;
    ASSERT_EQ(operation, operation2);

    operation2.payment = 6666.0f;
    ASSERT_NE(operation, operation2);
    operation2.payment = 6.0f;
    ASSERT_EQ(operation, operation2);

    operation2.avgCost = 7777.0f;
    ASSERT_NE(operation, operation2);
    operation2.avgCost = 7.0f;
    ASSERT_EQ(operation, operation2);

    operation2.cost.units = 8888;
    ASSERT_NE(operation, operation2);
    operation2.cost.units = 8;
    ASSERT_EQ(operation, operation2);

    operation2.cost.nano = 9999;
    ASSERT_NE(operation, operation2);
    operation2.cost.nano = 9;
    ASSERT_EQ(operation, operation2);

    operation2.commission = 10000.0f;
    ASSERT_NE(operation, operation2);
    operation2.commission = 10.0f;
    ASSERT_EQ(operation, operation2);

    operation2.yield = 11111.0f;
    ASSERT_NE(operation, operation2);
    operation2.yield = 11.0f;
    ASSERT_EQ(operation, operation2);

    operation2.yieldWithCommission = 12222.0f;
    ASSERT_NE(operation, operation2);
    operation2.yieldWithCommission = 12.0f;
    ASSERT_EQ(operation, operation2);

    operation2.yieldWithCommissionPercent = 13333.0f;
    ASSERT_NE(operation, operation2);
    operation2.yieldWithCommissionPercent = 13.0f;
    ASSERT_EQ(operation, operation2);

    operation2.inputMoney.units = 14444;
    ASSERT_NE(operation, operation2);
    operation2.inputMoney.units = 14;
    ASSERT_EQ(operation, operation2);

    operation2.inputMoney.nano = 15555;
    ASSERT_NE(operation, operation2);
    operation2.inputMoney.nano = 15;
    ASSERT_EQ(operation, operation2);

    operation2.maxInputMoney.units = 16666;
    ASSERT_NE(operation, operation2);
    operation2.maxInputMoney.units = 16;
    ASSERT_EQ(operation, operation2);

    operation2.maxInputMoney.nano = 17777;
    ASSERT_NE(operation, operation2);
    operation2.maxInputMoney.nano = 17;
    ASSERT_EQ(operation, operation2);

    operation2.totalYieldWithCommission.units = 18888;
    ASSERT_NE(operation, operation2);
    operation2.totalYieldWithCommission.units = 18;
    ASSERT_EQ(operation, operation2);

    operation2.totalYieldWithCommission.nano = 19999;
    ASSERT_NE(operation, operation2);
    operation2.totalYieldWithCommission.nano = 19;
    ASSERT_EQ(operation, operation2);

    operation2.totalYieldWithCommissionPercent = 20000.0f;
    ASSERT_NE(operation, operation2);
    operation2.totalYieldWithCommissionPercent = 20.0f;
    ASSERT_EQ(operation, operation2);

    operation2.remainedMoney.units = 21111;
    ASSERT_NE(operation, operation2);
    operation2.remainedMoney.units = 21;
    ASSERT_EQ(operation, operation2);

    operation2.remainedMoney.nano = 22222;
    ASSERT_NE(operation, operation2);
    operation2.remainedMoney.nano = 22;
    ASSERT_EQ(operation, operation2);

    operation2.totalMoney.units = 23333;
    ASSERT_NE(operation, operation2);
    operation2.totalMoney.units = 23;
    ASSERT_EQ(operation, operation2);

    operation2.totalMoney.nano = 24444;
    ASSERT_NE(operation, operation2);
    operation2.totalMoney.nano = 24;
    ASSERT_EQ(operation, operation2);

    operation2.paymentPrecision = -25;
    ASSERT_NE(operation, operation2);
    operation2.paymentPrecision = 25;
    ASSERT_EQ(operation, operation2);

    operation2.commissionPrecision = -26;
    ASSERT_NE(operation, operation2);
    operation2.commissionPrecision = 26;
    ASSERT_EQ(operation, operation2);
}
// NOLINTEND(readability-magic-numbers)
