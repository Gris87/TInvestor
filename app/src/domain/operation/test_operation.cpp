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
    ASSERT_EQ(operation.fifoItems.size(),                  0);
    ASSERT_NEAR(operation.avgPriceFifo,                    0, 0.0001f);
    ASSERT_NEAR(operation.avgPriceWavg,                    0, 0.0001f);
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
    operation.avgPriceFifo                    = 3.0f;
    operation.avgPriceWavg                    = 4.0f;
    operation.quantity                        = 5;
    operation.remainedQuantity                = 6;
    operation.payment                         = 7.0f;
    operation.avgCost                         = 8.0f;
    operation.cost.units                      = 9;
    operation.cost.nano                       = 10;
    operation.commission                      = 11.0f;
    operation.yield                           = 12.0f;
    operation.yieldWithCommission             = 13.0f;
    operation.yieldWithCommissionPercent      = 14.0f;
    operation.inputMoney.units                = 15;
    operation.inputMoney.nano                 = 16;
    operation.maxInputMoney.units             = 17;
    operation.maxInputMoney.nano              = 18;
    operation.totalYieldWithCommission.units  = 19;
    operation.totalYieldWithCommission.nano   = 20;
    operation.totalYieldWithCommissionPercent = 21;
    operation.remainedMoney.units             = 22;
    operation.remainedMoney.nano              = 23;
    operation.totalMoney.units                = 24;
    operation.totalMoney.nano                 = 25;
    operation.paymentPrecision                = 26;
    operation.commissionPrecision             = 27;

    OperationFifoItem item;

    item.quantity   = 28;
    item.cost.units = 29;
    item.cost.nano  = 30;

    operation.fifoItems.append(item);

    const Operation operation2(operation);

    // clang-format off
    ASSERT_EQ(operation2.timestamp,                         1);
    ASSERT_EQ(operation2.instrumentId,                      "a");
    ASSERT_EQ(operation2.description,                       "b");
    ASSERT_NEAR(operation2.price,                           2, 0.0001f);
    ASSERT_EQ(operation2.fifoItems.size(),                  1);
    ASSERT_EQ(operation2.fifoItems.at(0).quantity,          28);
    ASSERT_EQ(operation2.fifoItems.at(0).cost.units,        29);
    ASSERT_EQ(operation2.fifoItems.at(0).cost.nano,         30);
    ASSERT_NEAR(operation2.avgPriceFifo,                    3, 0.0001f);
    ASSERT_NEAR(operation2.avgPriceWavg,                    4, 0.0001f);
    ASSERT_EQ(operation2.quantity,                          5);
    ASSERT_EQ(operation2.remainedQuantity,                  6);
    ASSERT_NEAR(operation2.payment,                         7, 0.0001f);
    ASSERT_NEAR(operation2.avgCost,                         8, 0.0001f);
    ASSERT_EQ(operation2.cost.units,                        9);
    ASSERT_EQ(operation2.cost.nano,                         10);
    ASSERT_NEAR(operation2.commission,                      11, 0.0001f);
    ASSERT_NEAR(operation2.yield,                           12, 0.0001f);
    ASSERT_NEAR(operation2.yieldWithCommission,             13, 0.0001f);
    ASSERT_NEAR(operation2.yieldWithCommissionPercent,      14, 0.0001f);
    ASSERT_EQ(operation2.inputMoney.units,                  15);
    ASSERT_EQ(operation2.inputMoney.nano,                   16);
    ASSERT_EQ(operation2.maxInputMoney.units,               17);
    ASSERT_EQ(operation2.maxInputMoney.nano,                18);
    ASSERT_EQ(operation2.totalYieldWithCommission.units,    19);
    ASSERT_EQ(operation2.totalYieldWithCommission.nano,     20);
    ASSERT_NEAR(operation2.totalYieldWithCommissionPercent, 21, 0.0001f);
    ASSERT_EQ(operation2.remainedMoney.units,               22);
    ASSERT_EQ(operation2.remainedMoney.nano,                23);
    ASSERT_EQ(operation2.totalMoney.units,                  24);
    ASSERT_EQ(operation2.totalMoney.nano,                   25);
    ASSERT_EQ(operation2.paymentPrecision,                  26);
    ASSERT_EQ(operation2.commissionPrecision,               27);
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
    operation.avgPriceFifo                    = 3.0f;
    operation.avgPriceWavg                    = 4.0f;
    operation.quantity                        = 5;
    operation.remainedQuantity                = 6;
    operation.payment                         = 7.0f;
    operation.avgCost                         = 8.0f;
    operation.cost.units                      = 9;
    operation.cost.nano                       = 10;
    operation.commission                      = 11.0f;
    operation.yield                           = 12.0f;
    operation.yieldWithCommission             = 13.0f;
    operation.yieldWithCommissionPercent      = 14.0f;
    operation.inputMoney.units                = 15;
    operation.inputMoney.nano                 = 16;
    operation.maxInputMoney.units             = 17;
    operation.maxInputMoney.nano              = 18;
    operation.totalYieldWithCommission.units  = 19;
    operation.totalYieldWithCommission.nano   = 20;
    operation.totalYieldWithCommissionPercent = 21;
    operation.remainedMoney.units             = 22;
    operation.remainedMoney.nano              = 23;
    operation.totalMoney.units                = 24;
    operation.totalMoney.nano                 = 25;
    operation.paymentPrecision                = 26;
    operation.commissionPrecision             = 27;

    OperationFifoItem item;

    item.quantity   = 28;
    item.cost.units = 29;
    item.cost.nano  = 30;

    operation.fifoItems.append(item);

    operation2 = operation;

    // clang-format off
    ASSERT_EQ(operation2.timestamp,                         1);
    ASSERT_EQ(operation2.instrumentId,                      "a");
    ASSERT_EQ(operation2.description,                       "b");
    ASSERT_NEAR(operation2.price,                           2, 0.0001f);
    ASSERT_EQ(operation2.fifoItems.size(),                  1);
    ASSERT_EQ(operation2.fifoItems.at(0).quantity,          28);
    ASSERT_EQ(operation2.fifoItems.at(0).cost.units,        29);
    ASSERT_EQ(operation2.fifoItems.at(0).cost.nano,         30);
    ASSERT_NEAR(operation2.avgPriceFifo,                    3, 0.0001f);
    ASSERT_NEAR(operation2.avgPriceWavg,                    4, 0.0001f);
    ASSERT_EQ(operation2.quantity,                          5);
    ASSERT_EQ(operation2.remainedQuantity,                  6);
    ASSERT_NEAR(operation2.payment,                         7, 0.0001f);
    ASSERT_NEAR(operation2.avgCost,                         8, 0.0001f);
    ASSERT_EQ(operation2.cost.units,                        9);
    ASSERT_EQ(operation2.cost.nano,                         10);
    ASSERT_NEAR(operation2.commission,                      11, 0.0001f);
    ASSERT_NEAR(operation2.yield,                           12, 0.0001f);
    ASSERT_NEAR(operation2.yieldWithCommission,             13, 0.0001f);
    ASSERT_NEAR(operation2.yieldWithCommissionPercent,      14, 0.0001f);
    ASSERT_EQ(operation2.inputMoney.units,                  15);
    ASSERT_EQ(operation2.inputMoney.nano,                   16);
    ASSERT_EQ(operation2.maxInputMoney.units,               17);
    ASSERT_EQ(operation2.maxInputMoney.nano,                18);
    ASSERT_EQ(operation2.totalYieldWithCommission.units,    19);
    ASSERT_EQ(operation2.totalYieldWithCommission.nano,     20);
    ASSERT_NEAR(operation2.totalYieldWithCommissionPercent, 21, 0.0001f);
    ASSERT_EQ(operation2.remainedMoney.units,               22);
    ASSERT_EQ(operation2.remainedMoney.nano,                23);
    ASSERT_EQ(operation2.totalMoney.units,                  24);
    ASSERT_EQ(operation2.totalMoney.nano,                   25);
    ASSERT_EQ(operation2.paymentPrecision,                  26);
    ASSERT_EQ(operation2.commissionPrecision,               27);
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
    ASSERT_EQ(operation.fifoItems.size(),                  0);
    ASSERT_NEAR(operation.avgPriceFifo,                    0, 0.0001f);
    ASSERT_NEAR(operation.avgPriceWavg,                    0, 0.0001f);
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
        R"({"avgCost":8,"avgPriceFifo":3,"avgPriceWavg":4,"commission":11,"commissionPrecision":27,"cost":{"nano":10,"units":9},"description":"b","fifoItems":[{"cost":{"nano":30,"units":29},"quantity":28}],"inputMoney":{"nano":16,"units":15},"instrumentId":"a","maxInputMoney":{"nano":18,"units":17},"payment":7,"paymentPrecision":26,"price":2,"quantity":5,"remainedMoney":{"nano":23,"units":22},"remainedQuantity":6,"timestamp":1,"totalMoney":{"nano":25,"units":24},"totalYieldWithCommission":{"nano":20,"units":19},"totalYieldWithCommissionPercent":21,"yield":12,"yieldWithCommission":13,"yieldWithCommissionPercent":14})";

    QJsonParseError     parseError;
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(content.toUtf8(), &parseError);

    ASSERT_EQ(parseError.error, QJsonParseError::NoError);
    operation.fromJsonObject(jsonDoc.object());

    // clang-format off
    ASSERT_EQ(operation.timestamp,                         1);
    ASSERT_EQ(operation.instrumentId,                      "a");
    ASSERT_EQ(operation.description,                       "b");
    ASSERT_NEAR(operation.price,                           2, 0.0001f);
    ASSERT_EQ(operation.fifoItems.size(),                  1);
    ASSERT_EQ(operation.fifoItems.at(0).quantity,          28);
    ASSERT_EQ(operation.fifoItems.at(0).cost.units,        29);
    ASSERT_EQ(operation.fifoItems.at(0).cost.nano,         30);
    ASSERT_NEAR(operation.avgPriceFifo,                    3, 0.0001f);
    ASSERT_NEAR(operation.avgPriceWavg,                    4, 0.0001f);
    ASSERT_EQ(operation.quantity,                          5);
    ASSERT_EQ(operation.remainedQuantity,                  6);
    ASSERT_NEAR(operation.payment,                         7, 0.0001f);
    ASSERT_NEAR(operation.avgCost,                         8, 0.0001f);
    ASSERT_EQ(operation.cost.units,                        9);
    ASSERT_EQ(operation.cost.nano,                         10);
    ASSERT_NEAR(operation.commission,                      11, 0.0001f);
    ASSERT_NEAR(operation.yield,                           12, 0.0001f);
    ASSERT_NEAR(operation.yieldWithCommission,             13, 0.0001f);
    ASSERT_NEAR(operation.yieldWithCommissionPercent,      14, 0.0001f);
    ASSERT_EQ(operation.inputMoney.units,                  15);
    ASSERT_EQ(operation.inputMoney.nano,                   16);
    ASSERT_EQ(operation.maxInputMoney.units,               17);
    ASSERT_EQ(operation.maxInputMoney.nano,                18);
    ASSERT_EQ(operation.totalYieldWithCommission.units,    19);
    ASSERT_EQ(operation.totalYieldWithCommission.nano,     20);
    ASSERT_NEAR(operation.totalYieldWithCommissionPercent, 21, 0.0001f);
    ASSERT_EQ(operation.remainedMoney.units,               22);
    ASSERT_EQ(operation.remainedMoney.nano,                23);
    ASSERT_EQ(operation.totalMoney.units,                  24);
    ASSERT_EQ(operation.totalMoney.nano,                   25);
    ASSERT_EQ(operation.paymentPrecision,                  26);
    ASSERT_EQ(operation.commissionPrecision,               27);
    // clang-format on
}

TEST(Test_Operation, Test_toJsonObject)
{
    Operation operation;

    operation.timestamp                       = 1;
    operation.instrumentId                    = "a";
    operation.description                     = "b";
    operation.price                           = 2.0f;
    operation.avgPriceFifo                    = 3.0f;
    operation.avgPriceWavg                    = 4.0f;
    operation.quantity                        = 5;
    operation.remainedQuantity                = 6;
    operation.payment                         = 7.0f;
    operation.avgCost                         = 8.0f;
    operation.cost.units                      = 9;
    operation.cost.nano                       = 10;
    operation.commission                      = 11.0f;
    operation.yield                           = 12.0f;
    operation.yieldWithCommission             = 13.0f;
    operation.yieldWithCommissionPercent      = 14.0f;
    operation.inputMoney.units                = 15;
    operation.inputMoney.nano                 = 16;
    operation.maxInputMoney.units             = 17;
    operation.maxInputMoney.nano              = 18;
    operation.totalYieldWithCommission.units  = 19;
    operation.totalYieldWithCommission.nano   = 20;
    operation.totalYieldWithCommissionPercent = 21;
    operation.remainedMoney.units             = 22;
    operation.remainedMoney.nano              = 23;
    operation.totalMoney.units                = 24;
    operation.totalMoney.nano                 = 25;
    operation.paymentPrecision                = 26;
    operation.commissionPrecision             = 27;

    OperationFifoItem item;

    item.quantity   = 28;
    item.cost.units = 29;
    item.cost.nano  = 30;

    operation.fifoItems.append(item);

    const QJsonObject   jsonObject = operation.toJsonObject();
    const QJsonDocument jsonDoc(jsonObject);

    const QString content = QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
    const QString expectedContent =
        R"({"avgCost":8,"avgPriceFifo":3,"avgPriceWavg":4,"commission":11,"commissionPrecision":27,"cost":{"nano":10,"units":9},"description":"b","fifoItems":[{"cost":{"nano":30,"units":29},"quantity":28}],"inputMoney":{"nano":16,"units":15},"instrumentId":"a","maxInputMoney":{"nano":18,"units":17},"payment":7,"paymentPrecision":26,"price":2,"quantity":5,"remainedMoney":{"nano":23,"units":22},"remainedQuantity":6,"timestamp":1,"totalMoney":{"nano":25,"units":24},"totalYieldWithCommission":{"nano":20,"units":19},"totalYieldWithCommissionPercent":21,"yield":12,"yieldWithCommission":13,"yieldWithCommissionPercent":14})";

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
    operation.avgPriceFifo                    = 3.0f;
    operation.avgPriceWavg                    = 4.0f;
    operation.quantity                        = 5;
    operation.remainedQuantity                = 6;
    operation.payment                         = 7.0f;
    operation.avgCost                         = 8.0f;
    operation.cost.units                      = 9;
    operation.cost.nano                       = 10;
    operation.commission                      = 11.0f;
    operation.yield                           = 12.0f;
    operation.yieldWithCommission             = 13.0f;
    operation.yieldWithCommissionPercent      = 14.0f;
    operation.inputMoney.units                = 15;
    operation.inputMoney.nano                 = 16;
    operation.maxInputMoney.units             = 17;
    operation.maxInputMoney.nano              = 18;
    operation.totalYieldWithCommission.units  = 19;
    operation.totalYieldWithCommission.nano   = 20;
    operation.totalYieldWithCommissionPercent = 21;
    operation.remainedMoney.units             = 22;
    operation.remainedMoney.nano              = 23;
    operation.totalMoney.units                = 24;
    operation.totalMoney.nano                 = 25;
    operation.paymentPrecision                = 26;
    operation.commissionPrecision             = 27;

    operation2.timestamp                       = 1;
    operation2.instrumentId                    = "a";
    operation2.description                     = "b";
    operation2.price                           = 2.0f;
    operation2.avgPriceFifo                    = 3.0f;
    operation2.avgPriceWavg                    = 4.0f;
    operation2.quantity                        = 5;
    operation2.remainedQuantity                = 6;
    operation2.payment                         = 7.0f;
    operation2.avgCost                         = 8.0f;
    operation2.cost.units                      = 9;
    operation2.cost.nano                       = 10;
    operation2.commission                      = 11.0f;
    operation2.yield                           = 12.0f;
    operation2.yieldWithCommission             = 13.0f;
    operation2.yieldWithCommissionPercent      = 14.0f;
    operation2.inputMoney.units                = 15;
    operation2.inputMoney.nano                 = 16;
    operation2.maxInputMoney.units             = 17;
    operation2.maxInputMoney.nano              = 18;
    operation2.totalYieldWithCommission.units  = 19;
    operation2.totalYieldWithCommission.nano   = 20;
    operation2.totalYieldWithCommissionPercent = 21;
    operation2.remainedMoney.units             = 22;
    operation2.remainedMoney.nano              = 23;
    operation2.totalMoney.units                = 24;
    operation2.totalMoney.nano                 = 25;
    operation2.paymentPrecision                = 26;
    operation2.commissionPrecision             = 27;

    OperationFifoItem item;

    item.quantity   = 28;
    item.cost.units = 29;
    item.cost.nano  = 30;

    operation.fifoItems.append(item);

    OperationFifoItem item2;

    item2.quantity   = 28;
    item2.cost.units = 29;
    item2.cost.nano  = 30;

    operation2.fifoItems.append(item2);

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

    operation2.fifoItems[0].quantity = 28888;
    ASSERT_NE(operation, operation2);
    operation2.fifoItems[0].quantity = 28;
    ASSERT_EQ(operation, operation2);

    operation2.avgPriceFifo = 3333.0f;
    ASSERT_NE(operation, operation2);
    operation2.avgPriceFifo = 3.0f;
    ASSERT_EQ(operation, operation2);

    operation2.avgPriceWavg = 4444.0f;
    ASSERT_NE(operation, operation2);
    operation2.avgPriceWavg = 4.0f;
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

    operation2.avgCost = 8888.0f;
    ASSERT_NE(operation, operation2);
    operation2.avgCost = 8.0f;
    ASSERT_EQ(operation, operation2);

    operation2.cost.units = 9999;
    ASSERT_NE(operation, operation2);
    operation2.cost.units = 9;
    ASSERT_EQ(operation, operation2);

    operation2.cost.nano = 10000;
    ASSERT_NE(operation, operation2);
    operation2.cost.nano = 10;
    ASSERT_EQ(operation, operation2);

    operation2.commission = 11111.0f;
    ASSERT_NE(operation, operation2);
    operation2.commission = 11.0f;
    ASSERT_EQ(operation, operation2);

    operation2.yield = 12222.0f;
    ASSERT_NE(operation, operation2);
    operation2.yield = 12.0f;
    ASSERT_EQ(operation, operation2);

    operation2.yieldWithCommission = 13333.0f;
    ASSERT_NE(operation, operation2);
    operation2.yieldWithCommission = 13.0f;
    ASSERT_EQ(operation, operation2);

    operation2.yieldWithCommissionPercent = 14444.0f;
    ASSERT_NE(operation, operation2);
    operation2.yieldWithCommissionPercent = 14.0f;
    ASSERT_EQ(operation, operation2);

    operation2.inputMoney.units = 15555;
    ASSERT_NE(operation, operation2);
    operation2.inputMoney.units = 15;
    ASSERT_EQ(operation, operation2);

    operation2.inputMoney.nano = 16666;
    ASSERT_NE(operation, operation2);
    operation2.inputMoney.nano = 16;
    ASSERT_EQ(operation, operation2);

    operation2.maxInputMoney.units = 17777;
    ASSERT_NE(operation, operation2);
    operation2.maxInputMoney.units = 17;
    ASSERT_EQ(operation, operation2);

    operation2.maxInputMoney.nano = 18888;
    ASSERT_NE(operation, operation2);
    operation2.maxInputMoney.nano = 18;
    ASSERT_EQ(operation, operation2);

    operation2.totalYieldWithCommission.units = 19999;
    ASSERT_NE(operation, operation2);
    operation2.totalYieldWithCommission.units = 19;
    ASSERT_EQ(operation, operation2);

    operation2.totalYieldWithCommission.nano = 20000;
    ASSERT_NE(operation, operation2);
    operation2.totalYieldWithCommission.nano = 20;
    ASSERT_EQ(operation, operation2);

    operation2.totalYieldWithCommissionPercent = 21111.0f;
    ASSERT_NE(operation, operation2);
    operation2.totalYieldWithCommissionPercent = 21.0f;
    ASSERT_EQ(operation, operation2);

    operation2.remainedMoney.units = 22222;
    ASSERT_NE(operation, operation2);
    operation2.remainedMoney.units = 22;
    ASSERT_EQ(operation, operation2);

    operation2.remainedMoney.nano = 23333;
    ASSERT_NE(operation, operation2);
    operation2.remainedMoney.nano = 23;
    ASSERT_EQ(operation, operation2);

    operation2.totalMoney.units = 24444;
    ASSERT_NE(operation, operation2);
    operation2.totalMoney.units = 24;
    ASSERT_EQ(operation, operation2);

    operation2.totalMoney.nano = 25555;
    ASSERT_NE(operation, operation2);
    operation2.totalMoney.nano = 25;
    ASSERT_EQ(operation, operation2);

    operation2.paymentPrecision = -26;
    ASSERT_NE(operation, operation2);
    operation2.paymentPrecision = 26;
    ASSERT_EQ(operation, operation2);

    operation2.commissionPrecision = -27;
    ASSERT_NE(operation, operation2);
    operation2.commissionPrecision = 27;
    ASSERT_EQ(operation, operation2);
}
// NOLINTEND(readability-magic-numbers)
