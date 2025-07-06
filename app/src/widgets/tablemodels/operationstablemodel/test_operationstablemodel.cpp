#include "src/widgets/tablemodels/operationstablemodel/operationstablemodel.h"

#include <QBrush>
#include <gtest/gtest.h>

#include "src/widgets/tablemodels/modelroles.h"



class Test_OperationsTableModel : public ::testing::Test
{
protected:
    void SetUp() override
    {
        model = new OperationsTableModel();
    }

    void TearDown() override
    {
        delete model;
    }

    OperationsTableModel* model;
};



TEST_F(Test_OperationsTableModel, Test_constructor_and_destructor)
{
}

TEST_F(Test_OperationsTableModel, Test_rowCount)
{
    ASSERT_EQ(model->rowCount(), 0);

    QList<Operation> operations;

    operations.resize(3);

    model->operationsRead(operations);
    ASSERT_EQ(model->rowCount(), 3);

    operations.resize(5);

    model->operationsRead(operations);
    ASSERT_EQ(model->rowCount(), 5);

    operations.clear();

    model->operationsRead(operations);
    ASSERT_EQ(model->rowCount(), 0);
}

TEST_F(Test_OperationsTableModel, Test_columnCount)
{
    ASSERT_EQ(model->columnCount(), OPERATIONS_COLUMN_COUNT);
}

TEST_F(Test_OperationsTableModel, Test_headerData)
{
    // clang-format off
    ASSERT_EQ(model->headerData(OPERATIONS_TIME_COLUMN,                                Qt::Horizontal, Qt::DisplayRole),           QVariant("Time"));
    ASSERT_EQ(model->headerData(OPERATIONS_NAME_COLUMN,                                Qt::Horizontal, Qt::DisplayRole),           QVariant("Name"));
    ASSERT_EQ(model->headerData(OPERATIONS_DESCRIPTION_COLUMN,                         Qt::Horizontal, Qt::DisplayRole),           QVariant("Description"));
    ASSERT_EQ(model->headerData(OPERATIONS_PRICE_COLUMN,                               Qt::Horizontal, Qt::DisplayRole),           QVariant("Price"));
    ASSERT_EQ(model->headerData(OPERATIONS_AVG_PRICE_FIFO_COLUMN,                      Qt::Horizontal, Qt::DisplayRole),           QVariant("Avg price (FIFO)"));
    ASSERT_EQ(model->headerData(OPERATIONS_AVG_PRICE_WAVG_COLUMN,                      Qt::Horizontal, Qt::DisplayRole),           QVariant("Avg price (WAVG)"));
    ASSERT_EQ(model->headerData(OPERATIONS_QUANTITY_COLUMN,                            Qt::Horizontal, Qt::DisplayRole),           QVariant("Quantity"));
    ASSERT_EQ(model->headerData(OPERATIONS_REMAINED_QUANTITY_COLUMN,                   Qt::Horizontal, Qt::DisplayRole),           QVariant("Remained quantity"));
    ASSERT_EQ(model->headerData(OPERATIONS_PAYMENT_COLUMN,                             Qt::Horizontal, Qt::DisplayRole),           QVariant("Payment"));
    ASSERT_EQ(model->headerData(OPERATIONS_COMMISSION_COLUMN,                          Qt::Horizontal, Qt::DisplayRole),           QVariant("Commission"));
    ASSERT_EQ(model->headerData(OPERATIONS_YIELD_COLUMN,                               Qt::Horizontal, Qt::DisplayRole),           QVariant("Yield"));
    ASSERT_EQ(model->headerData(OPERATIONS_YIELD_WITH_COMMISSION_COLUMN,               Qt::Horizontal, Qt::DisplayRole),           QVariant("Yield with commission"));
    ASSERT_EQ(model->headerData(OPERATIONS_YIELD_WITH_COMMISSION_PERCENT_COLUMN,       Qt::Horizontal, Qt::DisplayRole),           QVariant("Yield with commission (%)"));
    ASSERT_EQ(model->headerData(OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_COLUMN,         Qt::Horizontal, Qt::DisplayRole),           QVariant("Total yield"));
    ASSERT_EQ(model->headerData(OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_PERCENT_COLUMN, Qt::Horizontal, Qt::DisplayRole),           QVariant("Total yield (%)"));
    ASSERT_EQ(model->headerData(OPERATIONS_REMAINED_MONEY_COLUMN,                      Qt::Horizontal, Qt::DisplayRole),           QVariant("Remained money"));
    ASSERT_EQ(model->headerData(OPERATIONS_TOTAL_MONEY_COLUMN,                         Qt::Horizontal, Qt::DisplayRole),           QVariant("Total money"));
    ASSERT_EQ(model->headerData(0,                                                     Qt::Vertical,   Qt::DisplayRole),           QVariant(1));
    ASSERT_EQ(model->headerData(1,                                                     Qt::Vertical,   Qt::DisplayRole),           QVariant(2));
    ASSERT_EQ(model->headerData(2,                                                     Qt::Vertical,   Qt::DisplayRole),           QVariant(3));
    ASSERT_EQ(model->headerData(0,                                                     Qt::Horizontal, Qt::WhatsThisPropertyRole), QVariant());
    // clang-format on
}

TEST_F(Test_OperationsTableModel, Test_data)
{
    Logo logo;

    QList<Operation> operations;

    Operation operation1;
    Operation operation2;
    Operation operation3;

    operation1.timestamp                       = 1704056520000;
    operation1.instrumentId                    = "aaaaa";
    operation1.instrumentLogo                  = nullptr;
    operation1.instrumentTicker                = "ABBA";
    operation1.instrumentName                  = "Abstract Basics";
    operation1.description                     = "A basket full";
    operation1.price                           = 100.0f;
    operation1.avgPriceFifo                    = 101.0f;
    operation1.avgPriceWavg                    = 102.0f;
    operation1.quantity                        = 103;
    operation1.remainedQuantity                = 104;
    operation1.payment                         = 105.0f;
    operation1.avgCostFifo                     = 106.0f;
    operation1.costFifo.units                  = 107;
    operation1.costFifo.nano                   = 108;
    operation1.costWavg.units                  = 109;
    operation1.costWavg.nano                   = 110;
    operation1.commission                      = 111.0f;
    operation1.yield                           = 112.0f;
    operation1.yieldWithCommission             = 113.0f;
    operation1.yieldWithCommissionPercent      = 114.0f;
    operation1.inputMoney.units                = 115;
    operation1.inputMoney.nano                 = 116;
    operation1.maxInputMoney.units             = 117;
    operation1.maxInputMoney.nano              = 118;
    operation1.totalYieldWithCommission.units  = 119;
    operation1.totalYieldWithCommission.nano   = 120;
    operation1.totalYieldWithCommissionPercent = 121.0f;
    operation1.remainedMoney.units             = 122;
    operation1.remainedMoney.nano              = 123;
    operation1.totalMoney.units                = 124;
    operation1.totalMoney.nano                 = 125;
    operation1.pricePrecision                  = 2;
    operation1.paymentPrecision                = 3;
    operation1.commissionPrecision             = 4;

    operation2.timestamp                       = 1704056460000;
    operation2.instrumentId                    = "bbbbb";
    operation2.instrumentLogo                  = nullptr;
    operation2.instrumentTicker                = "BASE";
    operation2.instrumentName                  = "Basketball enhancement";
    operation2.description                     = "Ball shall levitate";
    operation2.price                           = 200.0f;
    operation2.avgPriceFifo                    = 201.0f;
    operation2.avgPriceWavg                    = 202.0f;
    operation2.quantity                        = 203;
    operation2.remainedQuantity                = 204;
    operation2.payment                         = 205.0f;
    operation2.avgCostFifo                     = 206.0f;
    operation2.costFifo.units                  = 207;
    operation2.costFifo.nano                   = 208;
    operation2.costWavg.units                  = 209;
    operation2.costWavg.nano                   = 210;
    operation2.commission                      = 211.0f;
    operation2.yield                           = 212.0f;
    operation2.yieldWithCommission             = 213.0f;
    operation2.yieldWithCommissionPercent      = 214.0f;
    operation2.inputMoney.units                = 215;
    operation2.inputMoney.nano                 = 216;
    operation2.maxInputMoney.units             = 217;
    operation2.maxInputMoney.nano              = 218;
    operation2.totalYieldWithCommission.units  = 219;
    operation2.totalYieldWithCommission.nano   = 220;
    operation2.totalYieldWithCommissionPercent = 221.0f;
    operation2.remainedMoney.units             = 222;
    operation2.remainedMoney.nano              = 223;
    operation2.totalMoney.units                = 224;
    operation2.totalMoney.nano                 = 225;
    operation2.pricePrecision                  = 3;
    operation2.paymentPrecision                = 4;
    operation2.commissionPrecision             = 2;

    operation3.timestamp                       = 1704056400000;
    operation3.instrumentId                    = "ccccc";
    operation3.instrumentLogo                  = &logo;
    operation3.instrumentTicker                = "CODE";
    operation3.instrumentName                  = "Code base";
    operation3.description                     = "Can you eat fish?";
    operation3.price                           = 500.0f;
    operation3.avgPriceFifo                    = 501.0f;
    operation3.avgPriceWavg                    = 502.0f;
    operation3.quantity                        = 503;
    operation3.remainedQuantity                = 504;
    operation3.payment                         = 505.0f;
    operation3.avgCostFifo                     = 506.0f;
    operation3.costFifo.units                  = 507;
    operation3.costFifo.nano                   = 508;
    operation3.costWavg.units                  = 509;
    operation3.costWavg.nano                   = 510;
    operation3.commission                      = 511.0f;
    operation3.yield                           = 512.0f;
    operation3.yieldWithCommission             = 513.0f;
    operation3.yieldWithCommissionPercent      = 514.0f;
    operation3.inputMoney.units                = 515;
    operation3.inputMoney.nano                 = 516;
    operation3.maxInputMoney.units             = 517;
    operation3.maxInputMoney.nano              = 518;
    operation3.totalYieldWithCommission.units  = 519;
    operation3.totalYieldWithCommission.nano   = 520;
    operation3.totalYieldWithCommissionPercent = 521.0f;
    operation3.remainedMoney.units             = 522;
    operation3.remainedMoney.nano              = 523;
    operation3.totalMoney.units                = 524;
    operation3.totalMoney.nano                 = 525;
    operation3.pricePrecision                  = 4;
    operation3.paymentPrecision                = 2;
    operation3.commissionPrecision             = 3;
    operations << operation1 << operation2 << operation3;

    model->operationsRead(operations);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_TIME_COLUMN),                                Qt::DisplayRole),           QVariant("2024-01-01 00:02:00"));
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_NAME_COLUMN),                                Qt::DisplayRole),           QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_DESCRIPTION_COLUMN),                         Qt::DisplayRole),           QVariant("A basket full"));
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_PRICE_COLUMN),                               Qt::DisplayRole),           QVariant("100.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_AVG_PRICE_FIFO_COLUMN),                      Qt::DisplayRole),           QVariant("101.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_AVG_PRICE_WAVG_COLUMN),                      Qt::DisplayRole),           QVariant("102.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_QUANTITY_COLUMN),                            Qt::DisplayRole),           QVariant("103"));
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_REMAINED_QUANTITY_COLUMN),                   Qt::DisplayRole),           QVariant("104"));
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_PAYMENT_COLUMN),                             Qt::DisplayRole),           QVariant("+105.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_COMMISSION_COLUMN),                          Qt::DisplayRole),           QVariant("+111.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_YIELD_COLUMN),                               Qt::DisplayRole),           QVariant("+112.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_YIELD_WITH_COMMISSION_COLUMN),               Qt::DisplayRole),           QVariant("+113.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_YIELD_WITH_COMMISSION_PERCENT_COLUMN),       Qt::DisplayRole),           QVariant("+114.00%"));
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_COLUMN),         Qt::DisplayRole),           QVariant("+119.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_PERCENT_COLUMN), Qt::DisplayRole),           QVariant("+121.00%"));
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_REMAINED_MONEY_COLUMN),                      Qt::DisplayRole),           QVariant("122.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_TOTAL_MONEY_COLUMN),                         Qt::DisplayRole),           QVariant("124.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_TIME_COLUMN),                                Qt::DisplayRole),           QVariant("2024-01-01 00:01:00"));
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_NAME_COLUMN),                                Qt::DisplayRole),           QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_DESCRIPTION_COLUMN),                         Qt::DisplayRole),           QVariant("Ball shall levitate"));
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_PRICE_COLUMN),                               Qt::DisplayRole),           QVariant("200.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_AVG_PRICE_FIFO_COLUMN),                      Qt::DisplayRole),           QVariant("201.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_AVG_PRICE_WAVG_COLUMN),                      Qt::DisplayRole),           QVariant("202.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_QUANTITY_COLUMN),                            Qt::DisplayRole),           QVariant("203"));
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_REMAINED_QUANTITY_COLUMN),                   Qt::DisplayRole),           QVariant("204"));
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_PAYMENT_COLUMN),                             Qt::DisplayRole),           QVariant("+205.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_COMMISSION_COLUMN),                          Qt::DisplayRole),           QVariant("+211.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_YIELD_COLUMN),                               Qt::DisplayRole),           QVariant("+212.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_YIELD_WITH_COMMISSION_COLUMN),               Qt::DisplayRole),           QVariant("+213.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_YIELD_WITH_COMMISSION_PERCENT_COLUMN),       Qt::DisplayRole),           QVariant("+214.00%"));
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_COLUMN),         Qt::DisplayRole),           QVariant("+219.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_PERCENT_COLUMN), Qt::DisplayRole),           QVariant("+221.00%"));
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_REMAINED_MONEY_COLUMN),                      Qt::DisplayRole),           QVariant("222.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_TOTAL_MONEY_COLUMN),                         Qt::DisplayRole),           QVariant("224.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_TIME_COLUMN),                                Qt::DisplayRole),           QVariant("2024-01-01 00:00:00"));
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_NAME_COLUMN),                                Qt::DisplayRole),           QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_DESCRIPTION_COLUMN),                         Qt::DisplayRole),           QVariant("Can you eat fish?"));
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_PRICE_COLUMN),                               Qt::DisplayRole),           QVariant("500.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_AVG_PRICE_FIFO_COLUMN),                      Qt::DisplayRole),           QVariant("501.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_AVG_PRICE_WAVG_COLUMN),                      Qt::DisplayRole),           QVariant("502.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_QUANTITY_COLUMN),                            Qt::DisplayRole),           QVariant("503"));
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_REMAINED_QUANTITY_COLUMN),                   Qt::DisplayRole),           QVariant("504"));
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_PAYMENT_COLUMN),                             Qt::DisplayRole),           QVariant("+505.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_COMMISSION_COLUMN),                          Qt::DisplayRole),           QVariant("+511.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_YIELD_COLUMN),                               Qt::DisplayRole),           QVariant("+512.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_YIELD_WITH_COMMISSION_COLUMN),               Qt::DisplayRole),           QVariant("+513.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_YIELD_WITH_COMMISSION_PERCENT_COLUMN),       Qt::DisplayRole),           QVariant("+514.00%"));
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_COLUMN),         Qt::DisplayRole),           QVariant("+519.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_PERCENT_COLUMN), Qt::DisplayRole),           QVariant("+521.00%"));
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_REMAINED_MONEY_COLUMN),                      Qt::DisplayRole),           QVariant("522.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_TOTAL_MONEY_COLUMN),                         Qt::DisplayRole),           QVariant("524.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_TIME_COLUMN),                                Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_NAME_COLUMN),                                Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_DESCRIPTION_COLUMN),                         Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_PRICE_COLUMN),                               Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_AVG_PRICE_FIFO_COLUMN),                      Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_AVG_PRICE_WAVG_COLUMN),                      Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_QUANTITY_COLUMN),                            Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_REMAINED_QUANTITY_COLUMN),                   Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_PAYMENT_COLUMN),                             Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_COMMISSION_COLUMN),                          Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_YIELD_COLUMN),                               Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_YIELD_WITH_COMMISSION_COLUMN),               Qt::ForegroundRole),        QVariant(QBrush(QColor("#2BD793"))));
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_YIELD_WITH_COMMISSION_PERCENT_COLUMN),       Qt::ForegroundRole),        QVariant(QBrush(QColor("#2BD793"))));
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_COLUMN),         Qt::ForegroundRole),        QVariant(QBrush(QColor("#2BD793"))));
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_PERCENT_COLUMN), Qt::ForegroundRole),        QVariant(QBrush(QColor("#2BD793"))));
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_REMAINED_MONEY_COLUMN),                      Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_TOTAL_MONEY_COLUMN),                         Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_TIME_COLUMN),                                Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_NAME_COLUMN),                                Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_DESCRIPTION_COLUMN),                         Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_PRICE_COLUMN),                               Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_AVG_PRICE_FIFO_COLUMN),                      Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_AVG_PRICE_WAVG_COLUMN),                      Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_QUANTITY_COLUMN),                            Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_REMAINED_QUANTITY_COLUMN),                   Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_PAYMENT_COLUMN),                             Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_COMMISSION_COLUMN),                          Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_YIELD_COLUMN),                               Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_YIELD_WITH_COMMISSION_COLUMN),               Qt::ForegroundRole),        QVariant(QBrush(QColor("#2BD793"))));
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_YIELD_WITH_COMMISSION_PERCENT_COLUMN),       Qt::ForegroundRole),        QVariant(QBrush(QColor("#2BD793"))));
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_COLUMN),         Qt::ForegroundRole),        QVariant(QBrush(QColor("#2BD793"))));
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_PERCENT_COLUMN), Qt::ForegroundRole),        QVariant(QBrush(QColor("#2BD793"))));
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_REMAINED_MONEY_COLUMN),                      Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_TOTAL_MONEY_COLUMN),                         Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_TIME_COLUMN),                                Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_NAME_COLUMN),                                Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_DESCRIPTION_COLUMN),                         Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_PRICE_COLUMN),                               Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_AVG_PRICE_FIFO_COLUMN),                      Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_AVG_PRICE_WAVG_COLUMN),                      Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_QUANTITY_COLUMN),                            Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_REMAINED_QUANTITY_COLUMN),                   Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_PAYMENT_COLUMN),                             Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_COMMISSION_COLUMN),                          Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_YIELD_COLUMN),                               Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_YIELD_WITH_COMMISSION_COLUMN),               Qt::ForegroundRole),        QVariant(QBrush(QColor("#2BD793"))));
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_YIELD_WITH_COMMISSION_PERCENT_COLUMN),       Qt::ForegroundRole),        QVariant(QBrush(QColor("#2BD793"))));
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_COLUMN),         Qt::ForegroundRole),        QVariant(QBrush(QColor("#2BD793"))));
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_PERCENT_COLUMN), Qt::ForegroundRole),        QVariant(QBrush(QColor("#2BD793"))));
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_REMAINED_MONEY_COLUMN),                      Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_TOTAL_MONEY_COLUMN),                         Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_TIME_COLUMN),                                Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_NAME_COLUMN),                                Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_DESCRIPTION_COLUMN),                         Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_PRICE_COLUMN),                               Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_AVG_PRICE_FIFO_COLUMN),                      Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_AVG_PRICE_WAVG_COLUMN),                      Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_QUANTITY_COLUMN),                            Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_REMAINED_QUANTITY_COLUMN),                   Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_PAYMENT_COLUMN),                             Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_COMMISSION_COLUMN),                          Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_YIELD_COLUMN),                               Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_YIELD_WITH_COMMISSION_COLUMN),               Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_YIELD_WITH_COMMISSION_PERCENT_COLUMN),       Qt::ToolTipRole),           QVariant("From: 106.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_COLUMN),         Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_PERCENT_COLUMN), Qt::ToolTipRole),           QVariant("From: 117.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_REMAINED_MONEY_COLUMN),                      Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_TOTAL_MONEY_COLUMN),                         Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_TIME_COLUMN),                                Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_NAME_COLUMN),                                Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_DESCRIPTION_COLUMN),                         Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_PRICE_COLUMN),                               Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_AVG_PRICE_FIFO_COLUMN),                      Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_AVG_PRICE_WAVG_COLUMN),                      Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_QUANTITY_COLUMN),                            Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_REMAINED_QUANTITY_COLUMN),                   Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_PAYMENT_COLUMN),                             Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_COMMISSION_COLUMN),                          Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_YIELD_COLUMN),                               Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_YIELD_WITH_COMMISSION_COLUMN),               Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_YIELD_WITH_COMMISSION_PERCENT_COLUMN),       Qt::ToolTipRole),           QVariant("From: 206.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_COLUMN),         Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_PERCENT_COLUMN), Qt::ToolTipRole),           QVariant("From: 217.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_REMAINED_MONEY_COLUMN),                      Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_TOTAL_MONEY_COLUMN),                         Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_TIME_COLUMN),                                Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_NAME_COLUMN),                                Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_DESCRIPTION_COLUMN),                         Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_PRICE_COLUMN),                               Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_AVG_PRICE_FIFO_COLUMN),                      Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_AVG_PRICE_WAVG_COLUMN),                      Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_QUANTITY_COLUMN),                            Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_REMAINED_QUANTITY_COLUMN),                   Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_PAYMENT_COLUMN),                             Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_COMMISSION_COLUMN),                          Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_YIELD_COLUMN),                               Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_YIELD_WITH_COMMISSION_COLUMN),               Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_YIELD_WITH_COMMISSION_PERCENT_COLUMN),       Qt::ToolTipRole),           QVariant("From: 506.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_COLUMN),         Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_PERCENT_COLUMN), Qt::ToolTipRole),           QVariant("From: 517.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_REMAINED_MONEY_COLUMN),                      Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_TOTAL_MONEY_COLUMN),                         Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_NAME_COLUMN),                                ROLE_INSTRUMENT_LOGO),      QVariant(0));
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_NAME_COLUMN),                                ROLE_INSTRUMENT_LOGO),      QVariant(0));
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_NAME_COLUMN),                                ROLE_INSTRUMENT_LOGO),      QVariant(reinterpret_cast<qint64>(&logo)));
    ASSERT_EQ(model->data(model->index(0, OPERATIONS_NAME_COLUMN),                                ROLE_INSTRUMENT_NAME),      QVariant("Abstract Basics"));
    ASSERT_EQ(model->data(model->index(1, OPERATIONS_NAME_COLUMN),                                ROLE_INSTRUMENT_NAME),      QVariant("Basketball enhancement"));
    ASSERT_EQ(model->data(model->index(2, OPERATIONS_NAME_COLUMN),                                ROLE_INSTRUMENT_NAME),      QVariant("Code base"));
    ASSERT_EQ(model->data(model->index(0, 0),                                                     Qt::WhatsThisPropertyRole), QVariant());
    // clang-format on
}
