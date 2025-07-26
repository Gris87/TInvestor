#include "src/utils/optimizer/optimizer.h"

#include <gtest/gtest.h>



TEST(Test_Optimizer, Test_constructor_and_destructor)
{
}

/*
TEST_F(Test_Optimizer, Test_optimizeOperations)
{
    const InSequence seq;

    EXPECT_CALL(*operationsDatabaseMock, setAccount(QString("account-hash")));

    thread->setAccountId("account-hash", "account-id");

    QList<Operation> operations;
    QList<Operation> optimizedOperations;

    operations.resizeForOverwrite(11);
    optimizedOperations.resizeForOverwrite(5);

    thread->testSetLimitOperations(operations.size() - 1);
    thread->testSetOptimizeSize(optimizedOperations.size() - 2);

    for (int i = 0; i < operations.size() - 1; i += 2)
    {
        Operation& operation1 = operations[i];
        Operation& operation2 = operations[i + 1];

        operation1.timestamp                       = operations.size() - i;
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
        operation1.costFifo.units                  = 0;
        operation1.costFifo.nano                   = 0;
        operation1.costWavg.units                  = 0;
        operation1.costWavg.nano                   = 0;
        operation1.commission                      = -1.4f;
        operation1.yield                           = 267.0f;
        operation1.yieldWithCommission             = 265.6f;
        operation1.yieldWithCommissionPercent      = 10.4856f;
        operation1.inputMoney.units                = 200000;
        operation1.inputMoney.nano                 = 0;
        operation1.maxInputMoney.units             = 200000;
        operation1.maxInputMoney.nano              = 0;
        operation1.totalYieldWithCommission.units  = 265;
        operation1.totalYieldWithCommission.nano   = -666500000;
        operation1.totalYieldWithCommissionPercent = 0.1321667f;
        operation1.remainedMoney.units             = 200300;
        operation1.remainedMoney.nano              = 0;
        operation1.totalMoney.units                = 200300;
        operation1.totalMoney.nano                 = 0;
        operation1.pricePrecision                  = 2;
        operation1.paymentPrecision                = 2;
        operation1.commissionPrecision             = 2;

        operation2.timestamp                       = operations.size() - i - 1;
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
        operation2.costFifo.units                  = 2533;
        operation2.costFifo.nano                   = 0;
        operation2.costWavg.units                  = 2533;
        operation2.costWavg.nano                   = 0;
        operation2.commission                      = -1.2665f;
        operation2.yield                           = 0.0f;
        operation2.yieldWithCommission             = -1.2665f;
        operation2.yieldWithCommissionPercent      = -0.05f;
        operation2.inputMoney.units                = 200000;
        operation2.inputMoney.nano                 = 0;
        operation2.maxInputMoney.units             = 200000;
        operation2.maxInputMoney.nano              = 0;
        operation2.totalYieldWithCommission.units  = -1;
        operation2.totalYieldWithCommission.nano   = -266500000;
        operation2.totalYieldWithCommissionPercent = -0.0006332f;
        operation2.remainedMoney.units             = 197466;
        operation2.remainedMoney.nano              = -266500000;
        operation2.totalMoney.units                = 199999;
        operation2.totalMoney.nano                 = -266500000;
        operation2.pricePrecision                  = 2;
        operation2.paymentPrecision                = 2;
        operation2.commissionPrecision             = 4;
    }

    for (int i = 0; i < optimizedOperations.size() - 1; i += 2)
    {
        Operation& operation1 = optimizedOperations[i];
        Operation& operation2 = optimizedOperations[i + 1];

        operation1.timestamp                       = operations.size() - i;
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
        operation1.costFifo.units                  = 0;
        operation1.costFifo.nano                   = 0;
        operation1.costWavg.units                  = 0;
        operation1.costWavg.nano                   = 0;
        operation1.commission                      = -1.4f;
        operation1.yield                           = 267.0f;
        operation1.yieldWithCommission             = 265.6f;
        operation1.yieldWithCommissionPercent      = 10.4856f;
        operation1.inputMoney.units                = 200000;
        operation1.inputMoney.nano                 = 0;
        operation1.maxInputMoney.units             = 200000;
        operation1.maxInputMoney.nano              = 0;
        operation1.totalYieldWithCommission.units  = 265;
        operation1.totalYieldWithCommission.nano   = -666500000;
        operation1.totalYieldWithCommissionPercent = 0.1321667f;
        operation1.remainedMoney.units             = 200300;
        operation1.remainedMoney.nano              = 0;
        operation1.totalMoney.units                = 200300;
        operation1.totalMoney.nano                 = 0;
        operation1.pricePrecision                  = 2;
        operation1.paymentPrecision                = 2;
        operation1.commissionPrecision             = 2;

        operation2.timestamp                       = operations.size() - i - 1;
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
        operation2.costFifo.units                  = 2533;
        operation2.costFifo.nano                   = 0;
        operation2.costWavg.units                  = 2533;
        operation2.costWavg.nano                   = 0;
        operation2.commission                      = -1.2665f;
        operation2.yield                           = 0.0f;
        operation2.yieldWithCommission             = -1.2665f;
        operation2.yieldWithCommissionPercent      = -0.05f;
        operation2.inputMoney.units                = 200000;
        operation2.inputMoney.nano                 = 0;
        operation2.maxInputMoney.units             = 200000;
        operation2.maxInputMoney.nano              = 0;
        operation2.totalYieldWithCommission.units  = -1;
        operation2.totalYieldWithCommission.nano   = -266500000;
        operation2.totalYieldWithCommissionPercent = -0.0006332f;
        operation2.remainedMoney.units             = 197466;
        operation2.remainedMoney.nano              = -266500000;
        operation2.totalMoney.units                = 199999;
        operation2.totalMoney.nano                 = -266500000;
        operation2.pricePrecision                  = 2;
        operation2.paymentPrecision                = 2;
        operation2.commissionPrecision             = 4;
    }

    Operation& lastOperation1 = operations[operations.size() - 1];
    Operation& lastOperation2 = operations[operations.size() - 2];

    lastOperation1.timestamp                       = 1;
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
    lastOperation1.costFifo.units                  = 2533;
    lastOperation1.costFifo.nano                   = 0;
    lastOperation1.costWavg.units                  = 2533;
    lastOperation1.costWavg.nano                   = 0;
    lastOperation1.commission                      = -1.2665f;
    lastOperation1.yield                           = 0.0f;
    lastOperation1.yieldWithCommission             = -1.2665f;
    lastOperation1.yieldWithCommissionPercent      = -0.05f;
    lastOperation1.inputMoney.units                = 200000;
    lastOperation1.inputMoney.nano                 = 0;
    lastOperation1.maxInputMoney.units             = 200000;
    lastOperation1.maxInputMoney.nano              = 0;
    lastOperation1.totalYieldWithCommission.units  = -1;
    lastOperation1.totalYieldWithCommission.nano   = -266500000;
    lastOperation1.totalYieldWithCommissionPercent = -0.0006332f;
    lastOperation1.remainedMoney.units             = 197466;
    lastOperation1.remainedMoney.nano              = -266500000;
    lastOperation1.totalMoney.units                = 199999;
    lastOperation1.totalMoney.nano                 = -266500000;
    lastOperation1.pricePrecision                  = 2;
    lastOperation1.paymentPrecision                = 2;
    lastOperation1.commissionPrecision             = 4;

    lastOperation2.timestamp                       = 2;
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
    lastOperation2.costFifo.units                  = 2533;
    lastOperation2.costFifo.nano                   = 0;
    lastOperation2.costWavg.units                  = 2533;
    lastOperation2.costWavg.nano                   = 0;
    lastOperation2.commission                      = -1.2665f;
    lastOperation2.yield                           = 0.0f;
    lastOperation2.yieldWithCommission             = -1.2665f;
    lastOperation2.yieldWithCommissionPercent      = -0.05f;
    lastOperation2.inputMoney.units                = 200000;
    lastOperation2.inputMoney.nano                 = 0;
    lastOperation2.maxInputMoney.units             = 200000;
    lastOperation2.maxInputMoney.nano              = 0;
    lastOperation2.totalYieldWithCommission.units  = -1;
    lastOperation2.totalYieldWithCommission.nano   = -266500000;
    lastOperation2.totalYieldWithCommissionPercent = -0.0006332f;
    lastOperation2.remainedMoney.units             = 197466;
    lastOperation2.remainedMoney.nano              = -266500000;
    lastOperation2.totalMoney.units                = 199999;
    lastOperation2.totalMoney.nano                 = -266500000;
    lastOperation2.pricePrecision                  = 2;
    lastOperation2.paymentPrecision                = 2;
    lastOperation2.commissionPrecision             = 4;

    optimizedOperations[optimizedOperations.size() - 1] = lastOperation1;
    optimizedOperations[optimizedOperations.size() - 2] = lastOperation2;

    const std::shared_ptr<tinkoff::PositionsResponse> positionsResponse(new tinkoff::PositionsResponse());

    tinkoff::MoneyValue* money1 = positionsResponse->add_money(); // positionsResponse will take ownership
    money1->set_currency("usd");
    money1->set_units(0);
    money1->set_nano(0);

    tinkoff::MoneyValue* money2 = positionsResponse->add_money(); // positionsResponse will take ownership
    money2->set_currency("rub");
    money2->set_units(0);
    money2->set_nano(0);

    std::shared_ptr<PositionsStream> positionsStream(new PositionsStream());

    EXPECT_CALL(*operationsDatabaseMock, readOperations()).WillOnce(Return(operations));
    EXPECT_CALL(*grpcClientMock, getPositions(QThread::currentThread(), QString("account-id")))
        .WillOnce(Return(positionsResponse));
    EXPECT_CALL(*grpcClientMock, createPositionsStream(QString("account-id"))).WillOnce(Return(positionsStream));
    EXPECT_CALL(
        *grpcClientMock, getOperations(QThread::currentThread(), QString("account-id"), 1011, Gt(1704056400000), QString(""))
    )
        .WillOnce(Return(nullptr));
    EXPECT_CALL(*operationsDatabaseMock, readOperations()).WillOnce(Return(operations));
    EXPECT_CALL(*operationsDatabaseMock, writeOperations(optimizedOperations));
    EXPECT_CALL(*grpcClientMock, readPositionsStream(positionsStream)).WillOnce(Return(nullptr));
    EXPECT_CALL(*grpcClientMock, finishPositionsStream(positionsStream));

    thread->run();
}
*/

/*
TEST_F(Test_Optimizer, Test_optimizeLogs)
{
    const InSequence seq;
    
    QList<LogEntry> entries;
    QList<LogEntry> optimizedEntries;
    
    entries.resizeForOverwrite(11);
    optimizedEntries.resizeForOverwrite(5);
    
    thread->testSetLimitLogs(entries.size() - 1);
    thread->testSetOptimizeSize(optimizedEntries.size());
    
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
    
    EXPECT_CALL(*logsDatabaseMock, readLogs()).WillOnce(Return(entries));
    EXPECT_CALL(*logsDatabaseMock, readLogs()).WillOnce(Return(entries));
    EXPECT_CALL(*logsDatabaseMock, writeLogs(optimizedEntries));
    
    thread->testTerminateWithoutTerminate();
    thread->run();
}
*/
