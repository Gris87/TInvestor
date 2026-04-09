#include "src/widgets/tablemodels/stockstablemodel/stockstablemodel.h"

#include <QBrush>
#include <gtest/gtest.h>

#include "src/storage/user/iuserstorage_mock.h"
#include "src/widgets/tablemodels/modelroles.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



class Test_StocksTableModel : public ::testing::Test
{
protected:
    void SetUp() override
    {
        userStorageMock = new StrictMock<UserStorageMock>();

        model = new StocksTableModel(userStorageMock);
    }

    void TearDown() override
    {
        delete model;
        delete userStorageMock;
    }

    StocksTableModel*            model;
    StrictMock<UserStorageMock>* userStorageMock;
};



TEST_F(Test_StocksTableModel, Test_constructor_and_destructor)
{
}

TEST_F(Test_StocksTableModel, Test_rowCount)
{
    const InSequence seq;

    ASSERT_EQ(model->rowCount(), 0);

    QList<Stock*> stocks;

    Stock stock1;
    Stock stock2;
    Stock stock3;
    Stock stock4;
    Stock stock5;

    stocks << &stock1 << &stock2 << &stock3;

    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, isQualified()).WillOnce(Return(false));
    EXPECT_CALL(*userStorageMock, readUnlock());

    model->updateTable(stocks);
    ASSERT_EQ(model->rowCount(), 3);

    stocks << &stock4 << &stock5;

    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, isQualified()).WillOnce(Return(false));
    EXPECT_CALL(*userStorageMock, readUnlock());

    model->updateTable(stocks);
    ASSERT_EQ(model->rowCount(), 5);

    stocks.clear();

    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, isQualified()).WillOnce(Return(false));
    EXPECT_CALL(*userStorageMock, readUnlock());

    model->updateTable(stocks);
    ASSERT_EQ(model->rowCount(), 0);
}

TEST_F(Test_StocksTableModel, Test_columnCount)
{
    ASSERT_EQ(model->columnCount(), STOCKS_COLUMN_COUNT);
}

TEST_F(Test_StocksTableModel, Test_headerData)
{
    // clang-format off
    ASSERT_EQ(model->headerData(STOCKS_NAME_COLUMN,        Qt::Horizontal, Qt::DisplayRole),           QVariant("Name"));
    ASSERT_EQ(model->headerData(STOCKS_PRICE_COLUMN,       Qt::Horizontal, Qt::DisplayRole),           QVariant("Price"));
    ASSERT_EQ(model->headerData(STOCKS_DAY_CHANGE_COLUMN,  Qt::Horizontal, Qt::DisplayRole),           QVariant("Change from day start"));
    ASSERT_EQ(model->headerData(STOCKS_DATE_CHANGE_COLUMN, Qt::Horizontal, Qt::DisplayRole),           QVariant("Change from some date"));
    ASSERT_EQ(model->headerData(STOCKS_TURNOVER_COLUMN,    Qt::Horizontal, Qt::DisplayRole),           QVariant("Turnover"));
    ASSERT_EQ(model->headerData(STOCKS_PAYBACK_COLUMN,     Qt::Horizontal, Qt::DisplayRole),           QVariant("Payback"));
    ASSERT_EQ(model->headerData(STOCKS_DIVIDENDS_COLUMN,   Qt::Horizontal, Qt::DisplayRole),           QVariant("Dividends"));
    ASSERT_EQ(model->headerData(STOCKS_SHORTS_COLUMN,      Qt::Horizontal, Qt::DisplayRole),           QVariant("Shorts"));
    ASSERT_EQ(model->headerData(STOCKS_ACTIONS_COLUMN,     Qt::Horizontal, Qt::DisplayRole),           QVariant("Actions"));
    ASSERT_EQ(model->headerData(0,                         Qt::Vertical,   Qt::DisplayRole),           QVariant(1));
    ASSERT_EQ(model->headerData(1,                         Qt::Vertical,   Qt::DisplayRole),           QVariant(2));
    ASSERT_EQ(model->headerData(2,                         Qt::Vertical,   Qt::DisplayRole),           QVariant(3));
    ASSERT_EQ(model->headerData(STOCKS_NAME_COLUMN,        Qt::Horizontal, Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->headerData(STOCKS_PRICE_COLUMN,       Qt::Horizontal, Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->headerData(STOCKS_DAY_CHANGE_COLUMN,  Qt::Horizontal, Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->headerData(STOCKS_DATE_CHANGE_COLUMN, Qt::Horizontal, Qt::ToolTipRole),           QVariant(""));
    ASSERT_EQ(model->headerData(STOCKS_TURNOVER_COLUMN,    Qt::Horizontal, Qt::ToolTipRole),           QVariant("Average daily turnover in the last 31 days"));
    ASSERT_EQ(model->headerData(STOCKS_PAYBACK_COLUMN,     Qt::Horizontal, Qt::ToolTipRole),           QVariant("Opportunity to get money back (with commission) based on the last 24 hours"));
    ASSERT_EQ(model->headerData(STOCKS_DIVIDENDS_COLUMN,   Qt::Horizontal, Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->headerData(STOCKS_SHORTS_COLUMN,      Qt::Horizontal, Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->headerData(STOCKS_ACTIONS_COLUMN,     Qt::Horizontal, Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->headerData(STOCKS_NAME_COLUMN,        Qt::Horizontal, Qt::DecorationRole),        QVariant());
    ASSERT_EQ(model->headerData(STOCKS_PRICE_COLUMN,       Qt::Horizontal, Qt::DecorationRole),        QVariant());
    ASSERT_EQ(model->headerData(STOCKS_DAY_CHANGE_COLUMN,  Qt::Horizontal, Qt::DecorationRole),        QVariant());
    ASSERT_NE(model->headerData(STOCKS_DATE_CHANGE_COLUMN, Qt::Horizontal, Qt::DecorationRole),        QVariant());
    ASSERT_NE(model->headerData(STOCKS_TURNOVER_COLUMN,    Qt::Horizontal, Qt::DecorationRole),        QVariant());
    ASSERT_NE(model->headerData(STOCKS_PAYBACK_COLUMN,     Qt::Horizontal, Qt::DecorationRole),        QVariant());
    ASSERT_EQ(model->headerData(STOCKS_DIVIDENDS_COLUMN,   Qt::Horizontal, Qt::DecorationRole),        QVariant());
    ASSERT_EQ(model->headerData(STOCKS_SHORTS_COLUMN,      Qt::Horizontal, Qt::DecorationRole),        QVariant());
    ASSERT_EQ(model->headerData(STOCKS_ACTIONS_COLUMN,     Qt::Horizontal, Qt::DecorationRole),        QVariant());
    ASSERT_EQ(model->headerData(0,                         Qt::Horizontal, Qt::WhatsThisPropertyRole), QVariant());
    // clang-format on
}

TEST_F(Test_StocksTableModel, Test_data)
{
    const InSequence seq;

    Logo logo;

    QList<Stock*> stocks;

    Stock stock1;
    Stock stock2;
    Stock stock3;

    stock1.meta.instrumentId                = "aaaaa";
    stock1.meta.instrumentLogo              = nullptr;
    stock1.meta.instrumentTicker            = "ABBA";
    stock1.meta.instrumentName              = "Abstract Basics";
    stock1.meta.forQualInvestorFlag         = false;
    stock1.meta.lot                         = 1;
    stock1.meta.pricePrecision              = 2;
    stock1.meta.minPriceIncrement           = 0.01f;
    stock1.meta.ignore                      = false;
    stock1.meta.lastTradeTime               = QTime(18, 39);
    stock1.meta.turnover                    = 1120;
    stock1.meta.dividends.createTimestamp   = 1735678800000;
    stock1.meta.dividends.paymentTimestamp  = 1738357200000;
    stock1.meta.dividends.price             = 100.0f;
    stock1.meta.dividends.yield             = 10.0f;
    stock1.meta.shorts.enabled              = true;
    stock1.meta.shorts.lastEnabledTimestamp = 1735678800000;
    stock1.operational.dayStartPrice        = 0.0f;
    stock1.operational.specifiedDatePrice   = 0.0f;
    stock1.operational.payback              = 13.0f;

    StockOperationalData data1;
    data1.price = 100.0f;
    stock1.operational.detailedData << data1;

    stock2.meta.instrumentId                = "bbbbb";
    stock2.meta.instrumentLogo              = nullptr;
    stock2.meta.instrumentTicker            = "BASE";
    stock2.meta.instrumentName              = "Basketball enhancement";
    stock2.meta.forQualInvestorFlag         = true;
    stock2.meta.lot                         = 1;
    stock2.meta.pricePrecision              = 3;
    stock2.meta.minPriceIncrement           = 0.001f;
    stock2.meta.ignore                      = false;
    stock2.meta.lastTradeTime               = QTime(23, 49);
    stock2.meta.turnover                    = 2340000;
    stock2.meta.dividends.createTimestamp   = 1738357200000;
    stock2.meta.dividends.paymentTimestamp  = 1740776400000;
    stock2.meta.dividends.price             = 150.0f;
    stock2.meta.dividends.yield             = 15.0f;
    stock2.meta.shorts.enabled              = true;
    stock2.meta.shorts.lastEnabledTimestamp = 1738357200000;
    stock2.operational.dayStartPrice        = 201.0f;
    stock2.operational.specifiedDatePrice   = 202.0f;
    stock2.operational.payback              = 33.0f;

    StockOperationalData data2;
    data2.price = 250.0f;
    stock2.operational.detailedData << data2;

    stock3.meta.instrumentId                = "ccccc";
    stock3.meta.instrumentLogo              = &logo;
    stock3.meta.instrumentTicker            = "CODE";
    stock3.meta.instrumentName              = "Code base";
    stock3.meta.forQualInvestorFlag         = false;
    stock3.meta.lot                         = 1;
    stock3.meta.pricePrecision              = 4;
    stock3.meta.minPriceIncrement           = 0.0001f;
    stock3.meta.ignore                      = false;
    stock3.meta.lastTradeTime               = QTime(9, 59);
    stock3.meta.turnover                    = 5560000000;
    stock3.meta.dividends.createTimestamp   = 0;
    stock3.meta.dividends.paymentTimestamp  = 0;
    stock3.meta.dividends.price             = 0.0f;
    stock3.meta.dividends.yield             = 0.0f;
    stock3.meta.shorts.enabled              = false;
    stock3.meta.shorts.lastEnabledTimestamp = 0;
    stock3.operational.dayStartPrice        = 501.0f;
    stock3.operational.specifiedDatePrice   = 502.0f;
    stock3.operational.payback              = 83.0f;

    StockOperationalData data3;
    data3.price = 500.0f;
    stock3.operational.detailedData << data3;

    stocks << &stock1 << &stock2 << &stock3;

    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, isQualified()).WillOnce(Return(false));
    EXPECT_CALL(*userStorageMock, readUnlock());

    model->updateTable(stocks);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole),           QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole),           QVariant("100.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole),           QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole),           QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole),           QVariant("1.12K \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole),           QVariant("13.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole),           QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole),           QVariant("V"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole),           QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole),           QVariant("250.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole),           QVariant("+24.38%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole),           QVariant("+23.76%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole),           QVariant("2.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole),           QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole),           QVariant("15.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole),           QVariant("V"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole),           QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::DisplayRole),           QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::DisplayRole),           QVariant("500.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole),           QVariant("-0.20%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole),           QVariant("-0.40%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole),           QVariant("5.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole),           QVariant("83.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole),           QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole),           QVariant(""));
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::ForegroundRole),        QVariant(QBrush(QColor("#97AEC4"))));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::ForegroundRole),        QVariant(QBrush(QColor("#97AEC4"))));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::ForegroundRole),        QVariant(QBrush(QColor("#ED6F7E"))));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::ForegroundRole),        QVariant(QBrush(QColor("#ED6F7E"))));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::ForegroundRole),        QVariant(QBrush(QColor("#2BD793"))));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::ForegroundRole),        QVariant(QBrush(QColor("#2BD793"))));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::ForegroundRole),        QVariant(QBrush(QColor("#2BD793"))));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::ForegroundRole),        QVariant(QBrush(QColor("#97AEC4"))));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::ForegroundRole),        QVariant(QBrush(QColor("#97AEC4"))));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::ForegroundRole),        QVariant(QBrush(QColor("#2BD793"))));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::ForegroundRole),        QVariant(QBrush(QColor("#ED6F7E"))));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::ForegroundRole),        QVariant(QBrush(QColor("#ED6F7E"))));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::ForegroundRole),        QVariant(QBrush(QColor("#2BD793"))));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::ForegroundRole),        QVariant(QBrush(QColor("#2BD793"))));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DIVIDENDS_COLUMN),   Qt::ForegroundRole),        QVariant(QBrush(QColor("#97AEC4"))));
    ASSERT_EQ(model->data(model->index(2, STOCKS_SHORTS_COLUMN),      Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN),     Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::ToolTipRole),           QVariant("Detection time: 2025-01-01 00:00:00\nPayment date: 2025-02-01\nPrice at detection time: 100.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::ToolTipRole),           QVariant("Last enabled time: 2025-01-01 00:00:00"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::ToolTipRole),           QVariant("From: 201.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::ToolTipRole),           QVariant("From: 202.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::ToolTipRole),           QVariant("Detection time: 2025-02-01 00:00:00\nPayment date: 2025-03-01\nPrice at detection time: 150.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::ToolTipRole),           QVariant("Last enabled time: 2025-02-01 00:00:00"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::ToolTipRole),           QVariant("From: 501.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::ToolTipRole),           QVariant("From: 502.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_DIVIDENDS_COLUMN),   Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_SHORTS_COLUMN),      Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN),     Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::TextAlignmentRole),     QVariant());
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::TextAlignmentRole),     QVariant());
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::TextAlignmentRole),     QVariant());
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::TextAlignmentRole),     QVariant());
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::TextAlignmentRole),     QVariant());
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::TextAlignmentRole),     QVariant());
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::TextAlignmentRole),     QVariant());
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::TextAlignmentRole),     QVariant(Qt::AlignCenter));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::TextAlignmentRole),     QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::TextAlignmentRole),     QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::TextAlignmentRole),     QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::TextAlignmentRole),     QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::TextAlignmentRole),     QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::TextAlignmentRole),     QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::TextAlignmentRole),     QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::TextAlignmentRole),     QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::TextAlignmentRole),     QVariant(Qt::AlignCenter));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::TextAlignmentRole),     QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::TextAlignmentRole),     QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::TextAlignmentRole),     QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::TextAlignmentRole),     QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::TextAlignmentRole),     QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::TextAlignmentRole),     QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::TextAlignmentRole),     QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_DIVIDENDS_COLUMN),   Qt::TextAlignmentRole),     QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_SHORTS_COLUMN),      Qt::TextAlignmentRole),     QVariant(Qt::AlignCenter));
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN),     Qt::TextAlignmentRole),     QVariant());
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        ROLE_INSTRUMENT_LOGO),      QVariant(0));
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        ROLE_INSTRUMENT_LOGO),      QVariant(0));
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        ROLE_INSTRUMENT_LOGO),      QVariant(reinterpret_cast<qint64>(&logo)));
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        ROLE_INSTRUMENT_NAME),      QVariant("Abstract Basics"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        ROLE_INSTRUMENT_NAME),      QVariant("Basketball enhancement"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        ROLE_INSTRUMENT_NAME),      QVariant("Code base"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        ROLE_INSTRUMENT_LOCKED),    QVariant(false));
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        ROLE_INSTRUMENT_LOCKED),    QVariant(true));
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        ROLE_INSTRUMENT_LOCKED),    QVariant(false));
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        ROLE_STOCK),                QVariant(reinterpret_cast<qint64>(&stock1)));
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        ROLE_STOCK),                QVariant(reinterpret_cast<qint64>(&stock2)));
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        ROLE_STOCK),                QVariant(reinterpret_cast<qint64>(&stock3)));
    ASSERT_EQ(model->data(model->index(0, 0),                         Qt::WhatsThisPropertyRole), QVariant());
    // clang-format on
}

TEST_F(Test_StocksTableModel, Test_sort)
{
    const InSequence seq;

    Logo logo;

    QList<Stock*> stocks;

    Stock stock1;
    Stock stock2;
    Stock stock3;

    stock1.meta.instrumentId                = "aaaaa";
    stock1.meta.instrumentLogo              = nullptr;
    stock1.meta.instrumentTicker            = "ABBA";
    stock1.meta.instrumentName              = "Abstract Basics";
    stock1.meta.forQualInvestorFlag         = false;
    stock1.meta.lot                         = 1;
    stock1.meta.pricePrecision              = 2;
    stock1.meta.minPriceIncrement           = 0.01f;
    stock1.meta.ignore                      = false;
    stock1.meta.lastTradeTime               = QTime(18, 39);
    stock1.meta.turnover                    = 1120;
    stock1.meta.dividends.createTimestamp   = 1735678800000;
    stock1.meta.dividends.paymentTimestamp  = 1738357200000;
    stock1.meta.dividends.price             = 100.0f;
    stock1.meta.dividends.yield             = 10.0f;
    stock1.meta.shorts.enabled              = true;
    stock1.meta.shorts.lastEnabledTimestamp = 1735678800000;
    stock1.operational.dayStartPrice        = 0.0f;
    stock1.operational.specifiedDatePrice   = 0.0f;
    stock1.operational.payback              = 13.0f;

    StockOperationalData data1;
    data1.price = 100.0f;
    stock1.operational.detailedData << data1;

    stock2.meta.instrumentId                = "bbbbb";
    stock2.meta.instrumentLogo              = nullptr;
    stock2.meta.instrumentTicker            = "BASE";
    stock2.meta.instrumentName              = "Basketball enhancement";
    stock2.meta.forQualInvestorFlag         = true;
    stock2.meta.lot                         = 1;
    stock2.meta.pricePrecision              = 3;
    stock2.meta.minPriceIncrement           = 0.001f;
    stock2.meta.ignore                      = false;
    stock2.meta.lastTradeTime               = QTime(23, 49);
    stock2.meta.turnover                    = 2340000;
    stock2.meta.dividends.createTimestamp   = 1738357200000;
    stock2.meta.dividends.paymentTimestamp  = 1740776400000;
    stock2.meta.dividends.price             = 150.0f;
    stock2.meta.dividends.yield             = 15.0f;
    stock2.meta.shorts.enabled              = true;
    stock2.meta.shorts.lastEnabledTimestamp = 1738357200000;
    stock2.operational.dayStartPrice        = 201.0f;
    stock2.operational.specifiedDatePrice   = 202.0f;
    stock2.operational.payback              = 33.0f;

    StockOperationalData data2;
    data2.price = 250.0f;
    stock2.operational.detailedData << data2;

    stock3.meta.instrumentId                = "ccccc";
    stock3.meta.instrumentLogo              = &logo;
    stock3.meta.instrumentTicker            = "CODE";
    stock3.meta.instrumentName              = "Code base";
    stock3.meta.forQualInvestorFlag         = false;
    stock3.meta.lot                         = 1;
    stock3.meta.pricePrecision              = 4;
    stock3.meta.minPriceIncrement           = 0.0001f;
    stock3.meta.ignore                      = false;
    stock3.meta.lastTradeTime               = QTime(9, 59);
    stock3.meta.turnover                    = 5560000000;
    stock3.meta.dividends.createTimestamp   = 0;
    stock3.meta.dividends.paymentTimestamp  = 0;
    stock3.meta.dividends.price             = 0.0f;
    stock3.meta.dividends.yield             = 0.0f;
    stock3.meta.shorts.enabled              = false;
    stock3.meta.shorts.lastEnabledTimestamp = 0;
    stock3.operational.dayStartPrice        = 501.0f;
    stock3.operational.specifiedDatePrice   = 502.0f;
    stock3.operational.payback              = 83.0f;

    StockOperationalData data3;
    data3.price = 500.0f;
    stock3.operational.detailedData << data3;

    stocks << &stock1 << &stock2 << &stock3;

    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, isQualified()).WillOnce(Return(false));
    EXPECT_CALL(*userStorageMock, readUnlock());

    model->updateTable(stocks);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("100.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("1.12K \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("13.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("250.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+24.38%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+23.76%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("2.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("15.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("500.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-0.20%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-0.40%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("5.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("83.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on

    model->sort(STOCKS_NAME_COLUMN, Qt::AscendingOrder);

    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN), Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN), Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN), Qt::DisplayRole), QVariant("CODE"));

    model->sort(STOCKS_NAME_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN), Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN), Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN), Qt::DisplayRole), QVariant("ABBA"));

    model->sort(STOCKS_PRICE_COLUMN, Qt::AscendingOrder);

    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN), Qt::DisplayRole), QVariant("100.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN), Qt::DisplayRole), QVariant("250.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN), Qt::DisplayRole), QVariant("500.0000 \u20BD"));

    model->sort(STOCKS_PRICE_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN), Qt::DisplayRole), QVariant("500.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN), Qt::DisplayRole), QVariant("250.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN), Qt::DisplayRole), QVariant("100.00 \u20BD"));

    model->sort(STOCKS_DAY_CHANGE_COLUMN, Qt::AscendingOrder);

    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-0.20%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN), Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+24.38%"));

    model->sort(STOCKS_DAY_CHANGE_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+24.38%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN), Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-0.20%"));

    model->sort(STOCKS_DATE_CHANGE_COLUMN, Qt::AscendingOrder);

    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-0.40%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+23.76%"));

    model->sort(STOCKS_DATE_CHANGE_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+23.76%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-0.40%"));

    model->sort(STOCKS_TURNOVER_COLUMN, Qt::AscendingOrder);

    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN), Qt::DisplayRole), QVariant("1.12K \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN), Qt::DisplayRole), QVariant("2.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN), Qt::DisplayRole), QVariant("5.56B \u20BD"));

    model->sort(STOCKS_TURNOVER_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN), Qt::DisplayRole), QVariant("5.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN), Qt::DisplayRole), QVariant("2.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN), Qt::DisplayRole), QVariant("1.12K \u20BD"));

    model->sort(STOCKS_PAYBACK_COLUMN, Qt::AscendingOrder);

    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN), Qt::DisplayRole), QVariant("13.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN), Qt::DisplayRole), QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN), Qt::DisplayRole), QVariant("83.00%"));

    model->sort(STOCKS_PAYBACK_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN), Qt::DisplayRole), QVariant("83.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN), Qt::DisplayRole), QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN), Qt::DisplayRole), QVariant("13.00%"));

    model->sort(STOCKS_DIVIDENDS_COLUMN, Qt::AscendingOrder);

    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN), Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN), Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DIVIDENDS_COLUMN), Qt::DisplayRole), QVariant("15.00%"));

    model->sort(STOCKS_DIVIDENDS_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN), Qt::DisplayRole), QVariant("15.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN), Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DIVIDENDS_COLUMN), Qt::DisplayRole), QVariant("0.00%"));

    model->sort(STOCKS_SHORTS_COLUMN, Qt::AscendingOrder);

    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN), Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN), Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_SHORTS_COLUMN), Qt::DisplayRole), QVariant("V"));

    model->sort(STOCKS_SHORTS_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN), Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN), Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_SHORTS_COLUMN), Qt::DisplayRole), QVariant(""));

    model->sort(STOCKS_ACTIONS_COLUMN, Qt::AscendingOrder);

    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN), Qt::DisplayRole), QVariant());

    model->sort(STOCKS_ACTIONS_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN), Qt::DisplayRole), QVariant());

    model->sort(STOCKS_NAME_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN), Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN), Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN), Qt::DisplayRole), QVariant("ABBA"));

    model->sort(STOCKS_PRICE_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN), Qt::DisplayRole), QVariant("500.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN), Qt::DisplayRole), QVariant("250.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN), Qt::DisplayRole), QVariant("100.00 \u20BD"));

    model->sort(STOCKS_DAY_CHANGE_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+24.38%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN), Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-0.20%"));

    model->sort(STOCKS_DATE_CHANGE_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+23.76%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-0.40%"));

    model->sort(STOCKS_TURNOVER_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN), Qt::DisplayRole), QVariant("5.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN), Qt::DisplayRole), QVariant("2.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN), Qt::DisplayRole), QVariant("1.12K \u20BD"));

    model->sort(STOCKS_PAYBACK_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN), Qt::DisplayRole), QVariant("83.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN), Qt::DisplayRole), QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN), Qt::DisplayRole), QVariant("13.00%"));

    model->sort(STOCKS_DIVIDENDS_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN), Qt::DisplayRole), QVariant("15.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN), Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DIVIDENDS_COLUMN), Qt::DisplayRole), QVariant("0.00%"));

    model->sort(STOCKS_SHORTS_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN), Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN), Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_SHORTS_COLUMN), Qt::DisplayRole), QVariant(""));
}

TEST_F(Test_StocksTableModel, Test_setFilter)
{
    const InSequence seq;

    Logo logo;

    QList<Stock*> stocks;

    Stock stock1;
    Stock stock2;
    Stock stock3;

    stock1.meta.instrumentId                = "aaaaa";
    stock1.meta.instrumentLogo              = nullptr;
    stock1.meta.instrumentTicker            = "ABBA";
    stock1.meta.instrumentName              = "Abstract Basics";
    stock1.meta.forQualInvestorFlag         = false;
    stock1.meta.lot                         = 1;
    stock1.meta.pricePrecision              = 2;
    stock1.meta.minPriceIncrement           = 0.01f;
    stock1.meta.ignore                      = false;
    stock1.meta.lastTradeTime               = QTime(18, 39);
    stock1.meta.turnover                    = 1120;
    stock1.meta.dividends.createTimestamp   = 1735678800000;
    stock1.meta.dividends.paymentTimestamp  = 1738357200000;
    stock1.meta.dividends.price             = 100.0f;
    stock1.meta.dividends.yield             = 10.0f;
    stock1.meta.shorts.enabled              = true;
    stock1.meta.shorts.lastEnabledTimestamp = 1735678800000;
    stock1.operational.dayStartPrice        = 0.0f;
    stock1.operational.specifiedDatePrice   = 0.0f;
    stock1.operational.payback              = 13.0f;

    StockOperationalData data1;
    data1.price = 100.0f;
    stock1.operational.detailedData << data1;

    stock2.meta.instrumentId                = "bbbbb";
    stock2.meta.instrumentLogo              = nullptr;
    stock2.meta.instrumentTicker            = "BASE";
    stock2.meta.instrumentName              = "Basketball enhancement";
    stock2.meta.forQualInvestorFlag         = true;
    stock2.meta.lot                         = 1;
    stock2.meta.pricePrecision              = 3;
    stock2.meta.minPriceIncrement           = 0.001f;
    stock2.meta.ignore                      = false;
    stock2.meta.lastTradeTime               = QTime(23, 49);
    stock2.meta.turnover                    = 2340000;
    stock2.meta.dividends.createTimestamp   = 1738357200000;
    stock2.meta.dividends.paymentTimestamp  = 1740776400000;
    stock2.meta.dividends.price             = 150.0f;
    stock2.meta.dividends.yield             = 15.0f;
    stock2.meta.shorts.enabled              = true;
    stock2.meta.shorts.lastEnabledTimestamp = 1738357200000;
    stock2.operational.dayStartPrice        = 201.0f;
    stock2.operational.specifiedDatePrice   = 202.0f;
    stock2.operational.payback              = 33.0f;

    StockOperationalData data2;
    data2.price = 250.0f;
    stock2.operational.detailedData << data2;

    stock3.meta.instrumentId                = "ccccc";
    stock3.meta.instrumentLogo              = &logo;
    stock3.meta.instrumentTicker            = "CODE";
    stock3.meta.instrumentName              = "Code base";
    stock3.meta.forQualInvestorFlag         = false;
    stock3.meta.lot                         = 1;
    stock3.meta.pricePrecision              = 4;
    stock3.meta.minPriceIncrement           = 0.0001f;
    stock3.meta.ignore                      = false;
    stock3.meta.lastTradeTime               = QTime(9, 59);
    stock3.meta.turnover                    = 5560000000;
    stock3.meta.dividends.createTimestamp   = 0;
    stock3.meta.dividends.paymentTimestamp  = 0;
    stock3.meta.dividends.price             = 0.0f;
    stock3.meta.dividends.yield             = 0.0f;
    stock3.meta.shorts.enabled              = false;
    stock3.meta.shorts.lastEnabledTimestamp = 0;
    stock3.operational.dayStartPrice        = 501.0f;
    stock3.operational.specifiedDatePrice   = 502.0f;
    stock3.operational.payback              = 83.0f;

    StockOperationalData data3;
    data3.price = 500.0f;
    stock3.operational.detailedData << data3;

    stocks << &stock1 << &stock2 << &stock3;

    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, isQualified()).WillOnce(Return(false));
    EXPECT_CALL(*userStorageMock, readUnlock());

    model->updateTable(stocks);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("100.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("1.12K \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("13.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("250.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+24.38%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+23.76%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("2.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("15.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("500.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-0.20%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-0.40%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("5.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("83.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on

    StockFilter filter;
    filter.useTicker = true;
    filter.ticker    = "CODE";

    model->setFilter(filter);
    ASSERT_EQ(model->rowCount(), 1);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("500.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-0.20%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-0.40%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("5.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("83.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on
}

TEST_F(Test_StocksTableModel, Test_updateAll)
{
    const InSequence seq;

    Logo logo;

    QList<Stock*> stocks;

    Stock stock1;
    Stock stock2;
    Stock stock3;

    stock1.meta.instrumentId                = "aaaaa";
    stock1.meta.instrumentLogo              = nullptr;
    stock1.meta.instrumentTicker            = "ABBA";
    stock1.meta.instrumentName              = "Abstract Basics";
    stock1.meta.forQualInvestorFlag         = false;
    stock1.meta.lot                         = 1;
    stock1.meta.pricePrecision              = 2;
    stock1.meta.minPriceIncrement           = 0.01f;
    stock1.meta.ignore                      = false;
    stock1.meta.lastTradeTime               = QTime(18, 39);
    stock1.meta.turnover                    = 1120;
    stock1.meta.dividends.createTimestamp   = 1735678800000;
    stock1.meta.dividends.paymentTimestamp  = 1738357200000;
    stock1.meta.dividends.price             = 100.0f;
    stock1.meta.dividends.yield             = 10.0f;
    stock1.meta.shorts.enabled              = true;
    stock1.meta.shorts.lastEnabledTimestamp = 1735678800000;
    stock1.operational.dayStartPrice        = 0.0f;
    stock1.operational.specifiedDatePrice   = 0.0f;
    stock1.operational.payback              = 13.0f;

    StockOperationalData data1;
    data1.price = 100.0f;
    stock1.operational.detailedData << data1;

    stock2.meta.instrumentId                = "bbbbb";
    stock2.meta.instrumentLogo              = nullptr;
    stock2.meta.instrumentTicker            = "BASE";
    stock2.meta.instrumentName              = "Basketball enhancement";
    stock2.meta.forQualInvestorFlag         = true;
    stock2.meta.lot                         = 1;
    stock2.meta.pricePrecision              = 3;
    stock2.meta.minPriceIncrement           = 0.001f;
    stock2.meta.ignore                      = false;
    stock2.meta.lastTradeTime               = QTime(23, 49);
    stock2.meta.turnover                    = 2340000;
    stock2.meta.dividends.createTimestamp   = 1738357200000;
    stock2.meta.dividends.paymentTimestamp  = 1740776400000;
    stock2.meta.dividends.price             = 150.0f;
    stock2.meta.dividends.yield             = 15.0f;
    stock2.meta.shorts.enabled              = true;
    stock2.meta.shorts.lastEnabledTimestamp = 1738357200000;
    stock2.operational.dayStartPrice        = 201.0f;
    stock2.operational.specifiedDatePrice   = 202.0f;
    stock2.operational.payback              = 33.0f;

    StockOperationalData data2;
    data2.price = 250.0f;
    stock2.operational.detailedData << data2;

    stock3.meta.instrumentId                = "ccccc";
    stock3.meta.instrumentLogo              = &logo;
    stock3.meta.instrumentTicker            = "CODE";
    stock3.meta.instrumentName              = "Code base";
    stock3.meta.forQualInvestorFlag         = false;
    stock3.meta.lot                         = 1;
    stock3.meta.pricePrecision              = 4;
    stock3.meta.minPriceIncrement           = 0.0001f;
    stock3.meta.ignore                      = false;
    stock3.meta.lastTradeTime               = QTime(9, 59);
    stock3.meta.turnover                    = 5560000000;
    stock3.meta.dividends.createTimestamp   = 0;
    stock3.meta.dividends.paymentTimestamp  = 0;
    stock3.meta.dividends.price             = 0.0f;
    stock3.meta.dividends.yield             = 0.0f;
    stock3.meta.shorts.enabled              = false;
    stock3.meta.shorts.lastEnabledTimestamp = 0;
    stock3.operational.dayStartPrice        = 501.0f;
    stock3.operational.specifiedDatePrice   = 502.0f;
    stock3.operational.payback              = 83.0f;

    StockOperationalData data3;
    data3.price = 500.0f;
    stock3.operational.detailedData << data3;

    stocks << &stock1 << &stock2 << &stock3;

    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, isQualified()).WillOnce(Return(false));
    EXPECT_CALL(*userStorageMock, readUnlock());

    model->updateTable(stocks);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("100.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("1.12K \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("13.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("250.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+24.38%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+23.76%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("2.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("15.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("500.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-0.20%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-0.40%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("5.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("83.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on

    model->sort(STOCKS_ACTIONS_COLUMN, Qt::AscendingOrder);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("100.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("1.12K \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("13.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("250.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+24.38%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+23.76%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("2.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("15.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("500.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-0.20%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-0.40%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("5.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("83.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on

    stock1.meta.instrumentTicker             = "DISY";
    stock1.meta.instrumentName               = "Disintegration yoyo";
    stock1.meta.forQualInvestorFlag          = false;
    stock1.meta.lot                          = 1;
    stock1.meta.pricePrecision               = 4;
    stock1.meta.minPriceIncrement            = 0.01f;
    stock1.meta.ignore                       = false;
    stock1.meta.lastTradeTime                = QTime(18, 39);
    stock1.meta.turnover                     = 2120;
    stock1.meta.dividends.createTimestamp    = 1735678800000;
    stock1.meta.dividends.paymentTimestamp   = 1738357200000;
    stock1.meta.dividends.price              = 300.0f;
    stock1.meta.dividends.yield              = 30.0f;
    stock1.meta.shorts.enabled               = false;
    stock1.meta.shorts.lastEnabledTimestamp  = 1735678800000;
    stock1.operational.detailedData[0].price = 300.0f;
    stock1.operational.dayStartPrice         = 150.0f;
    stock1.operational.specifiedDatePrice    = 170.0f;
    stock1.operational.payback               = 17.0f;

    stock2.meta.instrumentTicker             = "EASY";
    stock2.meta.instrumentName               = "Easy peasy";
    stock2.meta.forQualInvestorFlag          = true;
    stock2.meta.lot                          = 1;
    stock2.meta.pricePrecision               = 2;
    stock2.meta.minPriceIncrement            = 0.001f;
    stock2.meta.ignore                       = false;
    stock2.meta.lastTradeTime                = QTime(23, 49);
    stock2.meta.turnover                     = 4340000;
    stock2.meta.dividends.createTimestamp    = 1738357200000;
    stock2.meta.dividends.paymentTimestamp   = 1740776400000;
    stock2.meta.dividends.price              = 450.0f;
    stock2.meta.dividends.yield              = 45.0f;
    stock2.meta.shorts.enabled               = false;
    stock2.meta.shorts.lastEnabledTimestamp  = 1738357200000;
    stock2.operational.detailedData[0].price = 800.0f;
    stock2.operational.dayStartPrice         = 51.0f;
    stock2.operational.specifiedDatePrice    = 22.0f;
    stock2.operational.payback               = 37.0f;

    stock3.meta.instrumentTicker             = "FUCK";
    stock3.meta.instrumentName               = "Functional clock";
    stock3.meta.forQualInvestorFlag          = false;
    stock3.meta.lot                          = 1;
    stock3.meta.pricePrecision               = 3;
    stock3.meta.minPriceIncrement            = 0.0001f;
    stock3.meta.ignore                       = false;
    stock3.meta.lastTradeTime                = QTime(9, 59);
    stock3.meta.turnover                     = 6560000000;
    stock3.meta.dividends.createTimestamp    = 0;
    stock3.meta.dividends.paymentTimestamp   = 0;
    stock3.meta.dividends.price              = 0.0f;
    stock3.meta.dividends.yield              = 0.0f;
    stock3.meta.shorts.enabled               = true;
    stock3.meta.shorts.lastEnabledTimestamp  = 1740776400000;
    stock3.operational.detailedData[0].price = 700.0f;
    stock3.operational.dayStartPrice         = 231.0f;
    stock3.operational.specifiedDatePrice    = 192.0f;
    stock3.operational.payback               = 87.0f;

    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, isQualified()).WillOnce(Return(false));
    EXPECT_CALL(*userStorageMock, readUnlock());

    model->updateAll();
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("DISY"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("300.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+100.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+76.47%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("2.12K \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("17.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("30.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("EASY"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("800.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+1468.63%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+3536.36%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("4.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("37.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("45.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("FUCK"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("700.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+203.03%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+264.58%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("6.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("87.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on

    model->sort(STOCKS_NAME_COLUMN, Qt::DescendingOrder);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("FUCK"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("700.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+203.03%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+264.58%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("6.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("87.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("EASY"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("800.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+1468.63%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+3536.36%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("4.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("37.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("45.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("DISY"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("300.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+100.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+76.47%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("2.12K \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("17.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("30.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on

    stock1.meta.instrumentTicker             = "GORA";
    stock1.meta.instrumentName               = "Go Randy";
    stock1.meta.forQualInvestorFlag          = false;
    stock1.meta.lot                          = 1;
    stock1.meta.pricePrecision               = 3;
    stock1.meta.minPriceIncrement            = 0.01f;
    stock1.meta.ignore                       = false;
    stock1.meta.lastTradeTime                = QTime(18, 39);
    stock1.meta.turnover                     = 3120;
    stock1.meta.dividends.createTimestamp    = 0;
    stock1.meta.dividends.paymentTimestamp   = 0;
    stock1.meta.dividends.price              = 0.0f;
    stock1.meta.dividends.yield              = 0.0f;
    stock1.meta.shorts.enabled               = true;
    stock1.meta.shorts.lastEnabledTimestamp  = 1735678800000;
    stock1.operational.detailedData[0].price = 290.0f;
    stock1.operational.dayStartPrice         = 640.0f;
    stock1.operational.specifiedDatePrice    = 170.0f;
    stock1.operational.payback               = 27.0f;

    stock2.meta.instrumentTicker             = "HIVE";
    stock2.meta.instrumentName               = "Bzzzzz hive";
    stock2.meta.forQualInvestorFlag          = true;
    stock2.meta.lot                          = 1;
    stock2.meta.pricePrecision               = 4;
    stock2.meta.minPriceIncrement            = 0.001f;
    stock2.meta.ignore                       = false;
    stock2.meta.lastTradeTime                = QTime(23, 49);
    stock2.meta.turnover                     = 5340000;
    stock2.meta.dividends.createTimestamp    = 1738357200000;
    stock2.meta.dividends.paymentTimestamp   = 1740776400000;
    stock2.meta.dividends.price              = 750.0f;
    stock2.meta.dividends.yield              = 75.0f;
    stock2.meta.shorts.enabled               = false;
    stock2.meta.shorts.lastEnabledTimestamp  = 1738357200000;
    stock2.operational.detailedData[0].price = 100.0f;
    stock2.operational.dayStartPrice         = 131.0f;
    stock2.operational.specifiedDatePrice    = 922.0f;
    stock2.operational.payback               = 47.0f;

    stock3.meta.instrumentTicker             = "IGOR";
    stock3.meta.instrumentName               = "I go ready";
    stock3.meta.forQualInvestorFlag          = false;
    stock3.meta.lot                          = 1;
    stock3.meta.pricePrecision               = 2;
    stock3.meta.minPriceIncrement            = 0.0001f;
    stock3.meta.ignore                       = false;
    stock3.meta.lastTradeTime                = QTime(9, 59);
    stock3.meta.turnover                     = 7560000000;
    stock3.meta.dividends.createTimestamp    = 0;
    stock3.meta.dividends.paymentTimestamp   = 0;
    stock3.meta.dividends.price              = 0.0f;
    stock3.meta.dividends.yield              = 0.0f;
    stock3.meta.shorts.enabled               = true;
    stock3.meta.shorts.lastEnabledTimestamp  = 1740776400000;
    stock3.operational.detailedData[0].price = 537.0f;
    stock3.operational.dayStartPrice         = 197.0f;
    stock3.operational.specifiedDatePrice    = 325.0f;
    stock3.operational.payback               = 97.0f;

    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, isQualified()).WillOnce(Return(false));
    EXPECT_CALL(*userStorageMock, readUnlock());

    model->updateAll();
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("IGOR"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("537.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+172.59%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+65.23%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("7.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("97.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("HIVE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("100.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-23.66%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-89.15%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("5.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("47.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("75.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("GORA"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("290.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-54.69%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+70.59%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("3.12K \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("27.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on

    StockFilter filter;
    filter.useTurnover  = true;
    filter.turnoverFrom = 1000000;

    model->setFilter(filter);
    ASSERT_EQ(model->rowCount(), 2);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("IGOR"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("537.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+172.59%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+65.23%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("7.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("97.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("HIVE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("100.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-23.66%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-89.15%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("5.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("47.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("75.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on

    stock1.meta.instrumentTicker             = "JOJO";
    stock1.meta.instrumentName               = "John Johnson";
    stock1.meta.forQualInvestorFlag          = false;
    stock1.meta.lot                          = 1;
    stock1.meta.pricePrecision               = 2;
    stock1.meta.minPriceIncrement            = 0.01f;
    stock1.meta.ignore                       = false;
    stock1.meta.lastTradeTime                = QTime(18, 39);
    stock1.meta.turnover                     = 9310000;
    stock1.meta.dividends.createTimestamp    = 1735678800000;
    stock1.meta.dividends.paymentTimestamp   = 1738357200000;
    stock1.meta.dividends.price              = 330.0f;
    stock1.meta.dividends.yield              = 33.0f;
    stock1.meta.shorts.enabled               = false;
    stock1.meta.shorts.lastEnabledTimestamp  = 1735678800000;
    stock1.operational.detailedData[0].price = 1111.0f;
    stock1.operational.dayStartPrice         = 888.0f;
    stock1.operational.specifiedDatePrice    = 999.0f;
    stock1.operational.payback               = 87.0f;

    stock2.meta.instrumentTicker             = "KOND";
    stock2.meta.instrumentName               = "King of the Northern Dome";
    stock2.meta.forQualInvestorFlag          = true;
    stock2.meta.lot                          = 1;
    stock2.meta.pricePrecision               = 4;
    stock2.meta.minPriceIncrement            = 0.001f;
    stock2.meta.ignore                       = false;
    stock2.meta.lastTradeTime                = QTime(23, 49);
    stock2.meta.turnover                     = 7340000;
    stock2.meta.dividends.createTimestamp    = 1738357200000;
    stock2.meta.dividends.paymentTimestamp   = 1740776400000;
    stock2.meta.dividends.price              = 770.0f;
    stock2.meta.dividends.yield              = 77.0f;
    stock2.meta.shorts.enabled               = true;
    stock2.meta.shorts.lastEnabledTimestamp  = 1738357200000;
    stock2.operational.detailedData[0].price = 2222.0f;
    stock2.operational.dayStartPrice         = 444.0f;
    stock2.operational.specifiedDatePrice    = 555.0f;
    stock2.operational.payback               = 57.0f;

    stock3.meta.instrumentTicker             = "LEMO";
    stock3.meta.instrumentName               = "Lemonade";
    stock3.meta.forQualInvestorFlag          = false;
    stock3.meta.lot                          = 1;
    stock3.meta.pricePrecision               = 3;
    stock3.meta.minPriceIncrement            = 0.0001f;
    stock3.meta.ignore                       = false;
    stock3.meta.lastTradeTime                = QTime(9, 59);
    stock3.meta.turnover                     = 9560;
    stock3.meta.dividends.createTimestamp    = 1740776400000;
    stock3.meta.dividends.paymentTimestamp   = 1743454800000;
    stock3.meta.dividends.price              = 50.0f;
    stock3.meta.dividends.yield              = 5.0f;
    stock3.meta.shorts.enabled               = false;
    stock3.meta.shorts.lastEnabledTimestamp  = 1740776400000;
    stock3.operational.detailedData[0].price = 3333.0f;
    stock3.operational.dayStartPrice         = 666.0f;
    stock3.operational.specifiedDatePrice    = 777.0f;
    stock3.operational.payback               = 37.0f;

    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, isQualified()).WillOnce(Return(false));
    EXPECT_CALL(*userStorageMock, readUnlock());

    model->updateAll();
    ASSERT_EQ(model->rowCount(), 2);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("KOND"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("2222.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+400.45%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+300.36%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("7.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("57.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("77.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("JOJO"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("1111.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+25.11%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+11.21%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("9.31M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("87.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on
}

TEST_F(Test_StocksTableModel, Test_updateLastPrices)
{
    const InSequence seq;

    Logo logo;

    QList<Stock*> stocks;

    Stock stock1;
    Stock stock2;
    Stock stock3;

    stock1.meta.instrumentId                = "aaaaa";
    stock1.meta.instrumentLogo              = nullptr;
    stock1.meta.instrumentTicker            = "ABBA";
    stock1.meta.instrumentName              = "Abstract Basics";
    stock1.meta.forQualInvestorFlag         = false;
    stock1.meta.lot                         = 1;
    stock1.meta.pricePrecision              = 2;
    stock1.meta.minPriceIncrement           = 0.01f;
    stock1.meta.ignore                      = false;
    stock1.meta.lastTradeTime               = QTime(18, 39);
    stock1.meta.turnover                    = 1120;
    stock1.meta.dividends.createTimestamp   = 1735678800000;
    stock1.meta.dividends.paymentTimestamp  = 1738357200000;
    stock1.meta.dividends.price             = 100.0f;
    stock1.meta.dividends.yield             = 10.0f;
    stock1.meta.shorts.enabled              = true;
    stock1.meta.shorts.lastEnabledTimestamp = 1735678800000;
    stock1.operational.dayStartPrice        = 0.0f;
    stock1.operational.specifiedDatePrice   = 0.0f;
    stock1.operational.payback              = 13.0f;

    StockOperationalData data1;
    data1.price = 100.0f;
    stock1.operational.detailedData << data1;

    stock2.meta.instrumentId                = "bbbbb";
    stock2.meta.instrumentLogo              = nullptr;
    stock2.meta.instrumentTicker            = "BASE";
    stock2.meta.instrumentName              = "Basketball enhancement";
    stock2.meta.forQualInvestorFlag         = true;
    stock2.meta.lot                         = 1;
    stock2.meta.pricePrecision              = 3;
    stock2.meta.minPriceIncrement           = 0.001f;
    stock2.meta.ignore                      = false;
    stock2.meta.lastTradeTime               = QTime(23, 49);
    stock2.meta.turnover                    = 2340000;
    stock2.meta.dividends.createTimestamp   = 1738357200000;
    stock2.meta.dividends.paymentTimestamp  = 1740776400000;
    stock2.meta.dividends.price             = 150.0f;
    stock2.meta.dividends.yield             = 15.0f;
    stock2.meta.shorts.enabled              = true;
    stock2.meta.shorts.lastEnabledTimestamp = 1738357200000;
    stock2.operational.dayStartPrice        = 201.0f;
    stock2.operational.specifiedDatePrice   = 202.0f;
    stock2.operational.payback              = 33.0f;

    StockOperationalData data2;
    data2.price = 250.0f;
    stock2.operational.detailedData << data2;

    stock3.meta.instrumentId                = "ccccc";
    stock3.meta.instrumentLogo              = &logo;
    stock3.meta.instrumentTicker            = "CODE";
    stock3.meta.instrumentName              = "Code base";
    stock3.meta.forQualInvestorFlag         = false;
    stock3.meta.lot                         = 1;
    stock3.meta.pricePrecision              = 4;
    stock3.meta.minPriceIncrement           = 0.0001f;
    stock3.meta.ignore                      = false;
    stock3.meta.lastTradeTime               = QTime(9, 59);
    stock3.meta.turnover                    = 5560000000;
    stock3.meta.dividends.createTimestamp   = 0;
    stock3.meta.dividends.paymentTimestamp  = 0;
    stock3.meta.dividends.price             = 0.0f;
    stock3.meta.dividends.yield             = 0.0f;
    stock3.meta.shorts.enabled              = false;
    stock3.meta.shorts.lastEnabledTimestamp = 0;
    stock3.operational.dayStartPrice        = 501.0f;
    stock3.operational.specifiedDatePrice   = 502.0f;
    stock3.operational.payback              = 83.0f;

    StockOperationalData data3;
    data3.price = 500.0f;
    stock3.operational.detailedData << data3;

    stocks << &stock1 << &stock2 << &stock3;

    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, isQualified()).WillOnce(Return(false));
    EXPECT_CALL(*userStorageMock, readUnlock());

    model->updateTable(stocks);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("100.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("1.12K \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("13.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("250.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+24.38%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+23.76%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("2.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("15.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("500.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-0.20%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-0.40%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("5.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("83.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on

    model->sort(STOCKS_ACTIONS_COLUMN, Qt::AscendingOrder);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("100.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("1.12K \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("13.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("250.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+24.38%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+23.76%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("2.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("15.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("500.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-0.20%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-0.40%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("5.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("83.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on

    stock1.meta.instrumentTicker             = "DISY";
    stock1.meta.instrumentName               = "Disintegration yoyo";
    stock1.meta.forQualInvestorFlag          = false;
    stock1.meta.lot                          = 1;
    stock1.meta.pricePrecision               = 4;
    stock1.meta.minPriceIncrement            = 0.01f;
    stock1.meta.ignore                       = false;
    stock1.meta.lastTradeTime                = QTime(18, 39);
    stock1.meta.turnover                     = 2120;
    stock1.meta.dividends.createTimestamp    = 1735678800000;
    stock1.meta.dividends.paymentTimestamp   = 1738357200000;
    stock1.meta.dividends.price              = 300.0f;
    stock1.meta.dividends.yield              = 30.0f;
    stock1.meta.shorts.enabled               = false;
    stock1.meta.shorts.lastEnabledTimestamp  = 1735678800000;
    stock1.operational.detailedData[0].price = 300.0f;
    stock1.operational.dayStartPrice         = 150.0f;
    stock1.operational.specifiedDatePrice    = 170.0f;
    stock1.operational.payback               = 17.0f;

    stock2.meta.instrumentTicker             = "EASY";
    stock2.meta.instrumentName               = "Easy peasy";
    stock2.meta.forQualInvestorFlag          = true;
    stock2.meta.lot                          = 1;
    stock2.meta.pricePrecision               = 2;
    stock2.meta.minPriceIncrement            = 0.001f;
    stock2.meta.ignore                       = false;
    stock2.meta.lastTradeTime                = QTime(23, 49);
    stock2.meta.turnover                     = 4340000;
    stock2.meta.dividends.createTimestamp    = 1738357200000;
    stock2.meta.dividends.paymentTimestamp   = 1740776400000;
    stock2.meta.dividends.price              = 450.0f;
    stock2.meta.dividends.yield              = 45.0f;
    stock2.meta.shorts.enabled               = false;
    stock2.meta.shorts.lastEnabledTimestamp  = 1738357200000;
    stock2.operational.detailedData[0].price = 800.0f;
    stock2.operational.dayStartPrice         = 51.0f;
    stock2.operational.specifiedDatePrice    = 22.0f;
    stock2.operational.payback               = 37.0f;

    stock3.meta.instrumentTicker             = "FUCK";
    stock3.meta.instrumentName               = "Functional clock";
    stock3.meta.forQualInvestorFlag          = false;
    stock3.meta.lot                          = 1;
    stock3.meta.pricePrecision               = 3;
    stock3.meta.minPriceIncrement            = 0.0001f;
    stock3.meta.ignore                       = false;
    stock3.meta.lastTradeTime                = QTime(9, 59);
    stock3.meta.turnover                     = 6560000000;
    stock3.meta.dividends.createTimestamp    = 0;
    stock3.meta.dividends.paymentTimestamp   = 0;
    stock3.meta.dividends.price              = 0.0f;
    stock3.meta.dividends.yield              = 0.0f;
    stock3.meta.shorts.enabled               = true;
    stock3.meta.shorts.lastEnabledTimestamp  = 1740776400000;
    stock3.operational.detailedData[0].price = 700.0f;
    stock3.operational.dayStartPrice         = 231.0f;
    stock3.operational.specifiedDatePrice    = 192.0f;
    stock3.operational.payback               = 87.0f;

    model->lastPriceChanged("aaaaa");
    model->lastPriceChanged("bbbbb");
    ASSERT_EQ(model->lastPricesUpdates.size(), 2);
    ASSERT_EQ(model->lastPricesUpdates.contains("aaaaa"), true);
    ASSERT_EQ(model->lastPricesUpdates.contains("bbbbb"), true);

    model->updateLastPrices();

    ASSERT_EQ(model->lastPricesUpdates.size(), 0);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("300.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+100.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+76.47%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("1.12K \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("13.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("800.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+1468.63%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+3536.36%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("2.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("15.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("500.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-0.20%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-0.40%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("5.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("83.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on

    model->sort(STOCKS_PRICE_COLUMN, Qt::DescendingOrder);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("800.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+1468.63%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+3536.36%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("2.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("15.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("500.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-0.20%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-0.40%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("5.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("83.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("300.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+100.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+76.47%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("1.12K \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("13.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on

    stock1.meta.instrumentTicker             = "GORA";
    stock1.meta.instrumentName               = "Go Randy";
    stock1.meta.forQualInvestorFlag          = false;
    stock1.meta.lot                          = 1;
    stock1.meta.pricePrecision               = 3;
    stock1.meta.minPriceIncrement            = 0.01f;
    stock1.meta.ignore                       = false;
    stock1.meta.lastTradeTime                = QTime(18, 39);
    stock1.meta.turnover                     = 3120;
    stock1.meta.dividends.createTimestamp    = 0;
    stock1.meta.dividends.paymentTimestamp   = 0;
    stock1.meta.dividends.price              = 0.0f;
    stock1.meta.dividends.yield              = 0.0f;
    stock1.meta.shorts.enabled               = true;
    stock1.meta.shorts.lastEnabledTimestamp  = 1735678800000;
    stock1.operational.detailedData[0].price = 290.0f;
    stock1.operational.dayStartPrice         = 640.0f;
    stock1.operational.specifiedDatePrice    = 170.0f;
    stock1.operational.payback               = 27.0f;

    stock2.meta.instrumentTicker             = "HIVE";
    stock2.meta.instrumentName               = "Bzzzzz hive";
    stock2.meta.forQualInvestorFlag          = true;
    stock2.meta.lot                          = 1;
    stock2.meta.pricePrecision               = 4;
    stock2.meta.minPriceIncrement            = 0.001f;
    stock2.meta.ignore                       = false;
    stock2.meta.lastTradeTime                = QTime(23, 49);
    stock2.meta.turnover                     = 5340000;
    stock2.meta.dividends.createTimestamp    = 1738357200000;
    stock2.meta.dividends.paymentTimestamp   = 1740776400000;
    stock2.meta.dividends.price              = 750.0f;
    stock2.meta.dividends.yield              = 75.0f;
    stock2.meta.shorts.enabled               = false;
    stock2.meta.shorts.lastEnabledTimestamp  = 1738357200000;
    stock2.operational.detailedData[0].price = 100.0f;
    stock2.operational.dayStartPrice         = 131.0f;
    stock2.operational.specifiedDatePrice    = 922.0f;
    stock2.operational.payback               = 47.0f;

    stock3.meta.instrumentTicker             = "IGOR";
    stock3.meta.instrumentName               = "I go ready";
    stock3.meta.forQualInvestorFlag          = false;
    stock3.meta.lot                          = 1;
    stock3.meta.pricePrecision               = 2;
    stock3.meta.minPriceIncrement            = 0.0001f;
    stock3.meta.ignore                       = false;
    stock3.meta.lastTradeTime                = QTime(9, 59);
    stock3.meta.turnover                     = 7560000000;
    stock3.meta.dividends.createTimestamp    = 0;
    stock3.meta.dividends.paymentTimestamp   = 0;
    stock3.meta.dividends.price              = 0.0f;
    stock3.meta.dividends.yield              = 0.0f;
    stock3.meta.shorts.enabled               = true;
    stock3.meta.shorts.lastEnabledTimestamp  = 1740776400000;
    stock3.operational.detailedData[0].price = 537.0f;
    stock3.operational.dayStartPrice         = 197.0f;
    stock3.operational.specifiedDatePrice    = 325.0f;
    stock3.operational.payback               = 97.0f;

    model->lastPriceChanged("bbbbb");
    ASSERT_EQ(model->lastPricesUpdates.size(), 1);
    ASSERT_EQ(model->lastPricesUpdates.contains("bbbbb"), true);

    model->updateLastPrices();

    ASSERT_EQ(model->lastPricesUpdates.size(), 0);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("500.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-0.20%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-0.40%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("5.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("83.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("300.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+100.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+76.47%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("1.12K \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("13.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("100.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-23.66%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-89.15%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("2.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("15.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on

    StockFilter filter;
    filter.useTurnover  = true;
    filter.turnoverFrom = 1000000;

    model->setFilter(filter);
    ASSERT_EQ(model->rowCount(), 2);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("500.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-0.20%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-0.40%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("5.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("83.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("100.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-23.66%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-89.15%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("2.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("15.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on

    stock1.meta.instrumentTicker             = "JOJO";
    stock1.meta.instrumentName               = "John Johnson";
    stock1.meta.forQualInvestorFlag          = false;
    stock1.meta.lot                          = 1;
    stock1.meta.pricePrecision               = 2;
    stock1.meta.minPriceIncrement            = 0.01f;
    stock1.meta.ignore                       = false;
    stock1.meta.lastTradeTime                = QTime(18, 39);
    stock1.meta.turnover                     = 9310000;
    stock1.meta.dividends.createTimestamp    = 1735678800000;
    stock1.meta.dividends.paymentTimestamp   = 1738357200000;
    stock1.meta.dividends.price              = 330.0f;
    stock1.meta.dividends.yield              = 33.0f;
    stock1.meta.shorts.enabled               = false;
    stock1.meta.shorts.lastEnabledTimestamp  = 1735678800000;
    stock1.operational.detailedData[0].price = 1111.0f;
    stock1.operational.dayStartPrice         = 888.0f;
    stock1.operational.specifiedDatePrice    = 999.0f;
    stock1.operational.payback               = 87.0f;

    stock2.meta.instrumentTicker             = "KOND";
    stock2.meta.instrumentName               = "King of the Northern Dome";
    stock2.meta.forQualInvestorFlag          = true;
    stock2.meta.lot                          = 1;
    stock2.meta.pricePrecision               = 4;
    stock2.meta.minPriceIncrement            = 0.001f;
    stock2.meta.ignore                       = false;
    stock2.meta.lastTradeTime                = QTime(23, 49);
    stock2.meta.turnover                     = 7340000;
    stock2.meta.dividends.createTimestamp    = 1738357200000;
    stock2.meta.dividends.paymentTimestamp   = 1740776400000;
    stock2.meta.dividends.price              = 770.0f;
    stock2.meta.dividends.yield              = 77.0f;
    stock2.meta.shorts.enabled               = true;
    stock2.meta.shorts.lastEnabledTimestamp  = 1738357200000;
    stock2.operational.detailedData[0].price = 2222.0f;
    stock2.operational.dayStartPrice         = 444.0f;
    stock2.operational.specifiedDatePrice    = 555.0f;
    stock2.operational.payback               = 57.0f;

    stock3.meta.instrumentTicker             = "LEMO";
    stock3.meta.instrumentName               = "Lemonade";
    stock3.meta.forQualInvestorFlag          = false;
    stock3.meta.lot                          = 1;
    stock3.meta.pricePrecision               = 3;
    stock3.meta.minPriceIncrement            = 0.0001f;
    stock3.meta.ignore                       = false;
    stock3.meta.lastTradeTime                = QTime(9, 59);
    stock3.meta.turnover                     = 9560;
    stock3.meta.dividends.createTimestamp    = 1740776400000;
    stock3.meta.dividends.paymentTimestamp   = 1743454800000;
    stock3.meta.dividends.price              = 50.0f;
    stock3.meta.dividends.yield              = 5.0f;
    stock3.meta.shorts.enabled               = false;
    stock3.meta.shorts.lastEnabledTimestamp  = 1740776400000;
    stock3.operational.detailedData[0].price = 3333.0f;
    stock3.operational.dayStartPrice         = 666.0f;
    stock3.operational.specifiedDatePrice    = 777.0f;
    stock3.operational.payback               = 37.0f;

    model->lastPriceChanged("aaaaa");
    model->lastPriceChanged("ccccc");
    ASSERT_EQ(model->lastPricesUpdates.size(), 2);
    ASSERT_EQ(model->lastPricesUpdates.contains("aaaaa"), true);
    ASSERT_EQ(model->lastPricesUpdates.contains("ccccc"), true);

    model->updateLastPrices();

    ASSERT_EQ(model->lastPricesUpdates.size(), 0);
    ASSERT_EQ(model->rowCount(), 2);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("3333.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+400.45%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+328.96%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("5.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("83.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("100.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-23.66%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-89.15%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("2.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("15.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on
}

TEST_F(Test_StocksTableModel, Test_updatePrices)
{
    const InSequence seq;

    Logo logo;

    QList<Stock*> stocks;

    Stock stock1;
    Stock stock2;
    Stock stock3;

    stock1.meta.instrumentId                = "aaaaa";
    stock1.meta.instrumentLogo              = nullptr;
    stock1.meta.instrumentTicker            = "ABBA";
    stock1.meta.instrumentName              = "Abstract Basics";
    stock1.meta.forQualInvestorFlag         = false;
    stock1.meta.lot                         = 1;
    stock1.meta.pricePrecision              = 2;
    stock1.meta.minPriceIncrement           = 0.01f;
    stock1.meta.ignore                      = false;
    stock1.meta.lastTradeTime               = QTime(18, 39);
    stock1.meta.turnover                    = 1120;
    stock1.meta.dividends.createTimestamp   = 1735678800000;
    stock1.meta.dividends.paymentTimestamp  = 1738357200000;
    stock1.meta.dividends.price             = 100.0f;
    stock1.meta.dividends.yield             = 10.0f;
    stock1.meta.shorts.enabled              = true;
    stock1.meta.shorts.lastEnabledTimestamp = 1735678800000;
    stock1.operational.dayStartPrice        = 0.0f;
    stock1.operational.specifiedDatePrice   = 0.0f;
    stock1.operational.payback              = 13.0f;

    StockOperationalData data1;
    data1.price = 100.0f;
    stock1.operational.detailedData << data1;

    stock2.meta.instrumentId                = "bbbbb";
    stock2.meta.instrumentLogo              = nullptr;
    stock2.meta.instrumentTicker            = "BASE";
    stock2.meta.instrumentName              = "Basketball enhancement";
    stock2.meta.forQualInvestorFlag         = true;
    stock2.meta.lot                         = 1;
    stock2.meta.pricePrecision              = 3;
    stock2.meta.minPriceIncrement           = 0.001f;
    stock2.meta.ignore                      = false;
    stock2.meta.lastTradeTime               = QTime(23, 49);
    stock2.meta.turnover                    = 2340000;
    stock2.meta.dividends.createTimestamp   = 1738357200000;
    stock2.meta.dividends.paymentTimestamp  = 1740776400000;
    stock2.meta.dividends.price             = 150.0f;
    stock2.meta.dividends.yield             = 15.0f;
    stock2.meta.shorts.enabled              = true;
    stock2.meta.shorts.lastEnabledTimestamp = 1738357200000;
    stock2.operational.dayStartPrice        = 201.0f;
    stock2.operational.specifiedDatePrice   = 202.0f;
    stock2.operational.payback              = 33.0f;

    StockOperationalData data2;
    data2.price = 250.0f;
    stock2.operational.detailedData << data2;

    stock3.meta.instrumentId                = "ccccc";
    stock3.meta.instrumentLogo              = &logo;
    stock3.meta.instrumentTicker            = "CODE";
    stock3.meta.instrumentName              = "Code base";
    stock3.meta.forQualInvestorFlag         = false;
    stock3.meta.lot                         = 1;
    stock3.meta.pricePrecision              = 4;
    stock3.meta.minPriceIncrement           = 0.0001f;
    stock3.meta.ignore                      = false;
    stock3.meta.lastTradeTime               = QTime(9, 59);
    stock3.meta.turnover                    = 5560000000;
    stock3.meta.dividends.createTimestamp   = 0;
    stock3.meta.dividends.paymentTimestamp  = 0;
    stock3.meta.dividends.price             = 0.0f;
    stock3.meta.dividends.yield             = 0.0f;
    stock3.meta.shorts.enabled              = false;
    stock3.meta.shorts.lastEnabledTimestamp = 0;
    stock3.operational.dayStartPrice        = 501.0f;
    stock3.operational.specifiedDatePrice   = 502.0f;
    stock3.operational.payback              = 83.0f;

    StockOperationalData data3;
    data3.price = 500.0f;
    stock3.operational.detailedData << data3;

    stocks << &stock1 << &stock2 << &stock3;

    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, isQualified()).WillOnce(Return(false));
    EXPECT_CALL(*userStorageMock, readUnlock());

    model->updateTable(stocks);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("100.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("1.12K \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("13.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("250.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+24.38%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+23.76%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("2.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("15.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("500.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-0.20%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-0.40%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("5.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("83.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on

    model->sort(STOCKS_ACTIONS_COLUMN, Qt::AscendingOrder);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("100.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("1.12K \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("13.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("250.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+24.38%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+23.76%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("2.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("15.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("500.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-0.20%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-0.40%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("5.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("83.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on

    stock1.meta.instrumentTicker             = "DISY";
    stock1.meta.instrumentName               = "Disintegration yoyo";
    stock1.meta.forQualInvestorFlag          = false;
    stock1.meta.lot                          = 1;
    stock1.meta.pricePrecision               = 4;
    stock1.meta.minPriceIncrement            = 0.01f;
    stock1.meta.ignore                       = false;
    stock1.meta.lastTradeTime                = QTime(18, 39);
    stock1.meta.turnover                     = 2120;
    stock1.meta.dividends.createTimestamp    = 1735678800000;
    stock1.meta.dividends.paymentTimestamp   = 1738357200000;
    stock1.meta.dividends.price              = 300.0f;
    stock1.meta.dividends.yield              = 30.0f;
    stock1.meta.shorts.enabled               = false;
    stock1.meta.shorts.lastEnabledTimestamp  = 1735678800000;
    stock1.operational.detailedData[0].price = 300.0f;
    stock1.operational.dayStartPrice         = 150.0f;
    stock1.operational.specifiedDatePrice    = 170.0f;
    stock1.operational.payback               = 17.0f;

    stock2.meta.instrumentTicker             = "EASY";
    stock2.meta.instrumentName               = "Easy peasy";
    stock2.meta.forQualInvestorFlag          = true;
    stock2.meta.lot                          = 1;
    stock2.meta.pricePrecision               = 2;
    stock2.meta.minPriceIncrement            = 0.001f;
    stock2.meta.ignore                       = false;
    stock2.meta.lastTradeTime                = QTime(23, 49);
    stock2.meta.turnover                     = 4340000;
    stock2.meta.dividends.createTimestamp    = 1738357200000;
    stock2.meta.dividends.paymentTimestamp   = 1740776400000;
    stock2.meta.dividends.price              = 450.0f;
    stock2.meta.dividends.yield              = 45.0f;
    stock2.meta.shorts.enabled               = false;
    stock2.meta.shorts.lastEnabledTimestamp  = 1738357200000;
    stock2.operational.detailedData[0].price = 800.0f;
    stock2.operational.dayStartPrice         = 51.0f;
    stock2.operational.specifiedDatePrice    = 22.0f;
    stock2.operational.payback               = 37.0f;

    stock3.meta.instrumentTicker             = "FUCK";
    stock3.meta.instrumentName               = "Functional clock";
    stock3.meta.forQualInvestorFlag          = false;
    stock3.meta.lot                          = 1;
    stock3.meta.pricePrecision               = 3;
    stock3.meta.minPriceIncrement            = 0.0001f;
    stock3.meta.ignore                       = false;
    stock3.meta.lastTradeTime                = QTime(9, 59);
    stock3.meta.turnover                     = 6560000000;
    stock3.meta.dividends.createTimestamp    = 0;
    stock3.meta.dividends.paymentTimestamp   = 0;
    stock3.meta.dividends.price              = 0.0f;
    stock3.meta.dividends.yield              = 0.0f;
    stock3.meta.shorts.enabled               = true;
    stock3.meta.shorts.lastEnabledTimestamp  = 1740776400000;
    stock3.operational.detailedData[0].price = 700.0f;
    stock3.operational.dayStartPrice         = 231.0f;
    stock3.operational.specifiedDatePrice    = 192.0f;
    stock3.operational.payback               = 87.0f;

    model->updatePrices();
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("300.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+100.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+76.47%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("1.12K \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("13.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("800.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+1468.63%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+3536.36%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("2.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("15.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("700.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+203.03%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+264.58%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("5.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("83.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on

    model->sort(STOCKS_PRICE_COLUMN, Qt::DescendingOrder);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("800.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+1468.63%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+3536.36%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("2.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("15.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("700.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+203.03%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+264.58%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("5.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("83.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("300.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+100.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+76.47%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("1.12K \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("13.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on

    stock1.meta.instrumentTicker             = "GORA";
    stock1.meta.instrumentName               = "Go Randy";
    stock1.meta.forQualInvestorFlag          = false;
    stock1.meta.lot                          = 1;
    stock1.meta.pricePrecision               = 3;
    stock1.meta.minPriceIncrement            = 0.01f;
    stock1.meta.ignore                       = false;
    stock1.meta.lastTradeTime                = QTime(18, 39);
    stock1.meta.turnover                     = 3120;
    stock1.meta.dividends.createTimestamp    = 0;
    stock1.meta.dividends.paymentTimestamp   = 0;
    stock1.meta.dividends.price              = 0.0f;
    stock1.meta.dividends.yield              = 0.0f;
    stock1.meta.shorts.enabled               = true;
    stock1.meta.shorts.lastEnabledTimestamp  = 1735678800000;
    stock1.operational.detailedData[0].price = 290.0f;
    stock1.operational.dayStartPrice         = 640.0f;
    stock1.operational.specifiedDatePrice    = 170.0f;
    stock1.operational.payback               = 27.0f;

    stock2.meta.instrumentTicker             = "HIVE";
    stock2.meta.instrumentName               = "Bzzzzz hive";
    stock2.meta.forQualInvestorFlag          = true;
    stock2.meta.lot                          = 1;
    stock2.meta.pricePrecision               = 4;
    stock2.meta.minPriceIncrement            = 0.001f;
    stock2.meta.ignore                       = false;
    stock2.meta.lastTradeTime                = QTime(23, 49);
    stock2.meta.turnover                     = 5340000;
    stock2.meta.dividends.createTimestamp    = 1738357200000;
    stock2.meta.dividends.paymentTimestamp   = 1740776400000;
    stock2.meta.dividends.price              = 750.0f;
    stock2.meta.dividends.yield              = 75.0f;
    stock2.meta.shorts.enabled               = false;
    stock2.meta.shorts.lastEnabledTimestamp  = 1738357200000;
    stock2.operational.detailedData[0].price = 100.0f;
    stock2.operational.dayStartPrice         = 131.0f;
    stock2.operational.specifiedDatePrice    = 922.0f;
    stock2.operational.payback               = 47.0f;

    stock3.meta.instrumentTicker             = "IGOR";
    stock3.meta.instrumentName               = "I go ready";
    stock3.meta.forQualInvestorFlag          = false;
    stock3.meta.lot                          = 1;
    stock3.meta.pricePrecision               = 2;
    stock3.meta.minPriceIncrement            = 0.0001f;
    stock3.meta.ignore                       = false;
    stock3.meta.lastTradeTime                = QTime(9, 59);
    stock3.meta.turnover                     = 7560000000;
    stock3.meta.dividends.createTimestamp    = 0;
    stock3.meta.dividends.paymentTimestamp   = 0;
    stock3.meta.dividends.price              = 0.0f;
    stock3.meta.dividends.yield              = 0.0f;
    stock3.meta.shorts.enabled               = true;
    stock3.meta.shorts.lastEnabledTimestamp  = 1740776400000;
    stock3.operational.detailedData[0].price = 537.0f;
    stock3.operational.dayStartPrice         = 197.0f;
    stock3.operational.specifiedDatePrice    = 325.0f;
    stock3.operational.payback               = 97.0f;

    model->updatePrices();
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("537.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+172.59%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+65.23%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("5.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("83.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("290.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-54.69%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+70.59%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("1.12K \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("13.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("100.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-23.66%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-89.15%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("2.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("15.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on

    StockFilter filter;
    filter.useTurnover  = true;
    filter.turnoverFrom = 1000000;

    model->setFilter(filter);
    ASSERT_EQ(model->rowCount(), 2);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("537.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+172.59%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+65.23%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("5.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("83.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("100.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-23.66%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-89.15%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("2.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("15.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on

    stock1.meta.instrumentTicker             = "JOJO";
    stock1.meta.instrumentName               = "John Johnson";
    stock1.meta.forQualInvestorFlag          = false;
    stock1.meta.lot                          = 1;
    stock1.meta.pricePrecision               = 2;
    stock1.meta.minPriceIncrement            = 0.01f;
    stock1.meta.ignore                       = false;
    stock1.meta.lastTradeTime                = QTime(18, 39);
    stock1.meta.turnover                     = 9310000;
    stock1.meta.dividends.createTimestamp    = 1735678800000;
    stock1.meta.dividends.paymentTimestamp   = 1738357200000;
    stock1.meta.dividends.price              = 330.0f;
    stock1.meta.dividends.yield              = 33.0f;
    stock1.meta.shorts.enabled               = false;
    stock1.meta.shorts.lastEnabledTimestamp  = 1735678800000;
    stock1.operational.detailedData[0].price = 1111.0f;
    stock1.operational.dayStartPrice         = 888.0f;
    stock1.operational.specifiedDatePrice    = 999.0f;
    stock1.operational.payback               = 87.0f;

    stock2.meta.instrumentTicker             = "KOND";
    stock2.meta.instrumentName               = "King of the Northern Dome";
    stock2.meta.forQualInvestorFlag          = true;
    stock2.meta.lot                          = 1;
    stock2.meta.pricePrecision               = 4;
    stock2.meta.minPriceIncrement            = 0.001f;
    stock2.meta.ignore                       = false;
    stock2.meta.lastTradeTime                = QTime(23, 49);
    stock2.meta.turnover                     = 7340000;
    stock2.meta.dividends.createTimestamp    = 1738357200000;
    stock2.meta.dividends.paymentTimestamp   = 1740776400000;
    stock2.meta.dividends.price              = 770.0f;
    stock2.meta.dividends.yield              = 77.0f;
    stock2.meta.shorts.enabled               = true;
    stock2.meta.shorts.lastEnabledTimestamp  = 1738357200000;
    stock2.operational.detailedData[0].price = 2222.0f;
    stock2.operational.dayStartPrice         = 444.0f;
    stock2.operational.specifiedDatePrice    = 555.0f;
    stock2.operational.payback               = 57.0f;

    stock3.meta.instrumentTicker             = "LEMO";
    stock3.meta.instrumentName               = "Lemonade";
    stock3.meta.forQualInvestorFlag          = false;
    stock3.meta.lot                          = 1;
    stock3.meta.pricePrecision               = 3;
    stock3.meta.minPriceIncrement            = 0.0001f;
    stock3.meta.ignore                       = false;
    stock3.meta.lastTradeTime                = QTime(9, 59);
    stock3.meta.turnover                     = 9560;
    stock3.meta.dividends.createTimestamp    = 1740776400000;
    stock3.meta.dividends.paymentTimestamp   = 1743454800000;
    stock3.meta.dividends.price              = 50.0f;
    stock3.meta.dividends.yield              = 5.0f;
    stock3.meta.shorts.enabled               = false;
    stock3.meta.shorts.lastEnabledTimestamp  = 1740776400000;
    stock3.operational.detailedData[0].price = 3333.0f;
    stock3.operational.dayStartPrice         = 666.0f;
    stock3.operational.specifiedDatePrice    = 777.0f;
    stock3.operational.payback               = 37.0f;

    model->updatePrices();
    ASSERT_EQ(model->rowCount(), 2);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("3333.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+400.45%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+328.96%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("5.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("83.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("2222.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+400.45%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+300.36%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("2.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("15.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on
}

TEST_F(Test_StocksTableModel, Test_updatePeriodicData)
{
    const InSequence seq;

    Logo logo;

    QList<Stock*> stocks;

    Stock stock1;
    Stock stock2;
    Stock stock3;

    stock1.meta.instrumentId                = "aaaaa";
    stock1.meta.instrumentLogo              = nullptr;
    stock1.meta.instrumentTicker            = "ABBA";
    stock1.meta.instrumentName              = "Abstract Basics";
    stock1.meta.forQualInvestorFlag         = false;
    stock1.meta.lot                         = 1;
    stock1.meta.pricePrecision              = 2;
    stock1.meta.minPriceIncrement           = 0.01f;
    stock1.meta.ignore                      = false;
    stock1.meta.lastTradeTime               = QTime(18, 39);
    stock1.meta.turnover                    = 1120;
    stock1.meta.dividends.createTimestamp   = 1735678800000;
    stock1.meta.dividends.paymentTimestamp  = 1738357200000;
    stock1.meta.dividends.price             = 100.0f;
    stock1.meta.dividends.yield             = 10.0f;
    stock1.meta.shorts.enabled              = true;
    stock1.meta.shorts.lastEnabledTimestamp = 1735678800000;
    stock1.operational.dayStartPrice        = 0.0f;
    stock1.operational.specifiedDatePrice   = 0.0f;
    stock1.operational.payback              = 13.0f;

    StockOperationalData data1;
    data1.price = 100.0f;
    stock1.operational.detailedData << data1;

    stock2.meta.instrumentId                = "bbbbb";
    stock2.meta.instrumentLogo              = nullptr;
    stock2.meta.instrumentTicker            = "BASE";
    stock2.meta.instrumentName              = "Basketball enhancement";
    stock2.meta.forQualInvestorFlag         = true;
    stock2.meta.lot                         = 1;
    stock2.meta.pricePrecision              = 3;
    stock2.meta.minPriceIncrement           = 0.001f;
    stock2.meta.ignore                      = false;
    stock2.meta.lastTradeTime               = QTime(23, 49);
    stock2.meta.turnover                    = 2340000;
    stock2.meta.dividends.createTimestamp   = 1738357200000;
    stock2.meta.dividends.paymentTimestamp  = 1740776400000;
    stock2.meta.dividends.price             = 150.0f;
    stock2.meta.dividends.yield             = 15.0f;
    stock2.meta.shorts.enabled              = true;
    stock2.meta.shorts.lastEnabledTimestamp = 1738357200000;
    stock2.operational.dayStartPrice        = 201.0f;
    stock2.operational.specifiedDatePrice   = 202.0f;
    stock2.operational.payback              = 33.0f;

    StockOperationalData data2;
    data2.price = 250.0f;
    stock2.operational.detailedData << data2;

    stock3.meta.instrumentId                = "ccccc";
    stock3.meta.instrumentLogo              = &logo;
    stock3.meta.instrumentTicker            = "CODE";
    stock3.meta.instrumentName              = "Code base";
    stock3.meta.forQualInvestorFlag         = false;
    stock3.meta.lot                         = 1;
    stock3.meta.pricePrecision              = 4;
    stock3.meta.minPriceIncrement           = 0.0001f;
    stock3.meta.ignore                      = false;
    stock3.meta.lastTradeTime               = QTime(9, 59);
    stock3.meta.turnover                    = 5560000000;
    stock3.meta.dividends.createTimestamp   = 0;
    stock3.meta.dividends.paymentTimestamp  = 0;
    stock3.meta.dividends.price             = 0.0f;
    stock3.meta.dividends.yield             = 0.0f;
    stock3.meta.shorts.enabled              = false;
    stock3.meta.shorts.lastEnabledTimestamp = 0;
    stock3.operational.dayStartPrice        = 501.0f;
    stock3.operational.specifiedDatePrice   = 502.0f;
    stock3.operational.payback              = 83.0f;

    StockOperationalData data3;
    data3.price = 500.0f;
    stock3.operational.detailedData << data3;

    stocks << &stock1 << &stock2 << &stock3;

    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, isQualified()).WillOnce(Return(false));
    EXPECT_CALL(*userStorageMock, readUnlock());

    model->updateTable(stocks);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("100.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("1.12K \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("13.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("250.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+24.38%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+23.76%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("2.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("15.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("500.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-0.20%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-0.40%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("5.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("83.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on

    model->sort(STOCKS_ACTIONS_COLUMN, Qt::AscendingOrder);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("100.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("1.12K \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("13.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("250.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+24.38%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+23.76%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("2.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("15.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("500.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-0.20%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-0.40%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("5.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("83.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on

    stock1.meta.instrumentTicker             = "DISY";
    stock1.meta.instrumentName               = "Disintegration yoyo";
    stock1.meta.forQualInvestorFlag          = false;
    stock1.meta.lot                          = 1;
    stock1.meta.pricePrecision               = 4;
    stock1.meta.minPriceIncrement            = 0.01f;
    stock1.meta.ignore                       = false;
    stock1.meta.lastTradeTime                = QTime(18, 39);
    stock1.meta.turnover                     = 2120;
    stock1.meta.dividends.createTimestamp    = 1735678800000;
    stock1.meta.dividends.paymentTimestamp   = 1738357200000;
    stock1.meta.dividends.price              = 300.0f;
    stock1.meta.dividends.yield              = 30.0f;
    stock1.meta.shorts.enabled               = false;
    stock1.meta.shorts.lastEnabledTimestamp  = 1735678800000;
    stock1.operational.detailedData[0].price = 300.0f;
    stock1.operational.dayStartPrice         = 150.0f;
    stock1.operational.specifiedDatePrice    = 170.0f;
    stock1.operational.payback               = 17.0f;

    stock2.meta.instrumentTicker             = "EASY";
    stock2.meta.instrumentName               = "Easy peasy";
    stock2.meta.forQualInvestorFlag          = true;
    stock2.meta.lot                          = 1;
    stock2.meta.pricePrecision               = 2;
    stock2.meta.minPriceIncrement            = 0.001f;
    stock2.meta.ignore                       = false;
    stock2.meta.lastTradeTime                = QTime(23, 49);
    stock2.meta.turnover                     = 4340000;
    stock2.meta.dividends.createTimestamp    = 1738357200000;
    stock2.meta.dividends.paymentTimestamp   = 1740776400000;
    stock2.meta.dividends.price              = 450.0f;
    stock2.meta.dividends.yield              = 45.0f;
    stock2.meta.shorts.enabled               = false;
    stock2.meta.shorts.lastEnabledTimestamp  = 1738357200000;
    stock2.operational.detailedData[0].price = 800.0f;
    stock2.operational.dayStartPrice         = 51.0f;
    stock2.operational.specifiedDatePrice    = 22.0f;
    stock2.operational.payback               = 37.0f;

    stock3.meta.instrumentTicker             = "FUCK";
    stock3.meta.instrumentName               = "Functional clock";
    stock3.meta.forQualInvestorFlag          = false;
    stock3.meta.lot                          = 1;
    stock3.meta.pricePrecision               = 3;
    stock3.meta.minPriceIncrement            = 0.0001f;
    stock3.meta.ignore                       = false;
    stock3.meta.lastTradeTime                = QTime(9, 59);
    stock3.meta.turnover                     = 6560000000;
    stock3.meta.dividends.createTimestamp    = 0;
    stock3.meta.dividends.paymentTimestamp   = 0;
    stock3.meta.dividends.price              = 0.0f;
    stock3.meta.dividends.yield              = 0.0f;
    stock3.meta.shorts.enabled               = true;
    stock3.meta.shorts.lastEnabledTimestamp  = 1740776400000;
    stock3.operational.detailedData[0].price = 700.0f;
    stock3.operational.dayStartPrice         = 231.0f;
    stock3.operational.specifiedDatePrice    = 192.0f;
    stock3.operational.payback               = 87.0f;

    model->updatePeriodicData();
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("100.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("2.12K \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("17.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("30.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("250.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+24.38%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+23.76%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("4.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("37.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("45.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("500.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-0.20%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-0.40%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("6.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("87.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on

    model->sort(STOCKS_TURNOVER_COLUMN, Qt::DescendingOrder);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("500.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-0.20%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-0.40%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("6.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("87.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("250.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+24.38%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+23.76%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("4.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("37.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("45.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("100.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("2.12K \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("17.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("30.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on

    stock1.meta.instrumentTicker             = "GORA";
    stock1.meta.instrumentName               = "Go Randy";
    stock1.meta.forQualInvestorFlag          = false;
    stock1.meta.lot                          = 1;
    stock1.meta.pricePrecision               = 3;
    stock1.meta.minPriceIncrement            = 0.01f;
    stock1.meta.ignore                       = false;
    stock1.meta.lastTradeTime                = QTime(18, 39);
    stock1.meta.turnover                     = 3120;
    stock1.meta.dividends.createTimestamp    = 0;
    stock1.meta.dividends.paymentTimestamp   = 0;
    stock1.meta.dividends.price              = 0.0f;
    stock1.meta.dividends.yield              = 0.0f;
    stock1.meta.shorts.enabled               = true;
    stock1.meta.shorts.lastEnabledTimestamp  = 1735678800000;
    stock1.operational.detailedData[0].price = 290.0f;
    stock1.operational.dayStartPrice         = 640.0f;
    stock1.operational.specifiedDatePrice    = 170.0f;
    stock1.operational.payback               = 27.0f;

    stock2.meta.instrumentTicker             = "HIVE";
    stock2.meta.instrumentName               = "Bzzzzz hive";
    stock2.meta.forQualInvestorFlag          = true;
    stock2.meta.lot                          = 1;
    stock2.meta.pricePrecision               = 4;
    stock2.meta.minPriceIncrement            = 0.001f;
    stock2.meta.ignore                       = false;
    stock2.meta.lastTradeTime                = QTime(23, 49);
    stock2.meta.turnover                     = 5340000;
    stock2.meta.dividends.createTimestamp    = 1738357200000;
    stock2.meta.dividends.paymentTimestamp   = 1740776400000;
    stock2.meta.dividends.price              = 750.0f;
    stock2.meta.dividends.yield              = 75.0f;
    stock2.meta.shorts.enabled               = false;
    stock2.meta.shorts.lastEnabledTimestamp  = 1738357200000;
    stock2.operational.detailedData[0].price = 100.0f;
    stock2.operational.dayStartPrice         = 131.0f;
    stock2.operational.specifiedDatePrice    = 922.0f;
    stock2.operational.payback               = 47.0f;

    stock3.meta.instrumentTicker             = "IGOR";
    stock3.meta.instrumentName               = "I go ready";
    stock3.meta.forQualInvestorFlag          = false;
    stock3.meta.lot                          = 1;
    stock3.meta.pricePrecision               = 2;
    stock3.meta.minPriceIncrement            = 0.0001f;
    stock3.meta.ignore                       = false;
    stock3.meta.lastTradeTime                = QTime(9, 59);
    stock3.meta.turnover                     = 7560000000;
    stock3.meta.dividends.createTimestamp    = 0;
    stock3.meta.dividends.paymentTimestamp   = 0;
    stock3.meta.dividends.price              = 0.0f;
    stock3.meta.dividends.yield              = 0.0f;
    stock3.meta.shorts.enabled               = true;
    stock3.meta.shorts.lastEnabledTimestamp  = 1740776400000;
    stock3.operational.detailedData[0].price = 537.0f;
    stock3.operational.dayStartPrice         = 197.0f;
    stock3.operational.specifiedDatePrice    = 325.0f;
    stock3.operational.payback               = 97.0f;

    model->updatePeriodicData();
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("500.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-0.20%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-0.40%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("7.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("97.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("250.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+24.38%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+23.76%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("5.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("47.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("75.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("100.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("3.12K \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("27.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on

    StockFilter filter;
    filter.useTurnover  = true;
    filter.turnoverFrom = 1000000;

    model->setFilter(filter);
    ASSERT_EQ(model->rowCount(), 2);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("500.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-0.20%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-0.40%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("7.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("97.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("250.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+24.38%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+23.76%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("5.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("47.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("75.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on

    stock1.meta.instrumentTicker             = "JOJO";
    stock1.meta.instrumentName               = "John Johnson";
    stock1.meta.forQualInvestorFlag          = false;
    stock1.meta.lot                          = 1;
    stock1.meta.pricePrecision               = 2;
    stock1.meta.minPriceIncrement            = 0.01f;
    stock1.meta.ignore                       = false;
    stock1.meta.lastTradeTime                = QTime(18, 39);
    stock1.meta.turnover                     = 9310000;
    stock1.meta.dividends.createTimestamp    = 1735678800000;
    stock1.meta.dividends.paymentTimestamp   = 1738357200000;
    stock1.meta.dividends.price              = 330.0f;
    stock1.meta.dividends.yield              = 33.0f;
    stock1.meta.shorts.enabled               = false;
    stock1.meta.shorts.lastEnabledTimestamp  = 1735678800000;
    stock1.operational.detailedData[0].price = 1111.0f;
    stock1.operational.dayStartPrice         = 888.0f;
    stock1.operational.specifiedDatePrice    = 999.0f;
    stock1.operational.payback               = 87.0f;

    stock2.meta.instrumentTicker             = "KOND";
    stock2.meta.instrumentName               = "King of the Northern Dome";
    stock2.meta.forQualInvestorFlag          = true;
    stock2.meta.lot                          = 1;
    stock2.meta.pricePrecision               = 4;
    stock2.meta.minPriceIncrement            = 0.001f;
    stock2.meta.ignore                       = false;
    stock2.meta.lastTradeTime                = QTime(23, 49);
    stock2.meta.turnover                     = 7340000;
    stock2.meta.dividends.createTimestamp    = 1738357200000;
    stock2.meta.dividends.paymentTimestamp   = 1740776400000;
    stock2.meta.dividends.price              = 770.0f;
    stock2.meta.dividends.yield              = 77.0f;
    stock2.meta.shorts.enabled               = true;
    stock2.meta.shorts.lastEnabledTimestamp  = 1738357200000;
    stock2.operational.detailedData[0].price = 2222.0f;
    stock2.operational.dayStartPrice         = 444.0f;
    stock2.operational.specifiedDatePrice    = 555.0f;
    stock2.operational.payback               = 57.0f;

    stock3.meta.instrumentTicker             = "LEMO";
    stock3.meta.instrumentName               = "Lemonade";
    stock3.meta.forQualInvestorFlag          = false;
    stock3.meta.lot                          = 1;
    stock3.meta.pricePrecision               = 3;
    stock3.meta.minPriceIncrement            = 0.0001f;
    stock3.meta.ignore                       = false;
    stock3.meta.lastTradeTime                = QTime(9, 59);
    stock3.meta.turnover                     = 9560;
    stock3.meta.dividends.createTimestamp    = 1740776400000;
    stock3.meta.dividends.paymentTimestamp   = 1743454800000;
    stock3.meta.dividends.price              = 50.0f;
    stock3.meta.dividends.yield              = 5.0f;
    stock3.meta.shorts.enabled               = false;
    stock3.meta.shorts.lastEnabledTimestamp  = 1740776400000;
    stock3.operational.detailedData[0].price = 3333.0f;
    stock3.operational.dayStartPrice         = 666.0f;
    stock3.operational.specifiedDatePrice    = 777.0f;
    stock3.operational.payback               = 37.0f;

    model->updatePeriodicData();
    ASSERT_EQ(model->rowCount(), 2);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("100.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("9.31M \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("87.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("250.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+24.38%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+23.76%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("7.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("57.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("77.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on
}

TEST_F(Test_StocksTableModel, Test_exportToExcel)
{
    const InSequence seq;

    Logo logo;

    QList<Stock*> stocks;

    Stock stock1;
    Stock stock2;
    Stock stock3;

    stock1.meta.instrumentId                = "aaaaa";
    stock1.meta.instrumentLogo              = nullptr;
    stock1.meta.instrumentTicker            = "ABBA";
    stock1.meta.instrumentName              = "Abstract Basics";
    stock1.meta.forQualInvestorFlag         = false;
    stock1.meta.lot                         = 1;
    stock1.meta.pricePrecision              = 2;
    stock1.meta.minPriceIncrement           = 0.01f;
    stock1.meta.ignore                      = false;
    stock1.meta.lastTradeTime               = QTime(18, 39);
    stock1.meta.turnover                    = 1120;
    stock1.meta.dividends.createTimestamp   = 1735678800000;
    stock1.meta.dividends.paymentTimestamp  = 1738357200000;
    stock1.meta.dividends.price             = 100.0f;
    stock1.meta.dividends.yield             = 10.0f;
    stock1.meta.shorts.enabled              = true;
    stock1.meta.shorts.lastEnabledTimestamp = 1735678800000;
    stock1.operational.dayStartPrice        = 0.0f;
    stock1.operational.specifiedDatePrice   = 0.0f;
    stock1.operational.payback              = 13.0f;

    StockOperationalData data1;
    data1.price = 100.0f;
    stock1.operational.detailedData << data1;

    stock2.meta.instrumentId                = "bbbbb";
    stock2.meta.instrumentLogo              = nullptr;
    stock2.meta.instrumentTicker            = "BASE";
    stock2.meta.instrumentName              = "Basketball enhancement";
    stock2.meta.forQualInvestorFlag         = true;
    stock2.meta.lot                         = 1;
    stock2.meta.pricePrecision              = 3;
    stock2.meta.minPriceIncrement           = 0.001f;
    stock2.meta.ignore                      = false;
    stock2.meta.lastTradeTime               = QTime(23, 49);
    stock2.meta.turnover                    = 2340000;
    stock2.meta.dividends.createTimestamp   = 1738357200000;
    stock2.meta.dividends.paymentTimestamp  = 1740776400000;
    stock2.meta.dividends.price             = 150.0f;
    stock2.meta.dividends.yield             = 15.0f;
    stock2.meta.shorts.enabled              = true;
    stock2.meta.shorts.lastEnabledTimestamp = 1738357200000;
    stock2.operational.dayStartPrice        = 201.0f;
    stock2.operational.specifiedDatePrice   = 202.0f;
    stock2.operational.payback              = 33.0f;

    StockOperationalData data2;
    data2.price = 250.0f;
    stock2.operational.detailedData << data2;

    stock3.meta.instrumentId                = "ccccc";
    stock3.meta.instrumentLogo              = &logo;
    stock3.meta.instrumentTicker            = "CODE";
    stock3.meta.instrumentName              = "Code base";
    stock3.meta.forQualInvestorFlag         = false;
    stock3.meta.lot                         = 1;
    stock3.meta.pricePrecision              = 4;
    stock3.meta.minPriceIncrement           = 0.0001f;
    stock3.meta.ignore                      = false;
    stock3.meta.lastTradeTime               = QTime(9, 59);
    stock3.meta.turnover                    = 5560000000;
    stock3.meta.dividends.createTimestamp   = 0;
    stock3.meta.dividends.paymentTimestamp  = 0;
    stock3.meta.dividends.price             = 0.0f;
    stock3.meta.dividends.yield             = 0.0f;
    stock3.meta.shorts.enabled              = false;
    stock3.meta.shorts.lastEnabledTimestamp = 0;
    stock3.operational.dayStartPrice        = 501.0f;
    stock3.operational.specifiedDatePrice   = 502.0f;
    stock3.operational.payback              = 83.0f;

    StockOperationalData data3;
    data3.price = 500.0f;
    stock3.operational.detailedData << data3;

    stocks << &stock1 << &stock2 << &stock3;

    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, isQualified()).WillOnce(Return(false));
    EXPECT_CALL(*userStorageMock, readUnlock());

    model->updateTable(stocks);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("100.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("1.12K \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("13.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("250.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+24.38%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+23.76%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("2.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("15.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant("V"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("500.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-0.20%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-0.40%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("5.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("83.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DIVIDENDS_COLUMN),   Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_SHORTS_COLUMN),      Qt::DisplayRole), QVariant(""));
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on

    QXlsx::Document doc;

    model->exportToExcel(doc);

    // clang-format off
    ASSERT_EQ(doc.cellAt(2, STOCKS_NAME_COLUMN + 1)->value(),                     QVariant("Abstract Basics"));
    ASSERT_EQ(doc.cellAt(2, STOCKS_NAME_COLUMN + 2)->value(),                     QVariant(false));
    ASSERT_NEAR(doc.cellAt(2, STOCKS_PRICE_COLUMN + 2)->value().toDouble(),       100.0, 0.0001);
    ASSERT_NEAR(doc.cellAt(2, STOCKS_DAY_CHANGE_COLUMN + 2)->value().toDouble(),  0.0, 0.0001);
    ASSERT_NEAR(doc.cellAt(2, STOCKS_DATE_CHANGE_COLUMN + 2)->value().toDouble(), 0.0, 0.0001);
    ASSERT_EQ(doc.cellAt(2, STOCKS_TURNOVER_COLUMN + 2)->value(),                 QVariant(1120));
    ASSERT_NEAR(doc.cellAt(2, STOCKS_PAYBACK_COLUMN + 2)->value().toDouble(),     0.13, 0.0001);
    ASSERT_NEAR(doc.cellAt(2, STOCKS_DIVIDENDS_COLUMN + 2)->value().toDouble(),   0.1, 0.0001);
    ASSERT_EQ(doc.cellAt(2, STOCKS_SHORTS_COLUMN + 2)->value(),                   QVariant(true));
    ASSERT_EQ(doc.cellAt(3, STOCKS_NAME_COLUMN + 1)->value(),                     QVariant("Basketball enhancement"));
    ASSERT_EQ(doc.cellAt(3, STOCKS_NAME_COLUMN + 2)->value(),                     QVariant(true));
    ASSERT_NEAR(doc.cellAt(3, STOCKS_PRICE_COLUMN + 2)->value().toDouble(),       250.0, 0.0001);
    ASSERT_NEAR(doc.cellAt(3, STOCKS_DAY_CHANGE_COLUMN + 2)->value().toDouble(),  0.24378, 0.0001);
    ASSERT_NEAR(doc.cellAt(3, STOCKS_DATE_CHANGE_COLUMN + 2)->value().toDouble(), 0.23762, 0.0001);
    ASSERT_EQ(doc.cellAt(3, STOCKS_TURNOVER_COLUMN + 2)->value(),                 QVariant(2340000));
    ASSERT_NEAR(doc.cellAt(3, STOCKS_PAYBACK_COLUMN + 2)->value().toDouble(),     0.33, 0.0001);
    ASSERT_NEAR(doc.cellAt(3, STOCKS_DIVIDENDS_COLUMN + 2)->value().toDouble(),   0.15, 0.0001);
    ASSERT_EQ(doc.cellAt(3, STOCKS_SHORTS_COLUMN + 2)->value(),                   QVariant(true));
    ASSERT_EQ(doc.cellAt(4, STOCKS_NAME_COLUMN + 1)->value(),                     QVariant("Code base"));
    ASSERT_EQ(doc.cellAt(4, STOCKS_NAME_COLUMN + 2)->value(),                     QVariant(false));
    ASSERT_NEAR(doc.cellAt(4, STOCKS_PRICE_COLUMN + 2)->value().toDouble(),       500.0, 0.0001);
    ASSERT_NEAR(doc.cellAt(4, STOCKS_DAY_CHANGE_COLUMN + 2)->value().toDouble(),  -0.0019, 0.0001);
    ASSERT_NEAR(doc.cellAt(4, STOCKS_DATE_CHANGE_COLUMN + 2)->value().toDouble(), -0.00398, 0.0001);
    ASSERT_EQ(doc.cellAt(4, STOCKS_TURNOVER_COLUMN + 2)->value(),                 QVariant(5560000000LL));
    ASSERT_NEAR(doc.cellAt(4, STOCKS_PAYBACK_COLUMN + 2)->value().toDouble(),     0.83, 0.0001);
    ASSERT_NEAR(doc.cellAt(4, STOCKS_DIVIDENDS_COLUMN + 2)->value().toDouble(),   0.0, 0.0001);
    ASSERT_EQ(doc.cellAt(4, STOCKS_SHORTS_COLUMN + 2)->value(),                   QVariant(false));
    // clang-format on
}

TEST_F(Test_StocksTableModel, Test_setDateChangeTooltip)
{
    ASSERT_EQ(model->headerData(STOCKS_DATE_CHANGE_COLUMN, Qt::Horizontal, Qt::ToolTipRole), QVariant(""));

    model->setDateChangeTooltip("aaaa");

    ASSERT_EQ(model->headerData(STOCKS_DATE_CHANGE_COLUMN, Qt::Horizontal, Qt::ToolTipRole), QVariant("aaaa"));

    model->setDateChangeTooltip("bbbb");

    ASSERT_EQ(model->headerData(STOCKS_DATE_CHANGE_COLUMN, Qt::Horizontal, Qt::ToolTipRole), QVariant("bbbb"));
}
