#include "src/utils/optimizer/optimizer.h"

#include <gtest/gtest.h>



class Test_Optimizer : public ::testing::Test
{
protected:
    void SetUp() override
    {
        optimizer = new Optimizer();
    }

    void TearDown() override
    {
        delete optimizer;
    }

    Optimizer* optimizer;
};



TEST_F(Test_Optimizer, Test_constructor_and_destructor)
{
}

TEST_F(Test_Optimizer, Test_optimizeOperations)
{
    QList<Operation> operations;
    QList<Operation> optimizedOperations;

    operations.resizeForOverwrite(11);
    optimizedOperations.resizeForOverwrite(5);

    for (int i = 0; i < operations.size() - 1; i += 2)
    {
        Operation& operation1 = operations[i];
        Operation& operation2 = operations[i + 1];

        operation1.timestamp                       = operations.size() - i;
        operation1.originalTimestamp               = operations.size() - i;
        operation1.instrumentId                    = "aaaaa";
        operation1.instrumentTicker                = "aaaaa";
        operation1.instrumentName                  = "?????";
        operation1.description                     = "Sell 10 ivashka durashka shares";
        operation1.price                           = 280.0f;
        operation1.avgPriceFifo                    = 253.3f;
        operation1.avgPriceWavg                    = 253.3f;
        operation1.quantity                        = 10;
        operation1.remainedQuantity                = 0;
        operation1.payment                         = 2800.0f;
        operation1.avgCostFifo                     = 2533.0f;
        operation1.costFifo                        = Quotation(0, 0);
        operation1.costWavg                        = Quotation(0, 0);
        operation1.commission                      = -1.4f;
        operation1.yield                           = 267.0f;
        operation1.yieldWithCommission             = 265.6f;
        operation1.yieldWithCommissionPercent      = 10.4856f;
        operation1.inputMoney                      = Quotation(200000, 0);
        operation1.maxInputMoney                   = Quotation(200000, 0);
        operation1.totalYieldWithCommission        = Quotation(265, -666500000);
        operation1.totalYieldWithCommissionPercent = 0.1321667f;
        operation1.remainedMoney                   = Quotation(200300, 0);
        operation1.totalMoney                      = Quotation(200300, 0);
        operation1.pricePrecision                  = 2;
        operation1.paymentPrecision                = 2;
        operation1.commissionPrecision             = 2;

        operation2.timestamp                       = operations.size() - i - 1;
        operation2.originalTimestamp               = operations.size() - i - 1;
        operation2.instrumentId                    = "aaaaa";
        operation2.instrumentTicker                = "aaaaa";
        operation2.instrumentName                  = "?????";
        operation2.description                     = "Buy 10 ivashka durashka shares";
        operation2.price                           = 253.3f;
        operation2.avgPriceFifo                    = 253.3f;
        operation2.avgPriceWavg                    = 253.3f;
        operation2.quantity                        = 10;
        operation2.remainedQuantity                = 10;
        operation2.payment                         = -2533.0f;
        operation2.avgCostFifo                     = 2533.0f;
        operation2.costFifo                        = Quotation(2533, 0);
        operation2.costWavg                        = Quotation(2533, 0);
        operation2.commission                      = -1.2665f;
        operation2.yield                           = 0.0f;
        operation2.yieldWithCommission             = -1.2665f;
        operation2.yieldWithCommissionPercent      = -0.05f;
        operation2.inputMoney                      = Quotation(200000, 0);
        operation2.maxInputMoney                   = Quotation(200000, 0);
        operation2.totalYieldWithCommission        = Quotation(-1, -266500000);
        operation2.totalYieldWithCommissionPercent = -0.0006332f;
        operation2.remainedMoney                   = Quotation(197466, -266500000);
        operation2.totalMoney                      = Quotation(199999, -266500000);
        operation2.pricePrecision                  = 2;
        operation2.paymentPrecision                = 2;
        operation2.commissionPrecision             = 4;
    }

    for (int i = 0; i < optimizedOperations.size() - 1; i += 2)
    {
        Operation& operation1 = optimizedOperations[i];
        Operation& operation2 = optimizedOperations[i + 1];

        operation1.timestamp                       = operations.size() - i;
        operation1.originalTimestamp               = operations.size() - i;
        operation1.instrumentId                    = "aaaaa";
        operation1.instrumentTicker                = "aaaaa";
        operation1.instrumentName                  = "?????";
        operation1.description                     = "Sell 10 ivashka durashka shares";
        operation1.price                           = 280.0f;
        operation1.avgPriceFifo                    = 253.3f;
        operation1.avgPriceWavg                    = 253.3f;
        operation1.quantity                        = 10;
        operation1.remainedQuantity                = 0;
        operation1.payment                         = 2800.0f;
        operation1.avgCostFifo                     = 2533.0f;
        operation1.costFifo                        = Quotation(0, 0);
        operation1.costWavg                        = Quotation(0, 0);
        operation1.commission                      = -1.4f;
        operation1.yield                           = 267.0f;
        operation1.yieldWithCommission             = 265.6f;
        operation1.yieldWithCommissionPercent      = 10.4856f;
        operation1.inputMoney                      = Quotation(200000, 0);
        operation1.maxInputMoney                   = Quotation(200000, 0);
        operation1.totalYieldWithCommission        = Quotation(265, -666500000);
        operation1.totalYieldWithCommissionPercent = 0.1321667f;
        operation1.remainedMoney                   = Quotation(200300, 0);
        operation1.totalMoney                      = Quotation(200300, 0);
        operation1.pricePrecision                  = 2;
        operation1.paymentPrecision                = 2;
        operation1.commissionPrecision             = 2;

        operation2.timestamp                       = operations.size() - i - 1;
        operation2.originalTimestamp               = operations.size() - i - 1;
        operation2.instrumentId                    = "aaaaa";
        operation2.instrumentTicker                = "aaaaa";
        operation2.instrumentName                  = "?????";
        operation2.description                     = "Buy 10 ivashka durashka shares";
        operation2.price                           = 253.3f;
        operation2.avgPriceFifo                    = 253.3f;
        operation2.avgPriceWavg                    = 253.3f;
        operation2.quantity                        = 10;
        operation2.remainedQuantity                = 10;
        operation2.payment                         = -2533.0f;
        operation2.avgCostFifo                     = 2533.0f;
        operation2.costFifo                        = Quotation(2533, 0);
        operation2.costWavg                        = Quotation(2533, 0);
        operation2.commission                      = -1.2665f;
        operation2.yield                           = 0.0f;
        operation2.yieldWithCommission             = -1.2665f;
        operation2.yieldWithCommissionPercent      = -0.05f;
        operation2.inputMoney                      = Quotation(200000, 0);
        operation2.maxInputMoney                   = Quotation(200000, 0);
        operation2.totalYieldWithCommission        = Quotation(-1, -266500000);
        operation2.totalYieldWithCommissionPercent = -0.0006332f;
        operation2.remainedMoney                   = Quotation(197466, -266500000);
        operation2.totalMoney                      = Quotation(199999, -266500000);
        operation2.pricePrecision                  = 2;
        operation2.paymentPrecision                = 2;
        operation2.commissionPrecision             = 4;
    }

    Operation& lastOperation1 = operations[operations.size() - 1];
    Operation& lastOperation2 = operations[operations.size() - 2];

    lastOperation1.timestamp                       = 1;
    lastOperation1.originalTimestamp               = 1;
    lastOperation1.instrumentId                    = "bbbbb";
    lastOperation1.instrumentTicker                = "bbbbb";
    lastOperation1.instrumentName                  = "?????";
    lastOperation1.description                     = "Buy 10 mega bonds";
    lastOperation1.price                           = 253.3f;
    lastOperation1.avgPriceFifo                    = 253.3f;
    lastOperation1.avgPriceWavg                    = 253.3f;
    lastOperation1.quantity                        = 10;
    lastOperation1.remainedQuantity                = 10;
    lastOperation1.payment                         = -2533.0f;
    lastOperation1.avgCostFifo                     = 2533.0f;
    lastOperation1.costFifo                        = Quotation(2533, 0);
    lastOperation1.costWavg                        = Quotation(2533, 0);
    lastOperation1.commission                      = -1.2665f;
    lastOperation1.yield                           = 0.0f;
    lastOperation1.yieldWithCommission             = -1.2665f;
    lastOperation1.yieldWithCommissionPercent      = -0.05f;
    lastOperation1.inputMoney                      = Quotation(200000, 0);
    lastOperation1.maxInputMoney                   = Quotation(200000, 0);
    lastOperation1.totalYieldWithCommission        = Quotation(-1, -266500000);
    lastOperation1.totalYieldWithCommissionPercent = -0.0006332f;
    lastOperation1.remainedMoney                   = Quotation(197466, -266500000);
    lastOperation1.totalMoney                      = Quotation(199999, -266500000);
    lastOperation1.pricePrecision                  = 2;
    lastOperation1.paymentPrecision                = 2;
    lastOperation1.commissionPrecision             = 4;

    lastOperation2.timestamp                       = 2;
    lastOperation2.originalTimestamp               = 2;
    lastOperation2.instrumentId                    = "ccccc";
    lastOperation2.instrumentTicker                = "ccccc";
    lastOperation2.instrumentName                  = "?????";
    lastOperation2.description                     = "Buy 10 mega bonds";
    lastOperation2.price                           = 253.3f;
    lastOperation2.avgPriceFifo                    = 253.3f;
    lastOperation2.avgPriceWavg                    = 253.3f;
    lastOperation2.quantity                        = 10;
    lastOperation2.remainedQuantity                = 10;
    lastOperation2.payment                         = -2533.0f;
    lastOperation2.avgCostFifo                     = 2533.0f;
    lastOperation2.costFifo                        = Quotation(2533, 0);
    lastOperation2.costWavg                        = Quotation(2533, 0);
    lastOperation2.commission                      = -1.2665f;
    lastOperation2.yield                           = 0.0f;
    lastOperation2.yieldWithCommission             = -1.2665f;
    lastOperation2.yieldWithCommissionPercent      = -0.05f;
    lastOperation2.inputMoney                      = Quotation(200000, 0);
    lastOperation2.maxInputMoney                   = Quotation(200000, 0);
    lastOperation2.totalYieldWithCommission        = Quotation(-1, -266500000);
    lastOperation2.totalYieldWithCommissionPercent = -0.0006332f;
    lastOperation2.remainedMoney                   = Quotation(197466, -266500000);
    lastOperation2.totalMoney                      = Quotation(199999, -266500000);
    lastOperation2.pricePrecision                  = 2;
    lastOperation2.paymentPrecision                = 2;
    lastOperation2.commissionPrecision             = 4;

    optimizedOperations[optimizedOperations.size() - 1] = lastOperation1;
    optimizedOperations[optimizedOperations.size() - 2] = lastOperation2;

    QList<Operation> result = optimizer->optimizeOperations(operations, 3, QStringList() << "bbbbb" << "ccccc");

    ASSERT_EQ(result, optimizedOperations);
}

TEST_F(Test_Optimizer, Test_optimizeLogs)
{
    QList<LogEntry> entries;
    QList<LogEntry> optimizedEntries;

    entries.resizeForOverwrite(11);
    optimizedEntries.resizeForOverwrite(5);

    for (int i = 0; i < entries.size(); ++i)
    {
        LogEntry& entry = entries[i];

        entry.timestamp        = entries.size() - i;
        entry.level            = LOG_LEVEL_DEBUG;
        entry.instrumentId     = "aaaaa";
        entry.instrumentLogo   = nullptr;
        entry.instrumentTicker = "MAFA";
        entry.instrumentName   = "Mathafaka";
        entry.message          = "Buy without reason";
    }

    for (int i = 0; i < optimizedEntries.size(); ++i)
    {
        LogEntry& entry = optimizedEntries[i];

        entry.timestamp        = entries.size() - i;
        entry.level            = LOG_LEVEL_DEBUG;
        entry.instrumentId     = "aaaaa";
        entry.instrumentLogo   = nullptr;
        entry.instrumentTicker = "MAFA";
        entry.instrumentName   = "Mathafaka";
        entry.message          = "Buy without reason";
    }

    QList<LogEntry> result = optimizer->optimizeLogs(entries, 5);

    ASSERT_EQ(result, optimizedEntries);
}
