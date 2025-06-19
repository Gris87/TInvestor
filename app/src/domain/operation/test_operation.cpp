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
    ASSERT_EQ(operation.instrumentLogo,                    nullptr);
    ASSERT_EQ(operation.instrumentTicker,                  "");
    ASSERT_EQ(operation.instrumentName,                    "");
    ASSERT_EQ(operation.description,                       "");
    ASSERT_NEAR(operation.price,                           0, 0.0001f);
    ASSERT_EQ(operation.fifoItems.size(),                  0);
    ASSERT_NEAR(operation.avgPriceFifo,                    0, 0.0001f);
    ASSERT_NEAR(operation.avgPriceWavg,                    0, 0.0001f);
    ASSERT_EQ(operation.quantity,                          0);
    ASSERT_EQ(operation.remainedQuantity,                  0);
    ASSERT_NEAR(operation.payment,                         0, 0.0001f);
    ASSERT_NEAR(operation.avgCostFifo,                     0, 0.0001f);
    ASSERT_EQ(operation.costFifo.units,                    0);
    ASSERT_EQ(operation.costFifo.nano,                     0);
    ASSERT_EQ(operation.costWavg.units,                    0);
    ASSERT_EQ(operation.costWavg.nano,                     0);
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
    ASSERT_EQ(operation.pricePrecision,                    0);
    ASSERT_EQ(operation.paymentPrecision,                  0);
    ASSERT_EQ(operation.commissionPrecision,               0);
    // clang-format on
}

TEST(Test_Operation, Test_copy_constructor)
{
    Operation operation;

    operation.timestamp                       = 1;
    operation.instrumentId                    = "a";
    operation.instrumentTicker                = "b";
    operation.instrumentName                  = "c";
    operation.description                     = "d";
    operation.price                           = 2.0f;
    operation.avgPriceFifo                    = 3.0f;
    operation.avgPriceWavg                    = 4.0f;
    operation.quantity                        = 5;
    operation.remainedQuantity                = 6;
    operation.payment                         = 7.0f;
    operation.avgCostFifo                     = 8.0f;
    operation.costFifo.units                  = 9;
    operation.costFifo.nano                   = 10;
    operation.costWavg.units                  = 11;
    operation.costWavg.nano                   = 12;
    operation.commission                      = 13.0f;
    operation.yield                           = 14.0f;
    operation.yieldWithCommission             = 15.0f;
    operation.yieldWithCommissionPercent      = 16.0f;
    operation.inputMoney.units                = 17;
    operation.inputMoney.nano                 = 18;
    operation.maxInputMoney.units             = 19;
    operation.maxInputMoney.nano              = 20;
    operation.totalYieldWithCommission.units  = 21;
    operation.totalYieldWithCommission.nano   = 22;
    operation.totalYieldWithCommissionPercent = 23;
    operation.remainedMoney.units             = 24;
    operation.remainedMoney.nano              = 25;
    operation.totalMoney.units                = 26;
    operation.totalMoney.nano                 = 27;
    operation.pricePrecision                  = 28;
    operation.paymentPrecision                = 29;
    operation.commissionPrecision             = 30;

    OperationFifoItem item;

    item.quantity   = 31;
    item.cost.units = 32;
    item.cost.nano  = 33;

    operation.fifoItems.append(item);

    const Operation operation2(operation);

    // clang-format off
    ASSERT_EQ(operation2.timestamp,                         1);
    ASSERT_EQ(operation2.instrumentId,                      "a");
    ASSERT_EQ(operation2.instrumentLogo,                    nullptr);
    ASSERT_EQ(operation2.instrumentTicker,                  "b");
    ASSERT_EQ(operation2.instrumentName,                    "c");
    ASSERT_EQ(operation2.description,                       "d");
    ASSERT_NEAR(operation2.price,                           2, 0.0001f);
    ASSERT_EQ(operation2.fifoItems.size(),                  1);
    ASSERT_EQ(operation2.fifoItems.at(0).quantity,          31);
    ASSERT_EQ(operation2.fifoItems.at(0).cost.units,        32);
    ASSERT_EQ(operation2.fifoItems.at(0).cost.nano,         33);
    ASSERT_NEAR(operation2.avgPriceFifo,                    3, 0.0001f);
    ASSERT_NEAR(operation2.avgPriceWavg,                    4, 0.0001f);
    ASSERT_EQ(operation2.quantity,                          5);
    ASSERT_EQ(operation2.remainedQuantity,                  6);
    ASSERT_NEAR(operation2.payment,                         7, 0.0001f);
    ASSERT_NEAR(operation2.avgCostFifo,                     8, 0.0001f);
    ASSERT_EQ(operation2.costFifo.units,                    9);
    ASSERT_EQ(operation2.costFifo.nano,                     10);
    ASSERT_EQ(operation2.costWavg.units,                    11);
    ASSERT_EQ(operation2.costWavg.nano,                     12);
    ASSERT_NEAR(operation2.commission,                      13, 0.0001f);
    ASSERT_NEAR(operation2.yield,                           14, 0.0001f);
    ASSERT_NEAR(operation2.yieldWithCommission,             15, 0.0001f);
    ASSERT_NEAR(operation2.yieldWithCommissionPercent,      16, 0.0001f);
    ASSERT_EQ(operation2.inputMoney.units,                  17);
    ASSERT_EQ(operation2.inputMoney.nano,                   18);
    ASSERT_EQ(operation2.maxInputMoney.units,               19);
    ASSERT_EQ(operation2.maxInputMoney.nano,                20);
    ASSERT_EQ(operation2.totalYieldWithCommission.units,    21);
    ASSERT_EQ(operation2.totalYieldWithCommission.nano,     22);
    ASSERT_NEAR(operation2.totalYieldWithCommissionPercent, 23, 0.0001f);
    ASSERT_EQ(operation2.remainedMoney.units,               24);
    ASSERT_EQ(operation2.remainedMoney.nano,                25);
    ASSERT_EQ(operation2.totalMoney.units,                  26);
    ASSERT_EQ(operation2.totalMoney.nano,                   27);
    ASSERT_EQ(operation2.pricePrecision,                    28);
    ASSERT_EQ(operation2.paymentPrecision,                  29);
    ASSERT_EQ(operation2.commissionPrecision,               30);
    // clang-format on
}

TEST(Test_Operation, Test_assign)
{
    Operation operation;
    Operation operation2;

    operation.timestamp                       = 1;
    operation.instrumentId                    = "a";
    operation.instrumentTicker                = "b";
    operation.instrumentName                  = "c";
    operation.description                     = "d";
    operation.price                           = 2.0f;
    operation.avgPriceFifo                    = 3.0f;
    operation.avgPriceWavg                    = 4.0f;
    operation.quantity                        = 5;
    operation.remainedQuantity                = 6;
    operation.payment                         = 7.0f;
    operation.avgCostFifo                     = 8.0f;
    operation.costFifo.units                  = 9;
    operation.costFifo.nano                   = 10;
    operation.costWavg.units                  = 11;
    operation.costWavg.nano                   = 12;
    operation.commission                      = 13.0f;
    operation.yield                           = 14.0f;
    operation.yieldWithCommission             = 15.0f;
    operation.yieldWithCommissionPercent      = 16.0f;
    operation.inputMoney.units                = 17;
    operation.inputMoney.nano                 = 18;
    operation.maxInputMoney.units             = 19;
    operation.maxInputMoney.nano              = 20;
    operation.totalYieldWithCommission.units  = 21;
    operation.totalYieldWithCommission.nano   = 22;
    operation.totalYieldWithCommissionPercent = 23;
    operation.remainedMoney.units             = 24;
    operation.remainedMoney.nano              = 25;
    operation.totalMoney.units                = 26;
    operation.totalMoney.nano                 = 27;
    operation.pricePrecision                  = 28;
    operation.paymentPrecision                = 29;
    operation.commissionPrecision             = 30;

    OperationFifoItem item;

    item.quantity   = 31;
    item.cost.units = 32;
    item.cost.nano  = 33;

    operation.fifoItems.append(item);

    operation2 = operation;

    // clang-format off
    ASSERT_EQ(operation2.timestamp,                         1);
    ASSERT_EQ(operation2.instrumentId,                      "a");
    ASSERT_EQ(operation2.instrumentLogo,                    nullptr);
    ASSERT_EQ(operation2.instrumentTicker,                  "b");
    ASSERT_EQ(operation2.instrumentName,                    "c");
    ASSERT_EQ(operation2.description,                       "d");
    ASSERT_NEAR(operation2.price,                           2, 0.0001f);
    ASSERT_EQ(operation2.fifoItems.size(),                  1);
    ASSERT_EQ(operation2.fifoItems.at(0).quantity,          31);
    ASSERT_EQ(operation2.fifoItems.at(0).cost.units,        32);
    ASSERT_EQ(operation2.fifoItems.at(0).cost.nano,         33);
    ASSERT_NEAR(operation2.avgPriceFifo,                    3, 0.0001f);
    ASSERT_NEAR(operation2.avgPriceWavg,                    4, 0.0001f);
    ASSERT_EQ(operation2.quantity,                          5);
    ASSERT_EQ(operation2.remainedQuantity,                  6);
    ASSERT_NEAR(operation2.payment,                         7, 0.0001f);
    ASSERT_NEAR(operation2.avgCostFifo,                     8, 0.0001f);
    ASSERT_EQ(operation2.costFifo.units,                    9);
    ASSERT_EQ(operation2.costFifo.nano,                     10);
    ASSERT_EQ(operation2.costWavg.units,                    11);
    ASSERT_EQ(operation2.costWavg.nano,                     12);
    ASSERT_NEAR(operation2.commission,                      13, 0.0001f);
    ASSERT_NEAR(operation2.yield,                           14, 0.0001f);
    ASSERT_NEAR(operation2.yieldWithCommission,             15, 0.0001f);
    ASSERT_NEAR(operation2.yieldWithCommissionPercent,      16, 0.0001f);
    ASSERT_EQ(operation2.inputMoney.units,                  17);
    ASSERT_EQ(operation2.inputMoney.nano,                   18);
    ASSERT_EQ(operation2.maxInputMoney.units,               19);
    ASSERT_EQ(operation2.maxInputMoney.nano,                20);
    ASSERT_EQ(operation2.totalYieldWithCommission.units,    21);
    ASSERT_EQ(operation2.totalYieldWithCommission.nano,     22);
    ASSERT_NEAR(operation2.totalYieldWithCommissionPercent, 23, 0.0001f);
    ASSERT_EQ(operation2.remainedMoney.units,               24);
    ASSERT_EQ(operation2.remainedMoney.nano,                25);
    ASSERT_EQ(operation2.totalMoney.units,                  26);
    ASSERT_EQ(operation2.totalMoney.nano,                   27);
    ASSERT_EQ(operation2.pricePrecision,                    28);
    ASSERT_EQ(operation2.paymentPrecision,                  29);
    ASSERT_EQ(operation2.commissionPrecision,               30);
    // clang-format on
}

TEST(Test_Operation, Test_fromJsonObject)
{
    Operation operation;

    // clang-format off
    ASSERT_EQ(operation.timestamp,                         0);
    ASSERT_EQ(operation.instrumentId,                      "");
    ASSERT_EQ(operation.instrumentLogo,                    nullptr);
    ASSERT_EQ(operation.instrumentTicker,                  "");
    ASSERT_EQ(operation.instrumentName,                    "");
    ASSERT_EQ(operation.description,                       "");
    ASSERT_NEAR(operation.price,                           0, 0.0001f);
    ASSERT_EQ(operation.fifoItems.size(),                  0);
    ASSERT_NEAR(operation.avgPriceFifo,                    0, 0.0001f);
    ASSERT_NEAR(operation.avgPriceWavg,                    0, 0.0001f);
    ASSERT_EQ(operation.quantity,                          0);
    ASSERT_EQ(operation.remainedQuantity,                  0);
    ASSERT_NEAR(operation.payment,                         0, 0.0001f);
    ASSERT_NEAR(operation.avgCostFifo,                     0, 0.0001f);
    ASSERT_EQ(operation.costFifo.units,                    0);
    ASSERT_EQ(operation.costFifo.nano,                     0);
    ASSERT_EQ(operation.costWavg.units,                    0);
    ASSERT_EQ(operation.costWavg.nano,                     0);
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
    ASSERT_EQ(operation.pricePrecision,                    0);
    ASSERT_EQ(operation.paymentPrecision,                  0);
    ASSERT_EQ(operation.commissionPrecision,               0);
    // clang-format on

    const QString content =
        R"({"avgCostFifo":8,"avgPriceFifo":3,"avgPriceWavg":4,"commission":13,"commissionPrecision":30,"costFifo":{"nano":10,"units":9},"costWavg":{"nano":12,"units":11},"description":"d","fifoItems":[{"cost":{"nano":33,"units":32},"quantity":31}],"inputMoney":{"nano":18,"units":17},"instrumentId":"a","instrumentName":"c","instrumentTicker":"b","maxInputMoney":{"nano":20,"units":19},"payment":7,"paymentPrecision":29,"price":2,"pricePrecision":28,"quantity":5,"remainedMoney":{"nano":25,"units":24},"remainedQuantity":6,"timestamp":1,"totalMoney":{"nano":27,"units":26},"totalYieldWithCommission":{"nano":22,"units":21},"totalYieldWithCommissionPercent":23,"yield":14,"yieldWithCommission":15,"yieldWithCommissionPercent":16})";

    simdjson::padded_string jsonData(content.toStdString());

    simdjson::ondemand::parser   parser;
    simdjson::ondemand::document doc = parser.iterate(jsonData);

    operation.fromJsonObject(doc.get_object());

    // clang-format off
    ASSERT_EQ(operation.timestamp,                         1);
    ASSERT_EQ(operation.instrumentId,                      "a");
    ASSERT_EQ(operation.instrumentLogo,                    nullptr);
    ASSERT_EQ(operation.instrumentTicker,                  "b");
    ASSERT_EQ(operation.instrumentName,                    "c");
    ASSERT_EQ(operation.description,                       "d");
    ASSERT_NEAR(operation.price,                           2, 0.0001f);
    ASSERT_EQ(operation.fifoItems.size(),                  1);
    ASSERT_EQ(operation.fifoItems.at(0).quantity,          31);
    ASSERT_EQ(operation.fifoItems.at(0).cost.units,        32);
    ASSERT_EQ(operation.fifoItems.at(0).cost.nano,         33);
    ASSERT_NEAR(operation.avgPriceFifo,                    3, 0.0001f);
    ASSERT_NEAR(operation.avgPriceWavg,                    4, 0.0001f);
    ASSERT_EQ(operation.quantity,                          5);
    ASSERT_EQ(operation.remainedQuantity,                  6);
    ASSERT_NEAR(operation.payment,                         7, 0.0001f);
    ASSERT_NEAR(operation.avgCostFifo,                     8, 0.0001f);
    ASSERT_EQ(operation.costFifo.units,                    9);
    ASSERT_EQ(operation.costFifo.nano,                     10);
    ASSERT_EQ(operation.costWavg.units,                    11);
    ASSERT_EQ(operation.costWavg.nano,                     12);
    ASSERT_NEAR(operation.commission,                      13, 0.0001f);
    ASSERT_NEAR(operation.yield,                           14, 0.0001f);
    ASSERT_NEAR(operation.yieldWithCommission,             15, 0.0001f);
    ASSERT_NEAR(operation.yieldWithCommissionPercent,      16, 0.0001f);
    ASSERT_EQ(operation.inputMoney.units,                  17);
    ASSERT_EQ(operation.inputMoney.nano,                   18);
    ASSERT_EQ(operation.maxInputMoney.units,               19);
    ASSERT_EQ(operation.maxInputMoney.nano,                20);
    ASSERT_EQ(operation.totalYieldWithCommission.units,    21);
    ASSERT_EQ(operation.totalYieldWithCommission.nano,     22);
    ASSERT_NEAR(operation.totalYieldWithCommissionPercent, 23, 0.0001f);
    ASSERT_EQ(operation.remainedMoney.units,               24);
    ASSERT_EQ(operation.remainedMoney.nano,                25);
    ASSERT_EQ(operation.totalMoney.units,                  26);
    ASSERT_EQ(operation.totalMoney.nano,                   27);
    ASSERT_EQ(operation.pricePrecision,                    28);
    ASSERT_EQ(operation.paymentPrecision,                  29);
    ASSERT_EQ(operation.commissionPrecision,               30);
    // clang-format on
}

TEST(Test_Operation, Test_toJsonObject)
{
    Operation operation;

    operation.timestamp                       = 1;
    operation.instrumentId                    = "a";
    operation.instrumentTicker                = "b";
    operation.instrumentName                  = "c";
    operation.description                     = "d";
    operation.price                           = 2.0f;
    operation.avgPriceFifo                    = 3.0f;
    operation.avgPriceWavg                    = 4.0f;
    operation.quantity                        = 5;
    operation.remainedQuantity                = 6;
    operation.payment                         = 7.0f;
    operation.avgCostFifo                     = 8.0f;
    operation.costFifo.units                  = 9;
    operation.costFifo.nano                   = 10;
    operation.costWavg.units                  = 11;
    operation.costWavg.nano                   = 12;
    operation.commission                      = 13.0f;
    operation.yield                           = 14.0f;
    operation.yieldWithCommission             = 15.0f;
    operation.yieldWithCommissionPercent      = 16.0f;
    operation.inputMoney.units                = 17;
    operation.inputMoney.nano                 = 18;
    operation.maxInputMoney.units             = 19;
    operation.maxInputMoney.nano              = 20;
    operation.totalYieldWithCommission.units  = 21;
    operation.totalYieldWithCommission.nano   = 22;
    operation.totalYieldWithCommissionPercent = 23;
    operation.remainedMoney.units             = 24;
    operation.remainedMoney.nano              = 25;
    operation.totalMoney.units                = 26;
    operation.totalMoney.nano                 = 27;
    operation.pricePrecision                  = 28;
    operation.paymentPrecision                = 29;
    operation.commissionPrecision             = 30;

    OperationFifoItem item;

    item.quantity   = 31;
    item.cost.units = 32;
    item.cost.nano  = 33;

    operation.fifoItems.append(item);

    const QJsonObject   jsonObject = operation.toJsonObject();
    const QJsonDocument jsonDoc(jsonObject);

    const QString content = QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
    const QString expectedContent =
        R"({"avgCostFifo":8,"avgPriceFifo":3,"avgPriceWavg":4,"commission":13,"commissionPrecision":30,"costFifo":{"nano":10,"units":9},"costWavg":{"nano":12,"units":11},"description":"d","fifoItems":[{"cost":{"nano":33,"units":32},"quantity":31}],"inputMoney":{"nano":18,"units":17},"instrumentId":"a","instrumentName":"c","instrumentTicker":"b","maxInputMoney":{"nano":20,"units":19},"payment":7,"paymentPrecision":29,"price":2,"pricePrecision":28,"quantity":5,"remainedMoney":{"nano":25,"units":24},"remainedQuantity":6,"timestamp":1,"totalMoney":{"nano":27,"units":26},"totalYieldWithCommission":{"nano":22,"units":21},"totalYieldWithCommissionPercent":23,"yield":14,"yieldWithCommission":15,"yieldWithCommissionPercent":16})";

    ASSERT_EQ(content, expectedContent);
}

TEST(Test_Operation, Test_equals)
{
    Operation operation;
    Operation operation2;

    operation.timestamp                       = 1;
    operation.instrumentId                    = "a";
    operation.instrumentTicker                = "b";
    operation.instrumentName                  = "c";
    operation.description                     = "d";
    operation.price                           = 2.0f;
    operation.avgPriceFifo                    = 3.0f;
    operation.avgPriceWavg                    = 4.0f;
    operation.quantity                        = 5;
    operation.remainedQuantity                = 6;
    operation.payment                         = 7.0f;
    operation.avgCostFifo                     = 8.0f;
    operation.costFifo.units                  = 9;
    operation.costFifo.nano                   = 10;
    operation.costWavg.units                  = 11;
    operation.costWavg.nano                   = 12;
    operation.commission                      = 13.0f;
    operation.yield                           = 14.0f;
    operation.yieldWithCommission             = 15.0f;
    operation.yieldWithCommissionPercent      = 16.0f;
    operation.inputMoney.units                = 17;
    operation.inputMoney.nano                 = 18;
    operation.maxInputMoney.units             = 19;
    operation.maxInputMoney.nano              = 20;
    operation.totalYieldWithCommission.units  = 21;
    operation.totalYieldWithCommission.nano   = 22;
    operation.totalYieldWithCommissionPercent = 23;
    operation.remainedMoney.units             = 24;
    operation.remainedMoney.nano              = 25;
    operation.totalMoney.units                = 26;
    operation.totalMoney.nano                 = 27;
    operation.pricePrecision                  = 28;
    operation.paymentPrecision                = 29;
    operation.commissionPrecision             = 30;

    operation2.timestamp                       = 1;
    operation2.instrumentId                    = "a";
    operation2.instrumentTicker                = "b";
    operation2.instrumentName                  = "c";
    operation2.description                     = "d";
    operation2.price                           = 2.0f;
    operation2.avgPriceFifo                    = 3.0f;
    operation2.avgPriceWavg                    = 4.0f;
    operation2.quantity                        = 5;
    operation2.remainedQuantity                = 6;
    operation2.payment                         = 7.0f;
    operation2.avgCostFifo                     = 8.0f;
    operation2.costFifo.units                  = 9;
    operation2.costFifo.nano                   = 10;
    operation2.costWavg.units                  = 11;
    operation2.costWavg.nano                   = 12;
    operation2.commission                      = 13.0f;
    operation2.yield                           = 14.0f;
    operation2.yieldWithCommission             = 15.0f;
    operation2.yieldWithCommissionPercent      = 16.0f;
    operation2.inputMoney.units                = 17;
    operation2.inputMoney.nano                 = 18;
    operation2.maxInputMoney.units             = 19;
    operation2.maxInputMoney.nano              = 20;
    operation2.totalYieldWithCommission.units  = 21;
    operation2.totalYieldWithCommission.nano   = 22;
    operation2.totalYieldWithCommissionPercent = 23;
    operation2.remainedMoney.units             = 24;
    operation2.remainedMoney.nano              = 25;
    operation2.totalMoney.units                = 26;
    operation2.totalMoney.nano                 = 27;
    operation2.pricePrecision                  = 28;
    operation2.paymentPrecision                = 29;
    operation2.commissionPrecision             = 30;

    OperationFifoItem item;

    item.quantity   = 31;
    item.cost.units = 32;
    item.cost.nano  = 33;

    operation.fifoItems.append(item);

    OperationFifoItem item2;

    item2.quantity   = 31;
    item2.cost.units = 32;
    item2.cost.nano  = 33;

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

    operation2.instrumentTicker = "bbbb";
    ASSERT_NE(operation, operation2);
    operation2.instrumentTicker = "b";
    ASSERT_EQ(operation, operation2);

    operation2.instrumentName = "cccc";
    ASSERT_NE(operation, operation2);
    operation2.instrumentName = "c";
    ASSERT_EQ(operation, operation2);

    operation2.description = "dddd";
    ASSERT_NE(operation, operation2);
    operation2.description = "d";
    ASSERT_EQ(operation, operation2);

    operation2.price = 2222.0f;
    ASSERT_NE(operation, operation2);
    operation2.price = 2.0f;
    ASSERT_EQ(operation, operation2);

    operation2.fifoItems[0].quantity = 31000;
    ASSERT_NE(operation, operation2);
    operation2.fifoItems[0].quantity = 31;
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

    operation2.avgCostFifo = 8888.0f;
    ASSERT_NE(operation, operation2);
    operation2.avgCostFifo = 8.0f;
    ASSERT_EQ(operation, operation2);

    operation2.costFifo.units = 9999;
    ASSERT_NE(operation, operation2);
    operation2.costFifo.units = 9;
    ASSERT_EQ(operation, operation2);

    operation2.costFifo.nano = 10000;
    ASSERT_NE(operation, operation2);
    operation2.costFifo.nano = 10;
    ASSERT_EQ(operation, operation2);

    operation2.costWavg.units = 11111;
    ASSERT_NE(operation, operation2);
    operation2.costWavg.units = 11;
    ASSERT_EQ(operation, operation2);

    operation2.costWavg.nano = 12222;
    ASSERT_NE(operation, operation2);
    operation2.costWavg.nano = 12;
    ASSERT_EQ(operation, operation2);

    operation2.commission = 13333.0f;
    ASSERT_NE(operation, operation2);
    operation2.commission = 13.0f;
    ASSERT_EQ(operation, operation2);

    operation2.yield = 14444.0f;
    ASSERT_NE(operation, operation2);
    operation2.yield = 14.0f;
    ASSERT_EQ(operation, operation2);

    operation2.yieldWithCommission = 15555.0f;
    ASSERT_NE(operation, operation2);
    operation2.yieldWithCommission = 15.0f;
    ASSERT_EQ(operation, operation2);

    operation2.yieldWithCommissionPercent = 16666.0f;
    ASSERT_NE(operation, operation2);
    operation2.yieldWithCommissionPercent = 16.0f;
    ASSERT_EQ(operation, operation2);

    operation2.inputMoney.units = 17777;
    ASSERT_NE(operation, operation2);
    operation2.inputMoney.units = 17;
    ASSERT_EQ(operation, operation2);

    operation2.inputMoney.nano = 18888;
    ASSERT_NE(operation, operation2);
    operation2.inputMoney.nano = 18;
    ASSERT_EQ(operation, operation2);

    operation2.maxInputMoney.units = 19999;
    ASSERT_NE(operation, operation2);
    operation2.maxInputMoney.units = 19;
    ASSERT_EQ(operation, operation2);

    operation2.maxInputMoney.nano = 20000;
    ASSERT_NE(operation, operation2);
    operation2.maxInputMoney.nano = 20;
    ASSERT_EQ(operation, operation2);

    operation2.totalYieldWithCommission.units = 21111;
    ASSERT_NE(operation, operation2);
    operation2.totalYieldWithCommission.units = 21;
    ASSERT_EQ(operation, operation2);

    operation2.totalYieldWithCommission.nano = 22222;
    ASSERT_NE(operation, operation2);
    operation2.totalYieldWithCommission.nano = 22;
    ASSERT_EQ(operation, operation2);

    operation2.totalYieldWithCommissionPercent = 23333.0f;
    ASSERT_NE(operation, operation2);
    operation2.totalYieldWithCommissionPercent = 23.0f;
    ASSERT_EQ(operation, operation2);

    operation2.remainedMoney.units = 24444;
    ASSERT_NE(operation, operation2);
    operation2.remainedMoney.units = 24;
    ASSERT_EQ(operation, operation2);

    operation2.remainedMoney.nano = 25555;
    ASSERT_NE(operation, operation2);
    operation2.remainedMoney.nano = 25;
    ASSERT_EQ(operation, operation2);

    operation2.totalMoney.units = 26666;
    ASSERT_NE(operation, operation2);
    operation2.totalMoney.units = 26;
    ASSERT_EQ(operation, operation2);

    operation2.totalMoney.nano = 27777;
    ASSERT_NE(operation, operation2);
    operation2.totalMoney.nano = 27;
    ASSERT_EQ(operation, operation2);

    operation2.pricePrecision = -28;
    ASSERT_NE(operation, operation2);
    operation2.pricePrecision = 28;
    ASSERT_EQ(operation, operation2);

    operation2.paymentPrecision = -29;
    ASSERT_NE(operation, operation2);
    operation2.paymentPrecision = 29;
    ASSERT_EQ(operation, operation2);

    operation2.commissionPrecision = -30;
    ASSERT_NE(operation, operation2);
    operation2.commissionPrecision = 30;
    ASSERT_EQ(operation, operation2);
}
// NOLINTEND(readability-magic-numbers)
