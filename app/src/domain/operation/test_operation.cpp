#include "src/domain/operation/operation.h"

#include <QDebug>
#include <QJsonDocument>
#include <gtest/gtest.h>

#include "src/utils/exception/exception.h"



// NOLINTBEGIN(readability-magic-numbers)
class Test_Operation : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};



TEST_F(Test_Operation, Test_constructor_and_destructor)
{
    const Operation operation;

    // clang-format off
    ASSERT_EQ(operation.timestamp,                         0);
    ASSERT_EQ(operation.originalTimestamp,                 0);
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
    ASSERT_EQ(operation.costFifo,                          Quotation(0, 0));
    ASSERT_EQ(operation.costWavg,                          Quotation(0, 0));
    ASSERT_NEAR(operation.commission,                      0, 0.0001f);
    ASSERT_NEAR(operation.yield,                           0, 0.0001f);
    ASSERT_NEAR(operation.yieldWithCommission,             0, 0.0001f);
    ASSERT_NEAR(operation.yieldWithCommissionPercent,      0, 0.0001f);
    ASSERT_EQ(operation.inputMoney,                        Quotation(0, 0));
    ASSERT_EQ(operation.maxInputMoney,                     Quotation(0, 0));
    ASSERT_EQ(operation.totalYieldWithCommission,          Quotation(0, 0));
    ASSERT_NEAR(operation.totalYieldWithCommissionPercent, 0, 0.0001f);
    ASSERT_EQ(operation.remainedMoney,                     Quotation(0, 0));
    ASSERT_EQ(operation.totalMoney,                        Quotation(0, 0));
    ASSERT_EQ(operation.pricePrecision,                    0);
    ASSERT_EQ(operation.paymentPrecision,                  0);
    ASSERT_EQ(operation.commissionPrecision,               0);
    // clang-format on
}

TEST_F(Test_Operation, Test_copy_constructor)
{
    Operation operation;

    operation.timestamp                       = 1;
    operation.originalTimestamp               = 2;
    operation.instrumentId                    = "a";
    operation.instrumentTicker                = "b";
    operation.instrumentName                  = "c";
    operation.description                     = "d";
    operation.price                           = 3.0f;
    operation.avgPriceFifo                    = 4.0f;
    operation.avgPriceWavg                    = 5.0f;
    operation.quantity                        = 6;
    operation.remainedQuantity                = 7;
    operation.payment                         = 8.0f;
    operation.avgCostFifo                     = 9.0f;
    operation.costFifo                        = Quotation(10, 11);
    operation.costWavg                        = Quotation(12, 13);
    operation.commission                      = 14.0f;
    operation.yield                           = 15.0f;
    operation.yieldWithCommission             = 16.0f;
    operation.yieldWithCommissionPercent      = 17.0f;
    operation.inputMoney                      = Quotation(18, 19);
    operation.maxInputMoney                   = Quotation(20, 21);
    operation.totalYieldWithCommission        = Quotation(22, 23);
    operation.totalYieldWithCommissionPercent = 24.0f;
    operation.remainedMoney                   = Quotation(25, 26);
    operation.totalMoney                      = Quotation(27, 28);
    operation.pricePrecision                  = 29;
    operation.paymentPrecision                = 30;
    operation.commissionPrecision             = 31;

    OperationFifoItem item;

    item.quantity = 32;
    item.cost     = Quotation(33, 34);

    operation.fifoItems << item;

    const Operation operation2(operation);

    // clang-format off
    ASSERT_EQ(operation2.timestamp,                         1);
    ASSERT_EQ(operation2.originalTimestamp,                 2);
    ASSERT_EQ(operation2.instrumentId,                      "a");
    ASSERT_EQ(operation2.instrumentLogo,                    nullptr);
    ASSERT_EQ(operation2.instrumentTicker,                  "b");
    ASSERT_EQ(operation2.instrumentName,                    "c");
    ASSERT_EQ(operation2.description,                       "d");
    ASSERT_NEAR(operation2.price,                           3, 0.0001f);
    ASSERT_EQ(operation2.fifoItems.size(),                  1);
    ASSERT_EQ(operation2.fifoItems.at(0).quantity,          32);
    ASSERT_EQ(operation2.fifoItems.at(0).cost,              Quotation(33, 34));
    ASSERT_NEAR(operation2.avgPriceFifo,                    4, 0.0001f);
    ASSERT_NEAR(operation2.avgPriceWavg,                    5, 0.0001f);
    ASSERT_EQ(operation2.quantity,                          6);
    ASSERT_EQ(operation2.remainedQuantity,                  7);
    ASSERT_NEAR(operation2.payment,                         8, 0.0001f);
    ASSERT_NEAR(operation2.avgCostFifo,                     9, 0.0001f);
    ASSERT_EQ(operation2.costFifo,                          Quotation(10, 11));
    ASSERT_EQ(operation2.costWavg,                          Quotation(12, 13));
    ASSERT_NEAR(operation2.commission,                      14, 0.0001f);
    ASSERT_NEAR(operation2.yield,                           15, 0.0001f);
    ASSERT_NEAR(operation2.yieldWithCommission,             16, 0.0001f);
    ASSERT_NEAR(operation2.yieldWithCommissionPercent,      17, 0.0001f);
    ASSERT_EQ(operation2.inputMoney,                        Quotation(18, 19));
    ASSERT_EQ(operation2.maxInputMoney,                     Quotation(20, 21));
    ASSERT_EQ(operation2.totalYieldWithCommission,          Quotation(22, 23));
    ASSERT_NEAR(operation2.totalYieldWithCommissionPercent, 24, 0.0001f);
    ASSERT_EQ(operation2.remainedMoney,                     Quotation(25, 26));
    ASSERT_EQ(operation2.totalMoney,                        Quotation(27, 28));
    ASSERT_EQ(operation2.pricePrecision,                    29);
    ASSERT_EQ(operation2.paymentPrecision,                  30);
    ASSERT_EQ(operation2.commissionPrecision,               31);
    // clang-format on
}

TEST_F(Test_Operation, Test_assign)
{
    Operation operation;
    Operation operation2;

    operation.timestamp                       = 1;
    operation.originalTimestamp               = 2;
    operation.instrumentId                    = "a";
    operation.instrumentTicker                = "b";
    operation.instrumentName                  = "c";
    operation.description                     = "d";
    operation.price                           = 3.0f;
    operation.avgPriceFifo                    = 4.0f;
    operation.avgPriceWavg                    = 5.0f;
    operation.quantity                        = 6;
    operation.remainedQuantity                = 7;
    operation.payment                         = 8.0f;
    operation.avgCostFifo                     = 9.0f;
    operation.costFifo                        = Quotation(10, 11);
    operation.costWavg                        = Quotation(12, 13);
    operation.commission                      = 14.0f;
    operation.yield                           = 15.0f;
    operation.yieldWithCommission             = 16.0f;
    operation.yieldWithCommissionPercent      = 17.0f;
    operation.inputMoney                      = Quotation(18, 19);
    operation.maxInputMoney                   = Quotation(20, 21);
    operation.totalYieldWithCommission        = Quotation(22, 23);
    operation.totalYieldWithCommissionPercent = 24.0f;
    operation.remainedMoney                   = Quotation(25, 26);
    operation.totalMoney                      = Quotation(27, 28);
    operation.pricePrecision                  = 29;
    operation.paymentPrecision                = 30;
    operation.commissionPrecision             = 31;

    OperationFifoItem item;

    item.quantity = 32;
    item.cost     = Quotation(33, 34);

    operation.fifoItems << item;

    operation2 = operation;

    // clang-format off
    ASSERT_EQ(operation2.timestamp,                         1);
    ASSERT_EQ(operation2.originalTimestamp,                 2);
    ASSERT_EQ(operation2.instrumentId,                      "a");
    ASSERT_EQ(operation2.instrumentLogo,                    nullptr);
    ASSERT_EQ(operation2.instrumentTicker,                  "b");
    ASSERT_EQ(operation2.instrumentName,                    "c");
    ASSERT_EQ(operation2.description,                       "d");
    ASSERT_NEAR(operation2.price,                           3, 0.0001f);
    ASSERT_EQ(operation2.fifoItems.size(),                  1);
    ASSERT_EQ(operation2.fifoItems.at(0).quantity,          32);
    ASSERT_EQ(operation2.fifoItems.at(0).cost,              Quotation(33, 34));
    ASSERT_NEAR(operation2.avgPriceFifo,                    4, 0.0001f);
    ASSERT_NEAR(operation2.avgPriceWavg,                    5, 0.0001f);
    ASSERT_EQ(operation2.quantity,                          6);
    ASSERT_EQ(operation2.remainedQuantity,                  7);
    ASSERT_NEAR(operation2.payment,                         8, 0.0001f);
    ASSERT_NEAR(operation2.avgCostFifo,                     9, 0.0001f);
    ASSERT_EQ(operation2.costFifo,                          Quotation(10, 11));
    ASSERT_EQ(operation2.costWavg,                          Quotation(12, 13));
    ASSERT_NEAR(operation2.commission,                      14, 0.0001f);
    ASSERT_NEAR(operation2.yield,                           15, 0.0001f);
    ASSERT_NEAR(operation2.yieldWithCommission,             16, 0.0001f);
    ASSERT_NEAR(operation2.yieldWithCommissionPercent,      17, 0.0001f);
    ASSERT_EQ(operation2.inputMoney,                        Quotation(18, 19));
    ASSERT_EQ(operation2.maxInputMoney,                     Quotation(20, 21));
    ASSERT_EQ(operation2.totalYieldWithCommission,          Quotation(22, 23));
    ASSERT_NEAR(operation2.totalYieldWithCommissionPercent, 24, 0.0001f);
    ASSERT_EQ(operation2.remainedMoney,                     Quotation(25, 26));
    ASSERT_EQ(operation2.totalMoney,                        Quotation(27, 28));
    ASSERT_EQ(operation2.pricePrecision,                    29);
    ASSERT_EQ(operation2.paymentPrecision,                  30);
    ASSERT_EQ(operation2.commissionPrecision,               31);
    // clang-format on
}

TEST_F(Test_Operation, Test_fromJsonObject)
{
    Operation operation;

    // clang-format off
    ASSERT_EQ(operation.timestamp,                         0);
    ASSERT_EQ(operation.originalTimestamp,                 0);
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
    ASSERT_EQ(operation.costFifo,                          Quotation(0, 0));
    ASSERT_EQ(operation.costWavg,                          Quotation(0, 0));
    ASSERT_NEAR(operation.commission,                      0, 0.0001f);
    ASSERT_NEAR(operation.yield,                           0, 0.0001f);
    ASSERT_NEAR(operation.yieldWithCommission,             0, 0.0001f);
    ASSERT_NEAR(operation.yieldWithCommissionPercent,      0, 0.0001f);
    ASSERT_EQ(operation.inputMoney,                        Quotation(0, 0));
    ASSERT_EQ(operation.maxInputMoney,                     Quotation(0, 0));
    ASSERT_EQ(operation.totalYieldWithCommission,          Quotation(0, 0));
    ASSERT_NEAR(operation.totalYieldWithCommissionPercent, 0, 0.0001f);
    ASSERT_EQ(operation.remainedMoney,                     Quotation(0, 0));
    ASSERT_EQ(operation.totalMoney,                        Quotation(0, 0));
    ASSERT_EQ(operation.pricePrecision,                    0);
    ASSERT_EQ(operation.paymentPrecision,                  0);
    ASSERT_EQ(operation.commissionPrecision,               0);
    // clang-format on

    const QString content =
        R"({"avgCostFifo":9,"avgPriceFifo":4,"avgPriceWavg":5,"commission":14,"commissionPrecision":31,"costFifo":{"nano":11,"units":10},"costWavg":{"nano":13,"units":12},"description":"d","fifoItems":[{"cost":{"nano":34,"units":33},"quantity":32}],"inputMoney":{"nano":19,"units":18},"instrumentId":"a","instrumentName":"c","instrumentTicker":"b","maxInputMoney":{"nano":21,"units":20},"originalTimestamp":2,"payment":8,"paymentPrecision":30,"price":3,"pricePrecision":29,"quantity":6,"remainedMoney":{"nano":26,"units":25},"remainedQuantity":7,"timestamp":1,"totalMoney":{"nano":28,"units":27},"totalYieldWithCommission":{"nano":23,"units":22},"totalYieldWithCommissionPercent":24,"yield":15,"yieldWithCommission":16,"yieldWithCommissionPercent":17})";

    const simdjson::padded_string jsonData(content.toStdString());

    simdjson::ondemand::parser   parser;
    simdjson::ondemand::document doc = parser.iterate(jsonData);

    operation.fromJsonObject(doc.get_object());

    // clang-format off
    ASSERT_EQ(operation.timestamp,                         1);
    ASSERT_EQ(operation.originalTimestamp,                 2);
    ASSERT_EQ(operation.instrumentId,                      "a");
    ASSERT_EQ(operation.instrumentLogo,                    nullptr);
    ASSERT_EQ(operation.instrumentTicker,                  "b");
    ASSERT_EQ(operation.instrumentName,                    "c");
    ASSERT_EQ(operation.description,                       "d");
    ASSERT_NEAR(operation.price,                           3, 0.0001f);
    ASSERT_EQ(operation.fifoItems.size(),                  1);
    ASSERT_EQ(operation.fifoItems.at(0).quantity,          32);
    ASSERT_EQ(operation.fifoItems.at(0).cost,              Quotation(33, 34));
    ASSERT_NEAR(operation.avgPriceFifo,                    4, 0.0001f);
    ASSERT_NEAR(operation.avgPriceWavg,                    5, 0.0001f);
    ASSERT_EQ(operation.quantity,                          6);
    ASSERT_EQ(operation.remainedQuantity,                  7);
    ASSERT_NEAR(operation.payment,                         8, 0.0001f);
    ASSERT_NEAR(operation.avgCostFifo,                     9, 0.0001f);
    ASSERT_EQ(operation.costFifo,                          Quotation(10, 11));
    ASSERT_EQ(operation.costWavg,                          Quotation(12, 13));
    ASSERT_NEAR(operation.commission,                      14, 0.0001f);
    ASSERT_NEAR(operation.yield,                           15, 0.0001f);
    ASSERT_NEAR(operation.yieldWithCommission,             16, 0.0001f);
    ASSERT_NEAR(operation.yieldWithCommissionPercent,      17, 0.0001f);
    ASSERT_EQ(operation.inputMoney,                        Quotation(18, 19));
    ASSERT_EQ(operation.maxInputMoney,                     Quotation(20, 21));
    ASSERT_EQ(operation.totalYieldWithCommission,          Quotation(22, 23));
    ASSERT_NEAR(operation.totalYieldWithCommissionPercent, 24, 0.0001f);
    ASSERT_EQ(operation.remainedMoney,                     Quotation(25, 26));
    ASSERT_EQ(operation.totalMoney,                        Quotation(27, 28));
    ASSERT_EQ(operation.pricePrecision,                    29);
    ASSERT_EQ(operation.paymentPrecision,                  30);
    ASSERT_EQ(operation.commissionPrecision,               31);
    // clang-format on

    const simdjson::padded_string jsonData2 = R"({"bad_key":1})"_padded;
    doc                                     = parser.iterate(jsonData2);

    lastThrownException = "";
    operation.fromJsonObject(doc.get_object());
    ASSERT_EQ(lastThrownException, "Unknown parameter");
}

TEST_F(Test_Operation, Test_toJsonObject)
{
    Operation operation;

    operation.timestamp                       = 1;
    operation.originalTimestamp               = 2;
    operation.instrumentId                    = "a";
    operation.instrumentTicker                = "b";
    operation.instrumentName                  = "c";
    operation.description                     = "d";
    operation.price                           = 3.0f;
    operation.avgPriceFifo                    = 4.0f;
    operation.avgPriceWavg                    = 5.0f;
    operation.quantity                        = 6;
    operation.remainedQuantity                = 7;
    operation.payment                         = 8.0f;
    operation.avgCostFifo                     = 9.0f;
    operation.costFifo                        = Quotation(10, 11);
    operation.costWavg                        = Quotation(12, 13);
    operation.commission                      = 14.0f;
    operation.yield                           = 15.0f;
    operation.yieldWithCommission             = 16.0f;
    operation.yieldWithCommissionPercent      = 17.0f;
    operation.inputMoney                      = Quotation(18, 19);
    operation.maxInputMoney                   = Quotation(20, 21);
    operation.totalYieldWithCommission        = Quotation(22, 23);
    operation.totalYieldWithCommissionPercent = 24.0f;
    operation.remainedMoney                   = Quotation(25, 26);
    operation.totalMoney                      = Quotation(27, 28);
    operation.pricePrecision                  = 29;
    operation.paymentPrecision                = 30;
    operation.commissionPrecision             = 31;

    OperationFifoItem item;

    item.quantity = 32;
    item.cost     = Quotation(33, 34);

    operation.fifoItems << item;

    const QJsonObject   jsonObject = operation.toJsonObject();
    const QJsonDocument jsonDoc(jsonObject);

    const QString content = QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
    const QString expectedContent =
        R"({"avgCostFifo":9,"avgPriceFifo":4,"avgPriceWavg":5,"commission":14,"commissionPrecision":31,"costFifo":{"nano":11,"units":10},"costWavg":{"nano":13,"units":12},"description":"d","fifoItems":[{"cost":{"nano":34,"units":33},"quantity":32}],"inputMoney":{"nano":19,"units":18},"instrumentId":"a","instrumentName":"c","instrumentTicker":"b","maxInputMoney":{"nano":21,"units":20},"originalTimestamp":2,"payment":8,"paymentPrecision":30,"price":3,"pricePrecision":29,"quantity":6,"remainedMoney":{"nano":26,"units":25},"remainedQuantity":7,"timestamp":1,"totalMoney":{"nano":28,"units":27},"totalYieldWithCommission":{"nano":23,"units":22},"totalYieldWithCommissionPercent":24,"yield":15,"yieldWithCommission":16,"yieldWithCommissionPercent":17})";

    ASSERT_EQ(content, expectedContent);
}

TEST_F(Test_Operation, Test_equals)
{
    Operation operation;
    Operation operation2;

    operation.timestamp                       = 1;
    operation.originalTimestamp               = 2;
    operation.instrumentId                    = "a";
    operation.instrumentTicker                = "b";
    operation.instrumentName                  = "c";
    operation.description                     = "d";
    operation.price                           = 3.0f;
    operation.avgPriceFifo                    = 4.0f;
    operation.avgPriceWavg                    = 5.0f;
    operation.quantity                        = 6;
    operation.remainedQuantity                = 7;
    operation.payment                         = 8.0f;
    operation.avgCostFifo                     = 9.0f;
    operation.costFifo                        = Quotation(10, 11);
    operation.costWavg                        = Quotation(12, 13);
    operation.commission                      = 14.0f;
    operation.yield                           = 15.0f;
    operation.yieldWithCommission             = 16.0f;
    operation.yieldWithCommissionPercent      = 17.0f;
    operation.inputMoney                      = Quotation(18, 19);
    operation.maxInputMoney                   = Quotation(20, 21);
    operation.totalYieldWithCommission        = Quotation(22, 23);
    operation.totalYieldWithCommissionPercent = 24.0f;
    operation.remainedMoney                   = Quotation(25, 26);
    operation.totalMoney                      = Quotation(27, 28);
    operation.pricePrecision                  = 29;
    operation.paymentPrecision                = 30;
    operation.commissionPrecision             = 31;

    operation2.timestamp                       = 1;
    operation2.originalTimestamp               = 2;
    operation2.instrumentId                    = "a";
    operation2.instrumentTicker                = "b";
    operation2.instrumentName                  = "c";
    operation2.description                     = "d";
    operation2.price                           = 3.0f;
    operation2.avgPriceFifo                    = 4.0f;
    operation2.avgPriceWavg                    = 5.0f;
    operation2.quantity                        = 6;
    operation2.remainedQuantity                = 7;
    operation2.payment                         = 8.0f;
    operation2.avgCostFifo                     = 9.0f;
    operation2.costFifo                        = Quotation(10, 11);
    operation2.costWavg                        = Quotation(12, 13);
    operation2.commission                      = 14.0f;
    operation2.yield                           = 15.0f;
    operation2.yieldWithCommission             = 16.0f;
    operation2.yieldWithCommissionPercent      = 17.0f;
    operation2.inputMoney                      = Quotation(18, 19);
    operation2.maxInputMoney                   = Quotation(20, 21);
    operation2.totalYieldWithCommission        = Quotation(22, 23);
    operation2.totalYieldWithCommissionPercent = 24.0f;
    operation2.remainedMoney                   = Quotation(25, 26);
    operation2.totalMoney                      = Quotation(27, 28);
    operation2.pricePrecision                  = 29;
    operation2.paymentPrecision                = 30;
    operation2.commissionPrecision             = 31;

    OperationFifoItem item;

    item.quantity = 32;
    item.cost     = Quotation(33, 34);

    operation.fifoItems << item;

    OperationFifoItem item2;

    item2.quantity = 32;
    item2.cost     = Quotation(33, 34);

    operation2.fifoItems << item2;

    ASSERT_EQ(operation, operation2);

    operation2.timestamp = 1000;
    ASSERT_NE(operation, operation2);
    operation2.timestamp = 1;
    ASSERT_EQ(operation, operation2);

    operation2.originalTimestamp = 2000;
    ASSERT_NE(operation, operation2);
    operation2.originalTimestamp = 2;
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

    operation2.price = 3333.0f;
    ASSERT_NE(operation, operation2);
    operation2.price = 3.0f;
    ASSERT_EQ(operation, operation2);

    operation2.fifoItems[0].quantity = 32000;
    ASSERT_NE(operation, operation2);
    operation2.fifoItems[0].quantity = 32;
    ASSERT_EQ(operation, operation2);

    operation2.avgPriceFifo = 4444.0f;
    ASSERT_NE(operation, operation2);
    operation2.avgPriceFifo = 4.0f;
    ASSERT_EQ(operation, operation2);

    operation2.avgPriceWavg = 5555.0f;
    ASSERT_NE(operation, operation2);
    operation2.avgPriceWavg = 5.0f;
    ASSERT_EQ(operation, operation2);

    operation2.quantity = 6666;
    ASSERT_NE(operation, operation2);
    operation2.quantity = 6;
    ASSERT_EQ(operation, operation2);

    operation2.remainedQuantity = 7777;
    ASSERT_NE(operation, operation2);
    operation2.remainedQuantity = 7;
    ASSERT_EQ(operation, operation2);

    operation2.payment = 8888.0f;
    ASSERT_NE(operation, operation2);
    operation2.payment = 8.0f;
    ASSERT_EQ(operation, operation2);

    operation2.avgCostFifo = 9999.0f;
    ASSERT_NE(operation, operation2);
    operation2.avgCostFifo = 9.0f;
    ASSERT_EQ(operation, operation2);

    operation2.costFifo = Quotation(-10, -11);
    ASSERT_NE(operation, operation2);
    operation2.costFifo = Quotation(10, 11);
    ASSERT_EQ(operation, operation2);

    operation2.costWavg = Quotation(-12, -13);
    ASSERT_NE(operation, operation2);
    operation2.costWavg = Quotation(12, 13);
    ASSERT_EQ(operation, operation2);

    operation2.commission = 14444.0f;
    ASSERT_NE(operation, operation2);
    operation2.commission = 14.0f;
    ASSERT_EQ(operation, operation2);

    operation2.yield = 15555.0f;
    ASSERT_NE(operation, operation2);
    operation2.yield = 15.0f;
    ASSERT_EQ(operation, operation2);

    operation2.yieldWithCommission = 16666.0f;
    ASSERT_NE(operation, operation2);
    operation2.yieldWithCommission = 16.0f;
    ASSERT_EQ(operation, operation2);

    operation2.yieldWithCommissionPercent = 17777.0f;
    ASSERT_NE(operation, operation2);
    operation2.yieldWithCommissionPercent = 17.0f;
    ASSERT_EQ(operation, operation2);

    operation2.inputMoney = Quotation(-18, -19);
    ASSERT_NE(operation, operation2);
    operation2.inputMoney = Quotation(18, 19);
    ASSERT_EQ(operation, operation2);

    operation2.maxInputMoney = Quotation(-20, -21);
    ASSERT_NE(operation, operation2);
    operation2.maxInputMoney = Quotation(20, 21);
    ASSERT_EQ(operation, operation2);

    operation2.totalYieldWithCommission = Quotation(-22, -23);
    ASSERT_NE(operation, operation2);
    operation2.totalYieldWithCommission = Quotation(22, 23);
    ASSERT_EQ(operation, operation2);

    operation2.totalYieldWithCommissionPercent = 24444.0f;
    ASSERT_NE(operation, operation2);
    operation2.totalYieldWithCommissionPercent = 24.0f;
    ASSERT_EQ(operation, operation2);

    operation2.remainedMoney = Quotation(-25, -26);
    ASSERT_NE(operation, operation2);
    operation2.remainedMoney = Quotation(25, 26);
    ASSERT_EQ(operation, operation2);

    operation2.totalMoney = Quotation(-27, -28);
    ASSERT_NE(operation, operation2);
    operation2.totalMoney = Quotation(27, 28);
    ASSERT_EQ(operation, operation2);

    operation2.pricePrecision = -29;
    ASSERT_NE(operation, operation2);
    operation2.pricePrecision = 29;
    ASSERT_EQ(operation, operation2);

    operation2.paymentPrecision = -30;
    ASSERT_NE(operation, operation2);
    operation2.paymentPrecision = 30;
    ASSERT_EQ(operation, operation2);

    operation2.commissionPrecision = -31;
    ASSERT_NE(operation, operation2);
    operation2.commissionPrecision = 31;
    ASSERT_EQ(operation, operation2);
}
// NOLINTEND(readability-magic-numbers)
