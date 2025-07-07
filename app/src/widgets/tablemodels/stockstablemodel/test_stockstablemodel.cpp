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

    std::shared_ptr<Stock> stock1 = std::make_shared<Stock>();
    std::shared_ptr<Stock> stock2 = std::make_shared<Stock>();
    std::shared_ptr<Stock> stock3 = std::make_shared<Stock>();
    std::shared_ptr<Stock> stock4 = std::make_shared<Stock>();
    std::shared_ptr<Stock> stock5 = std::make_shared<Stock>();

    stocks << stock1.get() << stock2.get() << stock3.get();

    EXPECT_CALL(*userStorageMock, isQualified()).WillOnce(Return(false));

    model->updateTable(stocks);
    ASSERT_EQ(model->rowCount(), 3);

    stocks << stock4.get() << stock5.get();

    EXPECT_CALL(*userStorageMock, isQualified()).WillOnce(Return(false));

    model->updateTable(stocks);
    ASSERT_EQ(model->rowCount(), 5);

    stocks.clear();

    EXPECT_CALL(*userStorageMock, isQualified()).WillOnce(Return(false));

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
    ASSERT_EQ(model->headerData(STOCKS_ACTIONS_COLUMN,     Qt::Horizontal, Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->headerData(STOCKS_NAME_COLUMN,        Qt::Horizontal, Qt::DecorationRole),        QVariant());
    ASSERT_EQ(model->headerData(STOCKS_PRICE_COLUMN,       Qt::Horizontal, Qt::DecorationRole),        QVariant());
    ASSERT_EQ(model->headerData(STOCKS_DAY_CHANGE_COLUMN,  Qt::Horizontal, Qt::DecorationRole),        QVariant());
    ASSERT_NE(model->headerData(STOCKS_DATE_CHANGE_COLUMN, Qt::Horizontal, Qt::DecorationRole),        QVariant());
    ASSERT_NE(model->headerData(STOCKS_TURNOVER_COLUMN,    Qt::Horizontal, Qt::DecorationRole),        QVariant());
    ASSERT_NE(model->headerData(STOCKS_PAYBACK_COLUMN,     Qt::Horizontal, Qt::DecorationRole),        QVariant());
    ASSERT_EQ(model->headerData(STOCKS_ACTIONS_COLUMN,     Qt::Horizontal, Qt::DecorationRole),        QVariant());
    ASSERT_EQ(model->headerData(0,                         Qt::Horizontal, Qt::WhatsThisPropertyRole), QVariant());
    // clang-format on
}

TEST_F(Test_StocksTableModel, Test_data)
{
    const InSequence seq;

    Logo logo;

    QList<Stock*> stocks;

    std::shared_ptr<Stock> stock1 = std::make_shared<Stock>();
    std::shared_ptr<Stock> stock2 = std::make_shared<Stock>();
    std::shared_ptr<Stock> stock3 = std::make_shared<Stock>();

    stock1->meta.instrumentId              = "aaaaa";
    stock1->meta.instrumentLogo            = nullptr;
    stock1->meta.instrumentTicker          = "ABBA";
    stock1->meta.instrumentName            = "Abstract Basics";
    stock1->meta.forQualInvestorFlag       = false;
    stock1->operational.dayStartPrice      = 0.0f;
    stock1->operational.specifiedDatePrice = 0.0f;
    stock1->operational.turnover           = 1120;
    stock1->operational.payback            = 13.0f;
    stock1->meta.pricePrecision            = 2;

    StockOperationalData data1;
    data1.price = 100.0f;
    stock1->operational.detailedData.append(data1);

    stock2->meta.instrumentId              = "bbbbb";
    stock2->meta.instrumentLogo            = nullptr;
    stock2->meta.instrumentTicker          = "BASE";
    stock2->meta.instrumentName            = "Basketball enhancement";
    stock2->meta.forQualInvestorFlag       = true;
    stock2->operational.dayStartPrice      = 201.0f;
    stock2->operational.specifiedDatePrice = 202.0f;
    stock2->operational.turnover           = 2340000;
    stock2->operational.payback            = 33.0f;
    stock2->meta.pricePrecision            = 3;

    StockOperationalData data2;
    data2.price = 250.0f;
    stock2->operational.detailedData.append(data2);

    stock3->meta.instrumentId              = "ccccc";
    stock3->meta.instrumentLogo            = &logo;
    stock3->meta.instrumentTicker          = "CODE";
    stock3->meta.instrumentName            = "Code base";
    stock3->meta.forQualInvestorFlag       = false;
    stock3->operational.dayStartPrice      = 501.0f;
    stock3->operational.specifiedDatePrice = 502.0f;
    stock3->operational.turnover           = 5560000000;
    stock3->operational.payback            = 83.0f;
    stock3->meta.pricePrecision            = 4;

    StockOperationalData data3;
    data3.price = 500.0f;
    stock3->operational.detailedData.append(data3);

    stocks << stock1.get() << stock2.get() << stock3.get();

    EXPECT_CALL(*userStorageMock, isQualified()).WillOnce(Return(false));

    model->updateTable(stocks);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole),           QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole),           QVariant("100.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole),           QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole),           QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole),           QVariant("1.12K \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole),           QVariant("13.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole),           QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole),           QVariant("250.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole),           QVariant("+24.38%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole),           QVariant("+23.76%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole),           QVariant("2.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole),           QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole),           QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::DisplayRole),           QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::DisplayRole),           QVariant("500.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole),           QVariant("-0.20%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole),           QVariant("-0.40%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole),           QVariant("5.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole),           QVariant("83.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::ForegroundRole),        QVariant(QBrush(QColor("#97AEC4"))));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::ForegroundRole),        QVariant(QBrush(QColor("#97AEC4"))));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::ForegroundRole),        QVariant(QBrush(QColor("#ED6F7E"))));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::ForegroundRole),        QVariant(QBrush(QColor("#ED6F7E"))));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::ForegroundRole),        QVariant(QBrush(QColor("#2BD793"))));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::ForegroundRole),        QVariant(QBrush(QColor("#2BD793"))));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::ForegroundRole),        QVariant(QBrush(QColor("#97AEC4"))));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::ForegroundRole),        QVariant(QBrush(QColor("#97AEC4"))));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::ForegroundRole),        QVariant(QBrush(QColor("#ED6F7E"))));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::ForegroundRole),        QVariant(QBrush(QColor("#ED6F7E"))));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::ForegroundRole),        QVariant(QBrush(QColor("#2BD793"))));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::ForegroundRole),        QVariant(QBrush(QColor("#2BD793"))));
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN),     Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::ToolTipRole),           QVariant("From: 201.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::ToolTipRole),           QVariant("From: 202.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::ToolTipRole),           QVariant("From: 501.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::ToolTipRole),           QVariant("From: 502.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN),     Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        ROLE_INSTRUMENT_LOGO),      QVariant(0));
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        ROLE_INSTRUMENT_LOGO),      QVariant(0));
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        ROLE_INSTRUMENT_LOGO),      QVariant(reinterpret_cast<qint64>(&logo)));
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        ROLE_INSTRUMENT_NAME),      QVariant("Abstract Basics"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        ROLE_INSTRUMENT_NAME),      QVariant("Basketball enhancement"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        ROLE_INSTRUMENT_NAME),      QVariant("Code base"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        ROLE_INSTRUMENT_LOCKED),    QVariant(false));
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        ROLE_INSTRUMENT_LOCKED),    QVariant(true));
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        ROLE_INSTRUMENT_LOCKED),    QVariant(false));
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        ROLE_STOCK),                QVariant(reinterpret_cast<qint64>(stock1.get())));
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        ROLE_STOCK),                QVariant(reinterpret_cast<qint64>(stock2.get())));
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        ROLE_STOCK),                QVariant(reinterpret_cast<qint64>(stock3.get())));
    ASSERT_EQ(model->data(model->index(0, 0),                         Qt::WhatsThisPropertyRole), QVariant());
    // clang-format on
}

TEST_F(Test_StocksTableModel, Test_sort)
{
    const InSequence seq;

    Logo logo;

    QList<Stock*> stocks;

    std::shared_ptr<Stock> stock1 = std::make_shared<Stock>();
    std::shared_ptr<Stock> stock2 = std::make_shared<Stock>();
    std::shared_ptr<Stock> stock3 = std::make_shared<Stock>();

    stock1->meta.instrumentId              = "aaaaa";
    stock1->meta.instrumentLogo            = nullptr;
    stock1->meta.instrumentTicker          = "ABBA";
    stock1->meta.instrumentName            = "Abstract Basics";
    stock1->meta.forQualInvestorFlag       = false;
    stock1->operational.dayStartPrice      = 0.0f;
    stock1->operational.specifiedDatePrice = 0.0f;
    stock1->operational.turnover           = 1120;
    stock1->operational.payback            = 13.0f;
    stock1->meta.pricePrecision            = 2;

    StockOperationalData data1;
    data1.price = 100.0f;
    stock1->operational.detailedData.append(data1);

    stock2->meta.instrumentId              = "bbbbb";
    stock2->meta.instrumentLogo            = nullptr;
    stock2->meta.instrumentTicker          = "BASE";
    stock2->meta.instrumentName            = "Basketball enhancement";
    stock2->meta.forQualInvestorFlag       = true;
    stock2->operational.dayStartPrice      = 201.0f;
    stock2->operational.specifiedDatePrice = 202.0f;
    stock2->operational.turnover           = 2340000;
    stock2->operational.payback            = 33.0f;
    stock2->meta.pricePrecision            = 3;

    StockOperationalData data2;
    data2.price = 250.0f;
    stock2->operational.detailedData.append(data2);

    stock3->meta.instrumentId              = "ccccc";
    stock3->meta.instrumentLogo            = &logo;
    stock3->meta.instrumentTicker          = "CODE";
    stock3->meta.instrumentName            = "Code base";
    stock3->meta.forQualInvestorFlag       = false;
    stock3->operational.dayStartPrice      = 501.0f;
    stock3->operational.specifiedDatePrice = 502.0f;
    stock3->operational.turnover           = 5560000000;
    stock3->operational.payback            = 83.0f;
    stock3->meta.pricePrecision            = 4;

    StockOperationalData data3;
    data3.price = 500.0f;
    stock3->operational.detailedData.append(data3);

    stocks << stock1.get() << stock2.get() << stock3.get();

    EXPECT_CALL(*userStorageMock, isQualified()).WillOnce(Return(false));

    model->updateTable(stocks);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("100.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("1.12K \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("13.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("250.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+24.38%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+23.76%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("2.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("500.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-0.20%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-0.40%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("5.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("83.00%"));
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
}

TEST_F(Test_StocksTableModel, Test_setFilter)
{
    const InSequence seq;

    Logo logo;

    QList<Stock*> stocks;

    std::shared_ptr<Stock> stock1 = std::make_shared<Stock>();
    std::shared_ptr<Stock> stock2 = std::make_shared<Stock>();
    std::shared_ptr<Stock> stock3 = std::make_shared<Stock>();

    stock1->meta.instrumentId              = "aaaaa";
    stock1->meta.instrumentLogo            = nullptr;
    stock1->meta.instrumentTicker          = "ABBA";
    stock1->meta.instrumentName            = "Abstract Basics";
    stock1->meta.forQualInvestorFlag       = false;
    stock1->operational.dayStartPrice      = 0.0f;
    stock1->operational.specifiedDatePrice = 0.0f;
    stock1->operational.turnover           = 1120;
    stock1->operational.payback            = 13.0f;
    stock1->meta.pricePrecision            = 2;

    StockOperationalData data1;
    data1.price = 100.0f;
    stock1->operational.detailedData.append(data1);

    stock2->meta.instrumentId              = "bbbbb";
    stock2->meta.instrumentLogo            = nullptr;
    stock2->meta.instrumentTicker          = "BASE";
    stock2->meta.instrumentName            = "Basketball enhancement";
    stock2->meta.forQualInvestorFlag       = true;
    stock2->operational.dayStartPrice      = 201.0f;
    stock2->operational.specifiedDatePrice = 202.0f;
    stock2->operational.turnover           = 2340000;
    stock2->operational.payback            = 33.0f;
    stock2->meta.pricePrecision            = 3;

    StockOperationalData data2;
    data2.price = 250.0f;
    stock2->operational.detailedData.append(data2);

    stock3->meta.instrumentId              = "ccccc";
    stock3->meta.instrumentLogo            = &logo;
    stock3->meta.instrumentTicker          = "CODE";
    stock3->meta.instrumentName            = "Code base";
    stock3->meta.forQualInvestorFlag       = false;
    stock3->operational.dayStartPrice      = 501.0f;
    stock3->operational.specifiedDatePrice = 502.0f;
    stock3->operational.turnover           = 5560000000;
    stock3->operational.payback            = 83.0f;
    stock3->meta.pricePrecision            = 4;

    StockOperationalData data3;
    data3.price = 500.0f;
    stock3->operational.detailedData.append(data3);

    stocks << stock1.get() << stock2.get() << stock3.get();

    EXPECT_CALL(*userStorageMock, isQualified()).WillOnce(Return(false));

    model->updateTable(stocks);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("100.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("1.12K \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("13.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("250.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+24.38%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+23.76%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("2.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("500.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-0.20%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-0.40%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("5.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("83.00%"));
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
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on
}

TEST_F(Test_StocksTableModel, Test_updateAll)
{
    const InSequence seq;

    Logo logo;

    QList<Stock*> stocks;

    std::shared_ptr<Stock> stock1 = std::make_shared<Stock>();
    std::shared_ptr<Stock> stock2 = std::make_shared<Stock>();
    std::shared_ptr<Stock> stock3 = std::make_shared<Stock>();

    stock1->meta.instrumentId              = "aaaaa";
    stock1->meta.instrumentLogo            = nullptr;
    stock1->meta.instrumentTicker          = "ABBA";
    stock1->meta.instrumentName            = "Abstract Basics";
    stock1->meta.forQualInvestorFlag       = false;
    stock1->operational.dayStartPrice      = 0.0f;
    stock1->operational.specifiedDatePrice = 0.0f;
    stock1->operational.turnover           = 1120;
    stock1->operational.payback            = 13.0f;
    stock1->meta.pricePrecision            = 2;

    StockOperationalData data1;
    data1.price = 100.0f;
    stock1->operational.detailedData.append(data1);

    stock2->meta.instrumentId              = "bbbbb";
    stock2->meta.instrumentLogo            = nullptr;
    stock2->meta.instrumentTicker          = "BASE";
    stock2->meta.instrumentName            = "Basketball enhancement";
    stock2->meta.forQualInvestorFlag       = true;
    stock2->operational.dayStartPrice      = 201.0f;
    stock2->operational.specifiedDatePrice = 202.0f;
    stock2->operational.turnover           = 2340000;
    stock2->operational.payback            = 33.0f;
    stock2->meta.pricePrecision            = 3;

    StockOperationalData data2;
    data2.price = 250.0f;
    stock2->operational.detailedData.append(data2);

    stock3->meta.instrumentId              = "ccccc";
    stock3->meta.instrumentLogo            = &logo;
    stock3->meta.instrumentTicker          = "CODE";
    stock3->meta.instrumentName            = "Code base";
    stock3->meta.forQualInvestorFlag       = false;
    stock3->operational.dayStartPrice      = 501.0f;
    stock3->operational.specifiedDatePrice = 502.0f;
    stock3->operational.turnover           = 5560000000;
    stock3->operational.payback            = 83.0f;
    stock3->meta.pricePrecision            = 4;

    StockOperationalData data3;
    data3.price = 500.0f;
    stock3->operational.detailedData.append(data3);

    stocks << stock1.get() << stock2.get() << stock3.get();

    EXPECT_CALL(*userStorageMock, isQualified()).WillOnce(Return(false));

    model->updateTable(stocks);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("100.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("1.12K \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("13.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("250.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+24.38%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+23.76%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("2.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("500.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-0.20%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-0.40%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("5.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("83.00%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    // clang-format on

    // TODO: Finish
}

TEST_F(Test_StocksTableModel, Test_exportToExcel)
{
    const InSequence seq;

    Logo logo;

    QList<Stock*> stocks;

    std::shared_ptr<Stock> stock1 = std::make_shared<Stock>();
    std::shared_ptr<Stock> stock2 = std::make_shared<Stock>();
    std::shared_ptr<Stock> stock3 = std::make_shared<Stock>();

    stock1->meta.instrumentId              = "aaaaa";
    stock1->meta.instrumentLogo            = nullptr;
    stock1->meta.instrumentTicker          = "ABBA";
    stock1->meta.instrumentName            = "Abstract Basics";
    stock1->meta.forQualInvestorFlag       = false;
    stock1->operational.dayStartPrice      = 0.0f;
    stock1->operational.specifiedDatePrice = 0.0f;
    stock1->operational.turnover           = 1120;
    stock1->operational.payback            = 13.0f;
    stock1->meta.pricePrecision            = 2;

    StockOperationalData data1;
    data1.price = 100.0f;
    stock1->operational.detailedData.append(data1);

    stock2->meta.instrumentId              = "bbbbb";
    stock2->meta.instrumentLogo            = nullptr;
    stock2->meta.instrumentTicker          = "BASE";
    stock2->meta.instrumentName            = "Basketball enhancement";
    stock2->meta.forQualInvestorFlag       = true;
    stock2->operational.dayStartPrice      = 201.0f;
    stock2->operational.specifiedDatePrice = 202.0f;
    stock2->operational.turnover           = 2340000;
    stock2->operational.payback            = 33.0f;
    stock2->meta.pricePrecision            = 3;

    StockOperationalData data2;
    data2.price = 250.0f;
    stock2->operational.detailedData.append(data2);

    stock3->meta.instrumentId              = "ccccc";
    stock3->meta.instrumentLogo            = &logo;
    stock3->meta.instrumentTicker          = "CODE";
    stock3->meta.instrumentName            = "Code base";
    stock3->meta.forQualInvestorFlag       = false;
    stock3->operational.dayStartPrice      = 501.0f;
    stock3->operational.specifiedDatePrice = 502.0f;
    stock3->operational.turnover           = 5560000000;
    stock3->operational.payback            = 83.0f;
    stock3->meta.pricePrecision            = 4;

    StockOperationalData data3;
    data3.price = 500.0f;
    stock3->operational.detailedData.append(data3);

    stocks << stock1.get() << stock2.get() << stock3.get();

    EXPECT_CALL(*userStorageMock, isQualified()).WillOnce(Return(false));

    model->updateTable(stocks);
    ASSERT_EQ(model->rowCount(), 3);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("100.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("0.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("1.12K \u20BD"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("13.00%"));
    ASSERT_EQ(model->data(model->index(0, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("250.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("+24.38%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("+23.76%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("2.34M \u20BD"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("33.00%"));
    ASSERT_EQ(model->data(model->index(1, STOCKS_ACTIONS_COLUMN),     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(2, STOCKS_NAME_COLUMN),        Qt::DisplayRole), QVariant("CODE"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PRICE_COLUMN),       Qt::DisplayRole), QVariant("500.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DAY_CHANGE_COLUMN),  Qt::DisplayRole), QVariant("-0.20%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_DATE_CHANGE_COLUMN), Qt::DisplayRole), QVariant("-0.40%"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_TURNOVER_COLUMN),    Qt::DisplayRole), QVariant("5.56B \u20BD"));
    ASSERT_EQ(model->data(model->index(2, STOCKS_PAYBACK_COLUMN),     Qt::DisplayRole), QVariant("83.00%"));
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
    ASSERT_EQ(doc.cellAt(3, STOCKS_NAME_COLUMN + 1)->value(),                     QVariant("Basketball enhancement"));
    ASSERT_EQ(doc.cellAt(3, STOCKS_NAME_COLUMN + 2)->value(),                     QVariant(true));
    ASSERT_NEAR(doc.cellAt(3, STOCKS_PRICE_COLUMN + 2)->value().toDouble(),       250.0, 0.0001);
    ASSERT_NEAR(doc.cellAt(3, STOCKS_DAY_CHANGE_COLUMN + 2)->value().toDouble(),  0.24378, 0.0001);
    ASSERT_NEAR(doc.cellAt(3, STOCKS_DATE_CHANGE_COLUMN + 2)->value().toDouble(), 0.23762, 0.0001);
    ASSERT_EQ(doc.cellAt(3, STOCKS_TURNOVER_COLUMN + 2)->value(),                 QVariant(2340000));
    ASSERT_NEAR(doc.cellAt(3, STOCKS_PAYBACK_COLUMN + 2)->value().toDouble(),     0.33, 0.0001);
    ASSERT_EQ(doc.cellAt(4, STOCKS_NAME_COLUMN + 1)->value(),                     QVariant("Code base"));
    ASSERT_EQ(doc.cellAt(4, STOCKS_NAME_COLUMN + 2)->value(),                     QVariant(false));
    ASSERT_NEAR(doc.cellAt(4, STOCKS_PRICE_COLUMN + 2)->value().toDouble(),       500.0, 0.0001);
    ASSERT_NEAR(doc.cellAt(4, STOCKS_DAY_CHANGE_COLUMN + 2)->value().toDouble(),  -0.0019, 0.0001);
    ASSERT_NEAR(doc.cellAt(4, STOCKS_DATE_CHANGE_COLUMN + 2)->value().toDouble(), -0.00398, 0.0001);
    ASSERT_EQ(doc.cellAt(4, STOCKS_TURNOVER_COLUMN + 2)->value(),                 QVariant(5560000000));
    ASSERT_NEAR(doc.cellAt(4, STOCKS_PAYBACK_COLUMN + 2)->value().toDouble(),     0.83, 0.0001);
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
