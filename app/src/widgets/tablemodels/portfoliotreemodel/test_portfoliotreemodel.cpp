#include "src/widgets/tablemodels/portfoliotreemodel/portfoliotreemodel.h"

#include <QBrush>
#include <gtest/gtest.h>

#include "src/widgets/tablemodels/modelroles.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";



class Test_PortfolioTreeModel : public ::testing::Test
{
protected:
    void SetUp() override
    {
        model = new PortfolioTreeModel();
    }

    void TearDown() override
    {
        delete model;
    }

    PortfolioTreeModel* model;
};



TEST_F(Test_PortfolioTreeModel, Test_constructor_and_destructor)
{
}

TEST_F(Test_PortfolioTreeModel, Test_parent)
{
    Portfolio             portfolio;
    PortfolioCategoryItem category;

    category.id = 0;
    category.items.resize(2);

    portfolio.positions << category;

    model->portfolioChanged(portfolio);

    ASSERT_EQ(model->parent(QModelIndex()), QModelIndex());
    ASSERT_EQ(model->parent(model->index(0, 0)), QModelIndex());
    ASSERT_EQ(model->parent(model->index(0, 0, model->index(0, 0))), model->index(0, 0));
}

TEST_F(Test_PortfolioTreeModel, Test_rowCount)
{
    ASSERT_EQ(model->rowCount(), 0);

    Portfolio portfolio;

    portfolio.positions.resize(3);

    model->portfolioChanged(portfolio);
    ASSERT_EQ(model->rowCount(), 3);

    portfolio.positions.resize(5);

    model->portfolioChanged(portfolio);
    ASSERT_EQ(model->rowCount(), 5);

    portfolio.positions.clear();

    model->portfolioChanged(portfolio);
    ASSERT_EQ(model->rowCount(), 0);

    PortfolioCategoryItem category;

    category.id = 0;
    category.items.resize(2);

    portfolio.positions << category;

    model->portfolioChanged(portfolio);
    ASSERT_EQ(model->rowCount(model->index(0, 0)), 2);

    portfolio.positions[0].items.resize(6);

    model->portfolioChanged(portfolio);
    ASSERT_EQ(model->rowCount(model->index(0, 0)), 6);

    ASSERT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 0);
}

TEST_F(Test_PortfolioTreeModel, Test_columnCount)
{
    ASSERT_EQ(model->columnCount(), PORTFOLIO_COLUMN_COUNT);
}

TEST_F(Test_PortfolioTreeModel, Test_headerData)
{
    // clang-format off
    ASSERT_EQ(model->headerData(PORTFOLIO_NAME_COLUMN,          Qt::Horizontal, Qt::DisplayRole),           QVariant("Name"));
    ASSERT_EQ(model->headerData(PORTFOLIO_AVAILABLE_COLUMN,     Qt::Horizontal, Qt::DisplayRole),           QVariant("Available"));
    ASSERT_EQ(model->headerData(PORTFOLIO_PRICE_COLUMN,         Qt::Horizontal, Qt::DisplayRole),           QVariant("Price"));
    ASSERT_EQ(model->headerData(PORTFOLIO_AVG_PRICE_COLUMN,     Qt::Horizontal, Qt::DisplayRole),           QVariant("Avg price"));
    ASSERT_EQ(model->headerData(PORTFOLIO_COST_COLUMN,          Qt::Horizontal, Qt::DisplayRole),           QVariant("Cost"));
    ASSERT_EQ(model->headerData(PORTFOLIO_PART_COLUMN,          Qt::Horizontal, Qt::DisplayRole),           QVariant("Part"));
    ASSERT_EQ(model->headerData(PORTFOLIO_YIELD_COLUMN,         Qt::Horizontal, Qt::DisplayRole),           QVariant("Yield"));
    ASSERT_EQ(model->headerData(PORTFOLIO_YIELD_PERCENT_COLUMN, Qt::Horizontal, Qt::DisplayRole),           QVariant("Yield, %"));
    ASSERT_EQ(model->headerData(PORTFOLIO_DAILY_YIELD_COLUMN,   Qt::Horizontal, Qt::DisplayRole),           QVariant("Daily yield, %"));
    ASSERT_EQ(model->headerData(0,                              Qt::Vertical,   Qt::DisplayRole),           QVariant(1));
    ASSERT_EQ(model->headerData(1,                              Qt::Vertical,   Qt::DisplayRole),           QVariant(2));
    ASSERT_EQ(model->headerData(2,                              Qt::Vertical,   Qt::DisplayRole),           QVariant(3));
    ASSERT_EQ(model->headerData(PORTFOLIO_NAME_COLUMN,          Qt::Horizontal, Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->headerData(PORTFOLIO_AVAILABLE_COLUMN,     Qt::Horizontal, Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->headerData(PORTFOLIO_PRICE_COLUMN,         Qt::Horizontal, Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->headerData(PORTFOLIO_AVG_PRICE_COLUMN,     Qt::Horizontal, Qt::ToolTipRole),           QVariant("Average price by FIFO"));
    ASSERT_EQ(model->headerData(PORTFOLIO_COST_COLUMN,          Qt::Horizontal, Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->headerData(PORTFOLIO_PART_COLUMN,          Qt::Horizontal, Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->headerData(PORTFOLIO_YIELD_COLUMN,         Qt::Horizontal, Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->headerData(PORTFOLIO_YIELD_PERCENT_COLUMN, Qt::Horizontal, Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->headerData(PORTFOLIO_DAILY_YIELD_COLUMN,   Qt::Horizontal, Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->headerData(PORTFOLIO_NAME_COLUMN,          Qt::Horizontal, Qt::DecorationRole),        QVariant());
    ASSERT_EQ(model->headerData(PORTFOLIO_AVAILABLE_COLUMN,     Qt::Horizontal, Qt::DecorationRole),        QVariant());
    ASSERT_EQ(model->headerData(PORTFOLIO_PRICE_COLUMN,         Qt::Horizontal, Qt::DecorationRole),        QVariant());
    ASSERT_NE(model->headerData(PORTFOLIO_AVG_PRICE_COLUMN,     Qt::Horizontal, Qt::DecorationRole),        QVariant());
    ASSERT_EQ(model->headerData(PORTFOLIO_COST_COLUMN,          Qt::Horizontal, Qt::DecorationRole),        QVariant());
    ASSERT_EQ(model->headerData(PORTFOLIO_PART_COLUMN,          Qt::Horizontal, Qt::DecorationRole),        QVariant());
    ASSERT_EQ(model->headerData(PORTFOLIO_YIELD_COLUMN,         Qt::Horizontal, Qt::DecorationRole),        QVariant());
    ASSERT_EQ(model->headerData(PORTFOLIO_YIELD_PERCENT_COLUMN, Qt::Horizontal, Qt::DecorationRole),        QVariant());
    ASSERT_EQ(model->headerData(PORTFOLIO_DAILY_YIELD_COLUMN,   Qt::Horizontal, Qt::DecorationRole),        QVariant());
    ASSERT_EQ(model->headerData(0,                              Qt::Horizontal, Qt::WhatsThisPropertyRole), QVariant());
    // clang-format on
}

TEST_F(Test_PortfolioTreeModel, Test_data)
{
    Logo logo;

    Portfolio             portfolio;
    PortfolioCategoryItem category1;
    PortfolioCategoryItem category2;
    PortfolioItem         item1;
    PortfolioItem         item2;
    PortfolioItem         item3;

    item1.instrumentId       = RUBLE_UID;
    item1.instrumentLogo     = nullptr;
    item1.instrumentTicker   = "RUBLE";
    item1.instrumentName     = "Ruble";
    item1.showPrices         = false;
    item1.available          = 100000.12;
    item1.price              = 1.0f;
    item1.avgPriceFifo       = 1.0f;
    item1.avgPriceWavg       = 1.0f;
    item1.cost               = 100000.0;
    item1.part               = 10.0;
    item1.yield              = 0.0f;
    item1.yieldPercent       = 0.0f;
    item1.dailyYield         = 0.0f;
    item1.priceForDailyYield = 0.0f;
    item1.costForDailyYield  = 0.0;
    item1.dailyYieldPercent  = 0.0f;
    item1.pricePrecision     = 2;

    item2.instrumentId       = "aaaaa";
    item2.instrumentLogo     = nullptr;
    item2.instrumentTicker   = "ABBA";
    item2.instrumentName     = "Abstract Basics";
    item2.showPrices         = true;
    item2.available          = 100.0;
    item2.price              = 101.0f;
    item2.avgPriceFifo       = 102.0f;
    item2.avgPriceWavg       = 103.0f;
    item2.cost               = 400000.0;
    item2.part               = 40.0;
    item2.yield              = -105.0f;
    item2.yieldPercent       = -106.0f;
    item2.dailyYield         = 107.0f;
    item2.priceForDailyYield = 108.0f;
    item2.costForDailyYield  = 109.0;
    item2.dailyYieldPercent  = -110.0f;
    item2.pricePrecision     = 3;

    item3.instrumentId       = "bbbbb";
    item3.instrumentLogo     = &logo;
    item3.instrumentTicker   = "BASE";
    item3.instrumentName     = "Basketball enhancement";
    item3.showPrices         = true;
    item3.available          = 200.0;
    item3.price              = 201.0f;
    item3.avgPriceFifo       = 202.0f;
    item3.avgPriceWavg       = 203.0f;
    item3.cost               = 500000.0;
    item3.part               = 50.0;
    item3.yield              = 205.0f;
    item3.yieldPercent       = 206.0f;
    item3.dailyYield         = 207.0f;
    item3.priceForDailyYield = 208.0f;
    item3.costForDailyYield  = 209.0;
    item3.dailyYieldPercent  = 210.0f;
    item3.pricePrecision     = 4;

    category1.id   = 0;
    category1.name = "Currency and metals";
    category1.cost = 100000.0;
    category1.part = 10.0;
    category1.items << item1;

    category2.id   = 1;
    category2.name = "Share";
    category2.cost = 900000.0;
    category2.part = 90.0;
    category2.items << item2 << item3;

    portfolio.positions << category1 << category2;

    model->portfolioChanged(portfolio);
    ASSERT_EQ(model->rowCount(), 2);
    ASSERT_EQ(model->rowCount(model->index(0, 0)), 1);
    ASSERT_EQ(model->rowCount(model->index(1, 0)), 2);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN),                              Qt::DisplayRole),           QVariant("Currency and metals"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN),                         Qt::DisplayRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN),                             Qt::DisplayRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN),                         Qt::DisplayRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_COST_COLUMN),                              Qt::DisplayRole),           QVariant("100000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN),                              Qt::DisplayRole),           QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN),                             Qt::DisplayRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN),                     Qt::DisplayRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN),                       Qt::DisplayRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_NAME_COLUMN),                              Qt::DisplayRole),           QVariant("Share"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVAILABLE_COLUMN),                         Qt::DisplayRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PRICE_COLUMN),                             Qt::DisplayRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVG_PRICE_COLUMN),                         Qt::DisplayRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_COST_COLUMN),                              Qt::DisplayRole),           QVariant("900000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PART_COLUMN),                              Qt::DisplayRole),           QVariant("90.00%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_COLUMN),                             Qt::DisplayRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_PERCENT_COLUMN),                     Qt::DisplayRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_DAILY_YIELD_COLUMN),                       Qt::DisplayRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN,          model->index(0, 0)), Qt::DisplayRole),           QVariant("RUBLE"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN,     model->index(0, 0)), Qt::DisplayRole),           QVariant("100000.12"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN,         model->index(0, 0)), Qt::DisplayRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN,     model->index(0, 0)), Qt::DisplayRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_COST_COLUMN,          model->index(0, 0)), Qt::DisplayRole),           QVariant("100000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN,          model->index(0, 0)), Qt::DisplayRole),           QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN,         model->index(0, 0)), Qt::DisplayRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(0, 0)), Qt::DisplayRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN,   model->index(0, 0)), Qt::DisplayRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN,          model->index(1, 0)), Qt::DisplayRole),           QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN,     model->index(1, 0)), Qt::DisplayRole),           QVariant("100"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN,         model->index(1, 0)), Qt::DisplayRole),           QVariant("101.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN,     model->index(1, 0)), Qt::DisplayRole),           QVariant("102.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_COST_COLUMN,          model->index(1, 0)), Qt::DisplayRole),           QVariant("400000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN,          model->index(1, 0)), Qt::DisplayRole),           QVariant("40.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN,         model->index(1, 0)), Qt::DisplayRole),           QVariant("-105.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(1, 0)), Qt::DisplayRole),           QVariant("-106.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN,   model->index(1, 0)), Qt::DisplayRole),           QVariant("-110.00%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_NAME_COLUMN,          model->index(1, 0)), Qt::DisplayRole),           QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVAILABLE_COLUMN,     model->index(1, 0)), Qt::DisplayRole),           QVariant("200"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PRICE_COLUMN,         model->index(1, 0)), Qt::DisplayRole),           QVariant("201.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVG_PRICE_COLUMN,     model->index(1, 0)), Qt::DisplayRole),           QVariant("202.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_COST_COLUMN,          model->index(1, 0)), Qt::DisplayRole),           QVariant("500000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PART_COLUMN,          model->index(1, 0)), Qt::DisplayRole),           QVariant("50.00%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_COLUMN,         model->index(1, 0)), Qt::DisplayRole),           QVariant("+205.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(1, 0)), Qt::DisplayRole),           QVariant("+206.00%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_DAILY_YIELD_COLUMN,   model->index(1, 0)), Qt::DisplayRole),           QVariant("+210.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN),                              Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN),                         Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN),                             Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN),                         Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_COST_COLUMN),                              Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN),                              Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN),                             Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN),                     Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN),                       Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_NAME_COLUMN),                              Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVAILABLE_COLUMN),                         Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PRICE_COLUMN),                             Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVG_PRICE_COLUMN),                         Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_COST_COLUMN),                              Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PART_COLUMN),                              Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_COLUMN),                             Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_PERCENT_COLUMN),                     Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_DAILY_YIELD_COLUMN),                       Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN,          model->index(0, 0)), Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN,     model->index(0, 0)), Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN,         model->index(0, 0)), Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN,     model->index(0, 0)), Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_COST_COLUMN,          model->index(0, 0)), Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN,          model->index(0, 0)), Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN,         model->index(0, 0)), Qt::ForegroundRole),        QVariant(QBrush(QColor("#97AEC4"))));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(0, 0)), Qt::ForegroundRole),        QVariant(QBrush(QColor("#97AEC4"))));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN,   model->index(0, 0)), Qt::ForegroundRole),        QVariant(QBrush(QColor("#97AEC4"))));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN,          model->index(1, 0)), Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN,     model->index(1, 0)), Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN,         model->index(1, 0)), Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN,     model->index(1, 0)), Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_COST_COLUMN,          model->index(1, 0)), Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN,          model->index(1, 0)), Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN,         model->index(1, 0)), Qt::ForegroundRole),        QVariant(QBrush(QColor("#ED6F7E"))));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(1, 0)), Qt::ForegroundRole),        QVariant(QBrush(QColor("#ED6F7E"))));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN,   model->index(1, 0)), Qt::ForegroundRole),        QVariant(QBrush(QColor("#ED6F7E"))));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_NAME_COLUMN,          model->index(1, 0)), Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVAILABLE_COLUMN,     model->index(1, 0)), Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PRICE_COLUMN,         model->index(1, 0)), Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVG_PRICE_COLUMN,     model->index(1, 0)), Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_COST_COLUMN,          model->index(1, 0)), Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PART_COLUMN,          model->index(1, 0)), Qt::ForegroundRole),        QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_COLUMN,         model->index(1, 0)), Qt::ForegroundRole),        QVariant(QBrush(QColor("#2BD793"))));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(1, 0)), Qt::ForegroundRole),        QVariant(QBrush(QColor("#2BD793"))));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_DAILY_YIELD_COLUMN,   model->index(1, 0)), Qt::ForegroundRole),        QVariant(QBrush(QColor("#2BD793"))));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN),                              Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN),                         Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN),                             Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN),                         Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_COST_COLUMN),                              Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN),                              Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN),                             Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN),                     Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN),                       Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_NAME_COLUMN),                              Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVAILABLE_COLUMN),                         Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PRICE_COLUMN),                             Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVG_PRICE_COLUMN),                         Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_COST_COLUMN),                              Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PART_COLUMN),                              Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_COLUMN),                             Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_PERCENT_COLUMN),                     Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_DAILY_YIELD_COLUMN),                       Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN,          model->index(0, 0)), Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN,     model->index(0, 0)), Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN,         model->index(0, 0)), Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN,     model->index(0, 0)), Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_COST_COLUMN,          model->index(0, 0)), Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN,          model->index(0, 0)), Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN,         model->index(0, 0)), Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(0, 0)), Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN,   model->index(0, 0)), Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN,          model->index(1, 0)), Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN,     model->index(1, 0)), Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN,         model->index(1, 0)), Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN,     model->index(1, 0)), Qt::ToolTipRole),           QVariant("Average price by WAVG: 103.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_COST_COLUMN,          model->index(1, 0)), Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN,          model->index(1, 0)), Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN,         model->index(1, 0)), Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(1, 0)), Qt::ToolTipRole),           QVariant("From: 102.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN,   model->index(1, 0)), Qt::ToolTipRole),           QVariant("From: 108.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_NAME_COLUMN,          model->index(1, 0)), Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVAILABLE_COLUMN,     model->index(1, 0)), Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PRICE_COLUMN,         model->index(1, 0)), Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVG_PRICE_COLUMN,     model->index(1, 0)), Qt::ToolTipRole),           QVariant("Average price by WAVG: 203.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_COST_COLUMN,          model->index(1, 0)), Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PART_COLUMN,          model->index(1, 0)), Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_COLUMN,         model->index(1, 0)), Qt::ToolTipRole),           QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(1, 0)), Qt::ToolTipRole),           QVariant("From: 202.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_DAILY_YIELD_COLUMN,   model->index(1, 0)), Qt::ToolTipRole),           QVariant("From: 208.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN),                              ROLE_INSTRUMENT_LOGO),      QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_NAME_COLUMN),                              ROLE_INSTRUMENT_LOGO),      QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN,          model->index(0, 0)), ROLE_INSTRUMENT_LOGO),      QVariant(0));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN,          model->index(1, 0)), ROLE_INSTRUMENT_LOGO),      QVariant(0));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_NAME_COLUMN,          model->index(1, 0)), ROLE_INSTRUMENT_LOGO),      QVariant(reinterpret_cast<qint64>(&logo)));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN),                              ROLE_INSTRUMENT_NAME),      QVariant("Currency and metals"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_NAME_COLUMN),                              ROLE_INSTRUMENT_NAME),      QVariant("Share"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN,          model->index(0, 0)), ROLE_INSTRUMENT_NAME),      QVariant("Ruble"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN,          model->index(1, 0)), ROLE_INSTRUMENT_NAME),      QVariant("Abstract Basics"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_NAME_COLUMN,          model->index(1, 0)), ROLE_INSTRUMENT_NAME),      QVariant("Basketball enhancement"));
    ASSERT_EQ(model->data(model->index(0, 0),                                                  Qt::WhatsThisPropertyRole), QVariant());
    // clang-format on
}

TEST_F(Test_PortfolioTreeModel, Test_sort)
{
    Logo logo;

    Portfolio             portfolio;
    PortfolioCategoryItem category1;
    PortfolioCategoryItem category2;
    PortfolioItem         item1;
    PortfolioItem         item2;
    PortfolioItem         item3;

    item1.instrumentId       = RUBLE_UID;
    item1.instrumentLogo     = nullptr;
    item1.instrumentTicker   = "RUBLE";
    item1.instrumentName     = "Ruble";
    item1.showPrices         = false;
    item1.available          = 100000.12;
    item1.price              = 1.0f;
    item1.avgPriceFifo       = 1.0f;
    item1.avgPriceWavg       = 1.0f;
    item1.cost               = 100000.0;
    item1.part               = 10.0;
    item1.yield              = 0.0f;
    item1.yieldPercent       = 0.0f;
    item1.dailyYield         = 0.0f;
    item1.priceForDailyYield = 0.0f;
    item1.costForDailyYield  = 0.0;
    item1.dailyYieldPercent  = 0.0f;
    item1.pricePrecision     = 2;

    item2.instrumentId       = "aaaaa";
    item2.instrumentLogo     = nullptr;
    item2.instrumentTicker   = "ABBA";
    item2.instrumentName     = "Abstract Basics";
    item2.showPrices         = true;
    item2.available          = 100.0;
    item2.price              = 101.0f;
    item2.avgPriceFifo       = 102.0f;
    item2.avgPriceWavg       = 103.0f;
    item2.cost               = 400000.0;
    item2.part               = 40.0;
    item2.yield              = -105.0f;
    item2.yieldPercent       = -106.0f;
    item2.dailyYield         = 107.0f;
    item2.priceForDailyYield = 108.0f;
    item2.costForDailyYield  = 109.0;
    item2.dailyYieldPercent  = -110.0f;
    item2.pricePrecision     = 3;

    item3.instrumentId       = "bbbbb";
    item3.instrumentLogo     = &logo;
    item3.instrumentTicker   = "BASE";
    item3.instrumentName     = "Basketball enhancement";
    item3.showPrices         = true;
    item3.available          = 200.0;
    item3.price              = 201.0f;
    item3.avgPriceFifo       = 202.0f;
    item3.avgPriceWavg       = 203.0f;
    item3.cost               = 500000.0;
    item3.part               = 50.0;
    item3.yield              = 205.0f;
    item3.yieldPercent       = 206.0f;
    item3.dailyYield         = 207.0f;
    item3.priceForDailyYield = 208.0f;
    item3.costForDailyYield  = 209.0;
    item3.dailyYieldPercent  = 210.0f;
    item3.pricePrecision     = 4;

    category1.id   = 0;
    category1.name = "Currency and metals";
    category1.cost = 100000.0;
    category1.part = 10.0;
    category1.items << item1;

    category2.id   = 1;
    category2.name = "Share";
    category2.cost = 900000.0;
    category2.part = 90.0;
    category2.items << item2 << item3;

    portfolio.positions << category1 << category2;

    model->portfolioChanged(portfolio);
    ASSERT_EQ(model->rowCount(), 2);
    ASSERT_EQ(model->rowCount(model->index(0, 0)), 1);
    ASSERT_EQ(model->rowCount(model->index(1, 0)), 2);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN),                              Qt::DisplayRole), QVariant("Currency and metals"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN),                         Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN),                             Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN),                         Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_COST_COLUMN),                              Qt::DisplayRole), QVariant("100000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN),                              Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN),                             Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN),                     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN),                       Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_NAME_COLUMN),                              Qt::DisplayRole), QVariant("Share"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVAILABLE_COLUMN),                         Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PRICE_COLUMN),                             Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVG_PRICE_COLUMN),                         Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_COST_COLUMN),                              Qt::DisplayRole), QVariant("900000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PART_COLUMN),                              Qt::DisplayRole), QVariant("90.00%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_COLUMN),                             Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_PERCENT_COLUMN),                     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_DAILY_YIELD_COLUMN),                       Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN,          model->index(0, 0)), Qt::DisplayRole), QVariant("RUBLE"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN,     model->index(0, 0)), Qt::DisplayRole), QVariant("100000.12"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN,         model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN,     model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_COST_COLUMN,          model->index(0, 0)), Qt::DisplayRole), QVariant("100000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN,          model->index(0, 0)), Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN,         model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN,   model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN,     model->index(1, 0)), Qt::DisplayRole), QVariant("100"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN,         model->index(1, 0)), Qt::DisplayRole), QVariant("101.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN,     model->index(1, 0)), Qt::DisplayRole), QVariant("102.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_COST_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("400000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("40.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN,         model->index(1, 0)), Qt::DisplayRole), QVariant("-105.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("-106.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN,   model->index(1, 0)), Qt::DisplayRole), QVariant("-110.00%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_NAME_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVAILABLE_COLUMN,     model->index(1, 0)), Qt::DisplayRole), QVariant("200"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PRICE_COLUMN,         model->index(1, 0)), Qt::DisplayRole), QVariant("201.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVG_PRICE_COLUMN,     model->index(1, 0)), Qt::DisplayRole), QVariant("202.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_COST_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("500000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PART_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("50.00%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_COLUMN,         model->index(1, 0)), Qt::DisplayRole), QVariant("+205.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("+206.00%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_DAILY_YIELD_COLUMN,   model->index(1, 0)), Qt::DisplayRole), QVariant("+210.00%"));
    // clang-format on

    model->sort(PORTFOLIO_NAME_COLUMN, Qt::AscendingOrder);

    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN), Qt::DisplayRole), QVariant("Currency and metals"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_NAME_COLUMN), Qt::DisplayRole), QVariant("Share"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant("RUBLE"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_NAME_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("BASE"));

    model->sort(PORTFOLIO_NAME_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN), Qt::DisplayRole), QVariant("Currency and metals"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_NAME_COLUMN), Qt::DisplayRole), QVariant("Share"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant("RUBLE"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_NAME_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("ABBA"));

    model->sort(PORTFOLIO_AVAILABLE_COLUMN, Qt::AscendingOrder);

    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVAILABLE_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(
        model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant("100000.12")
    );
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("100"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVAILABLE_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("200"));

    model->sort(PORTFOLIO_AVAILABLE_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVAILABLE_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(
        model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant("100000.12")
    );
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("200"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVAILABLE_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("100"));

    model->sort(PORTFOLIO_PRICE_COLUMN, Qt::AscendingOrder);

    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PRICE_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(
        model->data(model->index(0, PORTFOLIO_PRICE_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("101.000 \u20BD")
    );
    ASSERT_EQ(
        model->data(model->index(1, PORTFOLIO_PRICE_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("201.0000 \u20BD")
    );

    model->sort(PORTFOLIO_PRICE_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PRICE_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(
        model->data(model->index(0, PORTFOLIO_PRICE_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("201.0000 \u20BD")
    );
    ASSERT_EQ(
        model->data(model->index(1, PORTFOLIO_PRICE_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("101.000 \u20BD")
    );

    model->sort(PORTFOLIO_AVG_PRICE_COLUMN, Qt::AscendingOrder);

    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVG_PRICE_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(
        model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("102.000 \u20BD")
    );
    ASSERT_EQ(
        model->data(model->index(1, PORTFOLIO_AVG_PRICE_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("202.0000 \u20BD")
    );

    model->sort(PORTFOLIO_AVG_PRICE_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVG_PRICE_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(
        model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("202.0000 \u20BD")
    );
    ASSERT_EQ(
        model->data(model->index(1, PORTFOLIO_AVG_PRICE_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("102.000 \u20BD")
    );

    model->sort(PORTFOLIO_COST_COLUMN, Qt::AscendingOrder);

    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_COST_COLUMN), Qt::DisplayRole), QVariant("100000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_COST_COLUMN), Qt::DisplayRole), QVariant("900000.00 \u20BD"));
    ASSERT_EQ(
        model->data(model->index(0, PORTFOLIO_COST_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant("100000.00 \u20BD")
    );
    ASSERT_EQ(
        model->data(model->index(0, PORTFOLIO_COST_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("400000.00 \u20BD")
    );
    ASSERT_EQ(
        model->data(model->index(1, PORTFOLIO_COST_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("500000.00 \u20BD")
    );

    model->sort(PORTFOLIO_COST_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_COST_COLUMN), Qt::DisplayRole), QVariant("100000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_COST_COLUMN), Qt::DisplayRole), QVariant("900000.00 \u20BD"));
    ASSERT_EQ(
        model->data(model->index(0, PORTFOLIO_COST_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant("100000.00 \u20BD")
    );
    ASSERT_EQ(
        model->data(model->index(0, PORTFOLIO_COST_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("500000.00 \u20BD")
    );
    ASSERT_EQ(
        model->data(model->index(1, PORTFOLIO_COST_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("400000.00 \u20BD")
    );

    model->sort(PORTFOLIO_PART_COLUMN, Qt::AscendingOrder);

    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN), Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PART_COLUMN), Qt::DisplayRole), QVariant("90.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("40.00%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PART_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("50.00%"));

    model->sort(PORTFOLIO_PART_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN), Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PART_COLUMN), Qt::DisplayRole), QVariant("90.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("50.00%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PART_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("40.00%"));

    model->sort(PORTFOLIO_YIELD_COLUMN, Qt::AscendingOrder);

    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(
        model->data(model->index(0, PORTFOLIO_YIELD_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("-105.00 \u20BD")
    );
    ASSERT_EQ(
        model->data(model->index(1, PORTFOLIO_YIELD_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("+205.00 \u20BD")
    );

    model->sort(PORTFOLIO_YIELD_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(
        model->data(model->index(0, PORTFOLIO_YIELD_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("+205.00 \u20BD")
    );
    ASSERT_EQ(
        model->data(model->index(1, PORTFOLIO_YIELD_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("-105.00 \u20BD")
    );

    model->sort(PORTFOLIO_YIELD_PERCENT_COLUMN, Qt::AscendingOrder);

    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_PERCENT_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(
        model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("-106.00%")
    );
    ASSERT_EQ(
        model->data(model->index(1, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("+206.00%")
    );

    model->sort(PORTFOLIO_YIELD_PERCENT_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_PERCENT_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(
        model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("+206.00%")
    );
    ASSERT_EQ(
        model->data(model->index(1, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("-106.00%")
    );

    model->sort(PORTFOLIO_DAILY_YIELD_COLUMN, Qt::AscendingOrder);

    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_DAILY_YIELD_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(
        model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("-110.00%")
    );
    ASSERT_EQ(
        model->data(model->index(1, PORTFOLIO_DAILY_YIELD_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("+210.00%")
    );

    model->sort(PORTFOLIO_DAILY_YIELD_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_DAILY_YIELD_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(
        model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("+210.00%")
    );
    ASSERT_EQ(
        model->data(model->index(1, PORTFOLIO_DAILY_YIELD_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("-110.00%")
    );

    model->sort(PORTFOLIO_NAME_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN), Qt::DisplayRole), QVariant("Currency and metals"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_NAME_COLUMN), Qt::DisplayRole), QVariant("Share"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant("RUBLE"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_NAME_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("ABBA"));

    model->sort(PORTFOLIO_AVAILABLE_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVAILABLE_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(
        model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant("100000.12")
    );
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("200"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVAILABLE_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("100"));

    model->sort(PORTFOLIO_PRICE_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PRICE_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(
        model->data(model->index(0, PORTFOLIO_PRICE_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("201.0000 \u20BD")
    );
    ASSERT_EQ(
        model->data(model->index(1, PORTFOLIO_PRICE_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("101.000 \u20BD")
    );

    model->sort(PORTFOLIO_AVG_PRICE_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVG_PRICE_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(
        model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("202.0000 \u20BD")
    );
    ASSERT_EQ(
        model->data(model->index(1, PORTFOLIO_AVG_PRICE_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("102.000 \u20BD")
    );

    model->sort(PORTFOLIO_COST_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_COST_COLUMN), Qt::DisplayRole), QVariant("100000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_COST_COLUMN), Qt::DisplayRole), QVariant("900000.00 \u20BD"));
    ASSERT_EQ(
        model->data(model->index(0, PORTFOLIO_COST_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant("100000.00 \u20BD")
    );
    ASSERT_EQ(
        model->data(model->index(0, PORTFOLIO_COST_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("500000.00 \u20BD")
    );
    ASSERT_EQ(
        model->data(model->index(1, PORTFOLIO_COST_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("400000.00 \u20BD")
    );

    model->sort(PORTFOLIO_PART_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN), Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PART_COLUMN), Qt::DisplayRole), QVariant("90.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("50.00%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PART_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("40.00%"));

    model->sort(PORTFOLIO_YIELD_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(
        model->data(model->index(0, PORTFOLIO_YIELD_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("+205.00 \u20BD")
    );
    ASSERT_EQ(
        model->data(model->index(1, PORTFOLIO_YIELD_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("-105.00 \u20BD")
    );

    model->sort(PORTFOLIO_YIELD_PERCENT_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_PERCENT_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(
        model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("+206.00%")
    );
    ASSERT_EQ(
        model->data(model->index(1, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("-106.00%")
    );

    model->sort(PORTFOLIO_DAILY_YIELD_COLUMN, Qt::DescendingOrder);

    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_DAILY_YIELD_COLUMN), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(
        model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("+210.00%")
    );
    ASSERT_EQ(
        model->data(model->index(1, PORTFOLIO_DAILY_YIELD_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("-110.00%")
    );
}

TEST_F(Test_PortfolioTreeModel, Test_updateLastPrices)
{
    Logo logo;

    Portfolio             portfolio;
    PortfolioCategoryItem category1;
    PortfolioCategoryItem category2;
    PortfolioItem         item1;
    PortfolioItem         item2;
    PortfolioItem         item3;

    item1.instrumentId       = RUBLE_UID;
    item1.instrumentLogo     = nullptr;
    item1.instrumentTicker   = "RUBLE";
    item1.instrumentName     = "Ruble";
    item1.showPrices         = false;
    item1.available          = 100000.12;
    item1.price              = 1.0f;
    item1.avgPriceFifo       = 1.0f;
    item1.avgPriceWavg       = 1.0f;
    item1.cost               = 100000.0;
    item1.part               = 10.0;
    item1.yield              = 0.0f;
    item1.yieldPercent       = 0.0f;
    item1.dailyYield         = 0.0f;
    item1.priceForDailyYield = 0.0f;
    item1.costForDailyYield  = 0.0;
    item1.dailyYieldPercent  = 0.0f;
    item1.pricePrecision     = 2;

    item2.instrumentId       = "aaaaa";
    item2.instrumentLogo     = nullptr;
    item2.instrumentTicker   = "ABBA";
    item2.instrumentName     = "Abstract Basics";
    item2.showPrices         = true;
    item2.available          = 100.0;
    item2.price              = 101.0f;
    item2.avgPriceFifo       = 102.0f;
    item2.avgPriceWavg       = 103.0f;
    item2.cost               = 400000.0;
    item2.part               = 40.0;
    item2.yield              = -105.0f;
    item2.yieldPercent       = -106.0f;
    item2.dailyYield         = 107.0f;
    item2.priceForDailyYield = 108.0f;
    item2.costForDailyYield  = 109.0;
    item2.dailyYieldPercent  = -110.0f;
    item2.pricePrecision     = 3;

    item3.instrumentId       = "bbbbb";
    item3.instrumentLogo     = &logo;
    item3.instrumentTicker   = "BASE";
    item3.instrumentName     = "Basketball enhancement";
    item3.showPrices         = true;
    item3.available          = 200.0;
    item3.price              = 201.0f;
    item3.avgPriceFifo       = 202.0f;
    item3.avgPriceWavg       = 203.0f;
    item3.cost               = 500000.0;
    item3.part               = 50.0;
    item3.yield              = 205.0f;
    item3.yieldPercent       = 206.0f;
    item3.dailyYield         = 207.0f;
    item3.priceForDailyYield = 208.0f;
    item3.costForDailyYield  = 209.0;
    item3.dailyYieldPercent  = 210.0f;
    item3.pricePrecision     = 4;

    category1.id   = 0;
    category1.name = "Currency and metals";
    category1.cost = 100000.0;
    category1.part = 10.0;
    category1.items << item1;

    category2.id   = 1;
    category2.name = "Share";
    category2.cost = 900000.0;
    category2.part = 90.0;
    category2.items << item2 << item3;

    portfolio.positions << category1 << category2;

    model->portfolioChanged(portfolio);
    ASSERT_EQ(model->rowCount(), 2);
    ASSERT_EQ(model->rowCount(model->index(0, 0)), 1);
    ASSERT_EQ(model->rowCount(model->index(1, 0)), 2);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN),                              Qt::DisplayRole), QVariant("Currency and metals"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN),                         Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN),                             Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN),                         Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_COST_COLUMN),                              Qt::DisplayRole), QVariant("100000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN),                              Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN),                             Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN),                     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN),                       Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_NAME_COLUMN),                              Qt::DisplayRole), QVariant("Share"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVAILABLE_COLUMN),                         Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PRICE_COLUMN),                             Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVG_PRICE_COLUMN),                         Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_COST_COLUMN),                              Qt::DisplayRole), QVariant("900000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PART_COLUMN),                              Qt::DisplayRole), QVariant("90.00%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_COLUMN),                             Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_PERCENT_COLUMN),                     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_DAILY_YIELD_COLUMN),                       Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN,          model->index(0, 0)), Qt::DisplayRole), QVariant("RUBLE"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN,     model->index(0, 0)), Qt::DisplayRole), QVariant("100000.12"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN,         model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN,     model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_COST_COLUMN,          model->index(0, 0)), Qt::DisplayRole), QVariant("100000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN,          model->index(0, 0)), Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN,         model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN,   model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN,     model->index(1, 0)), Qt::DisplayRole), QVariant("100"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN,         model->index(1, 0)), Qt::DisplayRole), QVariant("101.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN,     model->index(1, 0)), Qt::DisplayRole), QVariant("102.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_COST_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("400000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("40.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN,         model->index(1, 0)), Qt::DisplayRole), QVariant("-105.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("-106.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN,   model->index(1, 0)), Qt::DisplayRole), QVariant("-110.00%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_NAME_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVAILABLE_COLUMN,     model->index(1, 0)), Qt::DisplayRole), QVariant("200"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PRICE_COLUMN,         model->index(1, 0)), Qt::DisplayRole), QVariant("201.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVG_PRICE_COLUMN,     model->index(1, 0)), Qt::DisplayRole), QVariant("202.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_COST_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("500000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PART_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("50.00%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_COLUMN,         model->index(1, 0)), Qt::DisplayRole), QVariant("+205.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("+206.00%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_DAILY_YIELD_COLUMN,   model->index(1, 0)), Qt::DisplayRole), QVariant("+210.00%"));
    // clang-format on

    model->lastPriceChanged("aaaaa", 202.0);
    model->updateLastPrices();

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN),                              Qt::DisplayRole), QVariant("Currency and metals"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN),                         Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN),                             Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN),                         Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_COST_COLUMN),                              Qt::DisplayRole), QVariant("100000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN),                              Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN),                             Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN),                     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN),                       Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_NAME_COLUMN),                              Qt::DisplayRole), QVariant("Share"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVAILABLE_COLUMN),                         Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PRICE_COLUMN),                             Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVG_PRICE_COLUMN),                         Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_COST_COLUMN),                              Qt::DisplayRole), QVariant("900000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PART_COLUMN),                              Qt::DisplayRole), QVariant("90.00%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_COLUMN),                             Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_PERCENT_COLUMN),                     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_DAILY_YIELD_COLUMN),                       Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN,          model->index(0, 0)), Qt::DisplayRole), QVariant("RUBLE"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN,     model->index(0, 0)), Qt::DisplayRole), QVariant("100000.12"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN,         model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN,     model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_COST_COLUMN,          model->index(0, 0)), Qt::DisplayRole), QVariant("100000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN,          model->index(0, 0)), Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN,         model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN,   model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN,     model->index(1, 0)), Qt::DisplayRole), QVariant("100"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN,         model->index(1, 0)), Qt::DisplayRole), QVariant("202.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN,     model->index(1, 0)), Qt::DisplayRole), QVariant("102.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_COST_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("400000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("40.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN,         model->index(1, 0)), Qt::DisplayRole), QVariant("-379800.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("-94.95%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN,   model->index(1, 0)), Qt::DisplayRole), QVariant("+18432.11%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_NAME_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVAILABLE_COLUMN,     model->index(1, 0)), Qt::DisplayRole), QVariant("200"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PRICE_COLUMN,         model->index(1, 0)), Qt::DisplayRole), QVariant("201.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVG_PRICE_COLUMN,     model->index(1, 0)), Qt::DisplayRole), QVariant("202.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_COST_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("500000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PART_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("50.00%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_COLUMN,         model->index(1, 0)), Qt::DisplayRole), QVariant("+205.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("+206.00%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_DAILY_YIELD_COLUMN,   model->index(1, 0)), Qt::DisplayRole), QVariant("+210.00%"));
    // clang-format on

    model->sort(PORTFOLIO_PRICE_COLUMN, Qt::AscendingOrder);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN),                              Qt::DisplayRole), QVariant("Currency and metals"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN),                         Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN),                             Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN),                         Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_COST_COLUMN),                              Qt::DisplayRole), QVariant("100000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN),                              Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN),                             Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN),                     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN),                       Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_NAME_COLUMN),                              Qt::DisplayRole), QVariant("Share"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVAILABLE_COLUMN),                         Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PRICE_COLUMN),                             Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVG_PRICE_COLUMN),                         Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_COST_COLUMN),                              Qt::DisplayRole), QVariant("900000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PART_COLUMN),                              Qt::DisplayRole), QVariant("90.00%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_COLUMN),                             Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_PERCENT_COLUMN),                     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_DAILY_YIELD_COLUMN),                       Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN,          model->index(0, 0)), Qt::DisplayRole), QVariant("RUBLE"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN,     model->index(0, 0)), Qt::DisplayRole), QVariant("100000.12"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN,         model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN,     model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_COST_COLUMN,          model->index(0, 0)), Qt::DisplayRole), QVariant("100000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN,          model->index(0, 0)), Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN,         model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN,   model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN,     model->index(1, 0)), Qt::DisplayRole), QVariant("200"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN,         model->index(1, 0)), Qt::DisplayRole), QVariant("201.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN,     model->index(1, 0)), Qt::DisplayRole), QVariant("202.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_COST_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("500000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("50.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN,         model->index(1, 0)), Qt::DisplayRole), QVariant("+205.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("+206.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN,   model->index(1, 0)), Qt::DisplayRole), QVariant("+210.00%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_NAME_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVAILABLE_COLUMN,     model->index(1, 0)), Qt::DisplayRole), QVariant("100"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PRICE_COLUMN,         model->index(1, 0)), Qt::DisplayRole), QVariant("202.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVG_PRICE_COLUMN,     model->index(1, 0)), Qt::DisplayRole), QVariant("102.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_COST_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("400000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PART_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("40.00%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_COLUMN,         model->index(1, 0)), Qt::DisplayRole), QVariant("-379800.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("-94.95%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_DAILY_YIELD_COLUMN,   model->index(1, 0)), Qt::DisplayRole), QVariant("+18432.11%"));
    // clang-format on

    model->lastPriceChanged("bbbbb", 603.0);
    model->updateLastPrices();

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN),                              Qt::DisplayRole), QVariant("Currency and metals"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN),                         Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN),                             Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN),                         Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_COST_COLUMN),                              Qt::DisplayRole), QVariant("100000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN),                              Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN),                             Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN),                     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN),                       Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_NAME_COLUMN),                              Qt::DisplayRole), QVariant("Share"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVAILABLE_COLUMN),                         Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PRICE_COLUMN),                             Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVG_PRICE_COLUMN),                         Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_COST_COLUMN),                              Qt::DisplayRole), QVariant("900000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PART_COLUMN),                              Qt::DisplayRole), QVariant("90.00%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_COLUMN),                             Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_PERCENT_COLUMN),                     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_DAILY_YIELD_COLUMN),                       Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN,          model->index(0, 0)), Qt::DisplayRole), QVariant("RUBLE"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN,     model->index(0, 0)), Qt::DisplayRole), QVariant("100000.12"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN,         model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN,     model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_COST_COLUMN,          model->index(0, 0)), Qt::DisplayRole), QVariant("100000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN,          model->index(0, 0)), Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN,         model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN,   model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN,     model->index(1, 0)), Qt::DisplayRole), QVariant("100"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN,         model->index(1, 0)), Qt::DisplayRole), QVariant("202.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN,     model->index(1, 0)), Qt::DisplayRole), QVariant("102.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_COST_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("400000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("40.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN,         model->index(1, 0)), Qt::DisplayRole), QVariant("-379800.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("-94.95%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN,   model->index(1, 0)), Qt::DisplayRole), QVariant("+18432.11%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_NAME_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVAILABLE_COLUMN,     model->index(1, 0)), Qt::DisplayRole), QVariant("200"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PRICE_COLUMN,         model->index(1, 0)), Qt::DisplayRole), QVariant("603.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVG_PRICE_COLUMN,     model->index(1, 0)), Qt::DisplayRole), QVariant("202.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_COST_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("500000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PART_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("50.00%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_COLUMN,         model->index(1, 0)), Qt::DisplayRole), QVariant("-379400.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("-75.88%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_DAILY_YIELD_COLUMN,   model->index(1, 0)), Qt::DisplayRole), QVariant("+57603.35%"));
    // clang-format on
}

TEST_F(Test_PortfolioTreeModel, Test_exportToExcel)
{
    Logo logo;

    Portfolio             portfolio;
    PortfolioCategoryItem category1;
    PortfolioCategoryItem category2;
    PortfolioItem         item1;
    PortfolioItem         item2;
    PortfolioItem         item3;

    item1.instrumentId       = RUBLE_UID;
    item1.instrumentLogo     = nullptr;
    item1.instrumentTicker   = "RUBLE";
    item1.instrumentName     = "Ruble";
    item1.showPrices         = false;
    item1.available          = 100000.12;
    item1.price              = 1.0f;
    item1.avgPriceFifo       = 1.0f;
    item1.avgPriceWavg       = 1.0f;
    item1.cost               = 100000.0;
    item1.part               = 10.0;
    item1.yield              = 0.0f;
    item1.yieldPercent       = 0.0f;
    item1.dailyYield         = 0.0f;
    item1.priceForDailyYield = 0.0f;
    item1.costForDailyYield  = 0.0;
    item1.dailyYieldPercent  = 0.0f;
    item1.pricePrecision     = 2;

    item2.instrumentId       = "aaaaa";
    item2.instrumentLogo     = nullptr;
    item2.instrumentTicker   = "ABBA";
    item2.instrumentName     = "Abstract Basics";
    item2.showPrices         = true;
    item2.available          = 100.0;
    item2.price              = 101.0f;
    item2.avgPriceFifo       = 102.0f;
    item2.avgPriceWavg       = 103.0f;
    item2.cost               = 400000.0;
    item2.part               = 40.0;
    item2.yield              = -105.0f;
    item2.yieldPercent       = -106.0f;
    item2.dailyYield         = 107.0f;
    item2.priceForDailyYield = 108.0f;
    item2.costForDailyYield  = 109.0;
    item2.dailyYieldPercent  = -110.0f;
    item2.pricePrecision     = 3;

    item3.instrumentId       = "bbbbb";
    item3.instrumentLogo     = &logo;
    item3.instrumentTicker   = "BASE";
    item3.instrumentName     = "Basketball enhancement";
    item3.showPrices         = true;
    item3.available          = 200.0;
    item3.price              = 201.0f;
    item3.avgPriceFifo       = 202.0f;
    item3.avgPriceWavg       = 203.0f;
    item3.cost               = 500000.0;
    item3.part               = 50.0;
    item3.yield              = 205.0f;
    item3.yieldPercent       = 206.0f;
    item3.dailyYield         = 207.0f;
    item3.priceForDailyYield = 208.0f;
    item3.costForDailyYield  = 209.0;
    item3.dailyYieldPercent  = 210.0f;
    item3.pricePrecision     = 4;

    category1.id   = 0;
    category1.name = "Currency and metals";
    category1.cost = 100000.0;
    category1.part = 10.0;
    category1.items << item1;

    category2.id   = 1;
    category2.name = "Share";
    category2.cost = 900000.0;
    category2.part = 90.0;
    category2.items << item2 << item3;

    portfolio.positions << category1 << category2;

    model->portfolioChanged(portfolio);
    ASSERT_EQ(model->rowCount(), 2);
    ASSERT_EQ(model->rowCount(model->index(0, 0)), 1);
    ASSERT_EQ(model->rowCount(model->index(1, 0)), 2);

    // clang-format off
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN),                              Qt::DisplayRole), QVariant("Currency and metals"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN),                         Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN),                             Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN),                         Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_COST_COLUMN),                              Qt::DisplayRole), QVariant("100000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN),                              Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN),                             Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN),                     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN),                       Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_NAME_COLUMN),                              Qt::DisplayRole), QVariant("Share"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVAILABLE_COLUMN),                         Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PRICE_COLUMN),                             Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVG_PRICE_COLUMN),                         Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_COST_COLUMN),                              Qt::DisplayRole), QVariant("900000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PART_COLUMN),                              Qt::DisplayRole), QVariant("90.00%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_COLUMN),                             Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_PERCENT_COLUMN),                     Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_DAILY_YIELD_COLUMN),                       Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN,          model->index(0, 0)), Qt::DisplayRole), QVariant("RUBLE"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN,     model->index(0, 0)), Qt::DisplayRole), QVariant("100000.12"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN,         model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN,     model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_COST_COLUMN,          model->index(0, 0)), Qt::DisplayRole), QVariant("100000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN,          model->index(0, 0)), Qt::DisplayRole), QVariant("10.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN,         model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN,   model->index(0, 0)), Qt::DisplayRole), QVariant());
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_NAME_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("ABBA"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVAILABLE_COLUMN,     model->index(1, 0)), Qt::DisplayRole), QVariant("100"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PRICE_COLUMN,         model->index(1, 0)), Qt::DisplayRole), QVariant("101.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_AVG_PRICE_COLUMN,     model->index(1, 0)), Qt::DisplayRole), QVariant("102.000 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_COST_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("400000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_PART_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("40.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_COLUMN,         model->index(1, 0)), Qt::DisplayRole), QVariant("-105.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("-106.00%"));
    ASSERT_EQ(model->data(model->index(0, PORTFOLIO_DAILY_YIELD_COLUMN,   model->index(1, 0)), Qt::DisplayRole), QVariant("-110.00%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_NAME_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("BASE"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVAILABLE_COLUMN,     model->index(1, 0)), Qt::DisplayRole), QVariant("200"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PRICE_COLUMN,         model->index(1, 0)), Qt::DisplayRole), QVariant("201.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_AVG_PRICE_COLUMN,     model->index(1, 0)), Qt::DisplayRole), QVariant("202.0000 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_COST_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("500000.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_PART_COLUMN,          model->index(1, 0)), Qt::DisplayRole), QVariant("50.00%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_COLUMN,         model->index(1, 0)), Qt::DisplayRole), QVariant("+205.00 \u20BD"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_YIELD_PERCENT_COLUMN, model->index(1, 0)), Qt::DisplayRole), QVariant("+206.00%"));
    ASSERT_EQ(model->data(model->index(1, PORTFOLIO_DAILY_YIELD_COLUMN,   model->index(1, 0)), Qt::DisplayRole), QVariant("+210.00%"));
    // clang-format on

    QXlsx::Document doc;

    model->exportToExcel(doc);

    // clang-format off
    ASSERT_EQ(doc.cellAt(2, PORTFOLIO_NAME_COLUMN + 1)->value(),                       QVariant("Currency and metals"));
    ASSERT_EQ(doc.cellAt(2, PORTFOLIO_AVAILABLE_COLUMN + 1)->value(),                  QVariant(""));
    ASSERT_EQ(doc.cellAt(2, PORTFOLIO_PRICE_COLUMN + 1)->value(),                      QVariant(""));
    ASSERT_EQ(doc.cellAt(2, PORTFOLIO_AVG_PRICE_COLUMN + 1)->value(),                  QVariant(""));
    ASSERT_NEAR(doc.cellAt(2, PORTFOLIO_COST_COLUMN + 1)->value().toDouble(),          100000.0, 0.0001);
    ASSERT_NEAR(doc.cellAt(2, PORTFOLIO_PART_COLUMN + 1)->value().toDouble(),          0.1, 0.0001);
    ASSERT_EQ(doc.cellAt(2, PORTFOLIO_YIELD_COLUMN + 1)->value(),                      QVariant(""));
    ASSERT_EQ(doc.cellAt(2, PORTFOLIO_YIELD_PERCENT_COLUMN + 1)->value(),              QVariant(""));
    ASSERT_EQ(doc.cellAt(2, PORTFOLIO_DAILY_YIELD_COLUMN + 1)->value(),                QVariant(""));
    ASSERT_EQ(doc.cellAt(3, PORTFOLIO_NAME_COLUMN + 1)->value(),                       QVariant("Ruble"));
    ASSERT_NEAR(doc.cellAt(3, PORTFOLIO_AVAILABLE_COLUMN + 1)->value().toDouble(),     100000.12, 0.0001);
    ASSERT_EQ(doc.cellAt(3, PORTFOLIO_PRICE_COLUMN + 1)->value(),                      QVariant(""));
    ASSERT_EQ(doc.cellAt(3, PORTFOLIO_AVG_PRICE_COLUMN + 1)->value(),                  QVariant(""));
    ASSERT_NEAR(doc.cellAt(3, PORTFOLIO_COST_COLUMN + 1)->value().toDouble(),          100000.0, 0.0001);
    ASSERT_NEAR(doc.cellAt(3, PORTFOLIO_PART_COLUMN + 1)->value().toDouble(),          0.1, 0.0001);
    ASSERT_EQ(doc.cellAt(3, PORTFOLIO_YIELD_COLUMN + 1)->value(),                      QVariant(""));
    ASSERT_EQ(doc.cellAt(3, PORTFOLIO_YIELD_PERCENT_COLUMN + 1)->value(),              QVariant(""));
    ASSERT_EQ(doc.cellAt(3, PORTFOLIO_DAILY_YIELD_COLUMN + 1)->value(),                QVariant(""));
    ASSERT_EQ(doc.cellAt(4, PORTFOLIO_NAME_COLUMN + 1)->value(),                       QVariant("Share"));
    ASSERT_EQ(doc.cellAt(4, PORTFOLIO_AVAILABLE_COLUMN + 1)->value(),                  QVariant(""));
    ASSERT_EQ(doc.cellAt(4, PORTFOLIO_PRICE_COLUMN + 1)->value(),                      QVariant(""));
    ASSERT_EQ(doc.cellAt(4, PORTFOLIO_AVG_PRICE_COLUMN + 1)->value(),                  QVariant(""));
    ASSERT_NEAR(doc.cellAt(4, PORTFOLIO_COST_COLUMN + 1)->value().toDouble(),          900000.0, 0.0001);
    ASSERT_NEAR(doc.cellAt(4, PORTFOLIO_PART_COLUMN + 1)->value().toDouble(),          0.9, 0.0001);
    ASSERT_EQ(doc.cellAt(4, PORTFOLIO_YIELD_COLUMN + 1)->value(),                      QVariant(""));
    ASSERT_EQ(doc.cellAt(4, PORTFOLIO_YIELD_PERCENT_COLUMN + 1)->value(),              QVariant(""));
    ASSERT_EQ(doc.cellAt(4, PORTFOLIO_DAILY_YIELD_COLUMN + 1)->value(),                QVariant(""));
    ASSERT_EQ(doc.cellAt(5, PORTFOLIO_NAME_COLUMN + 1)->value(),                       QVariant("Abstract Basics"));
    ASSERT_NEAR(doc.cellAt(5, PORTFOLIO_AVAILABLE_COLUMN + 1)->value().toDouble(),     100.0, 0.0001);
    ASSERT_NEAR(doc.cellAt(5, PORTFOLIO_PRICE_COLUMN + 1)->value().toDouble(),         101.0, 0.0001);
    ASSERT_NEAR(doc.cellAt(5, PORTFOLIO_AVG_PRICE_COLUMN + 1)->value().toDouble(),     102.0, 0.0001);
    ASSERT_NEAR(doc.cellAt(5, PORTFOLIO_COST_COLUMN + 1)->value().toDouble(),          400000.0, 0.0001);
    ASSERT_NEAR(doc.cellAt(5, PORTFOLIO_PART_COLUMN + 1)->value().toDouble(),          0.4, 0.0001);
    ASSERT_NEAR(doc.cellAt(5, PORTFOLIO_YIELD_COLUMN + 1)->value().toDouble(),         -105.0, 0.0001);
    ASSERT_NEAR(doc.cellAt(5, PORTFOLIO_YIELD_PERCENT_COLUMN + 1)->value().toDouble(), -1.06, 0.0001);
    ASSERT_NEAR(doc.cellAt(5, PORTFOLIO_DAILY_YIELD_COLUMN + 1)->value().toDouble(),   -1.1, 0.0001);
    ASSERT_EQ(doc.cellAt(6, PORTFOLIO_NAME_COLUMN + 1)->value(),                       QVariant("Basketball enhancement"));
    ASSERT_NEAR(doc.cellAt(6, PORTFOLIO_AVAILABLE_COLUMN + 1)->value().toDouble(),     200.0, 0.0001);
    ASSERT_NEAR(doc.cellAt(6, PORTFOLIO_PRICE_COLUMN + 1)->value().toDouble(),         201.0, 0.0001);
    ASSERT_NEAR(doc.cellAt(6, PORTFOLIO_AVG_PRICE_COLUMN + 1)->value().toDouble(),     202.0, 0.0001);
    ASSERT_NEAR(doc.cellAt(6, PORTFOLIO_COST_COLUMN + 1)->value().toDouble(),          500000.0, 0.0001);
    ASSERT_NEAR(doc.cellAt(6, PORTFOLIO_PART_COLUMN + 1)->value().toDouble(),          0.5, 0.0001);
    ASSERT_NEAR(doc.cellAt(6, PORTFOLIO_YIELD_COLUMN + 1)->value().toDouble(),         205.0, 0.0001);
    ASSERT_NEAR(doc.cellAt(6, PORTFOLIO_YIELD_PERCENT_COLUMN + 1)->value().toDouble(), 2.06, 0.0001);
    ASSERT_NEAR(doc.cellAt(6, PORTFOLIO_DAILY_YIELD_COLUMN + 1)->value().toDouble(),   2.1, 0.0001);
    // clang-format on
}

TEST_F(Test_PortfolioTreeModel, Test_totalCost)
{
    ASSERT_NEAR(model->totalCost(), 0, 0.0001);

    Portfolio             portfolio;
    PortfolioCategoryItem category1;
    PortfolioCategoryItem category2;

    category1.id   = 0;
    category1.name = "Currency and metals";
    category1.cost = 100000.0;
    category1.part = 10.0;

    category2.id   = 1;
    category2.name = "Share";
    category2.cost = 900000.0;
    category2.part = 90.0;

    portfolio.positions << category1 << category2;

    model->portfolioChanged(portfolio);

    ASSERT_NEAR(model->totalCost(), 1000000.0, 0.0001);

    portfolio.positions[1].cost = 50000.0;

    model->portfolioChanged(portfolio);

    ASSERT_NEAR(model->totalCost(), 150000.0, 0.0001);
}

TEST_F(Test_PortfolioTreeModel, Test_totalYield)
{
    ASSERT_NEAR(model->totalYield(), 0, 0.0001);

    Portfolio             portfolio;
    PortfolioCategoryItem category1;
    PortfolioCategoryItem category2;
    PortfolioItem         item1;
    PortfolioItem         item2;
    PortfolioItem         item3;

    item1.instrumentId       = RUBLE_UID;
    item1.instrumentTicker   = "RUBLE";
    item1.instrumentName     = "Ruble";
    item1.showPrices         = false;
    item1.available          = 100000.12;
    item1.price              = 1.0f;
    item1.avgPriceFifo       = 1.0f;
    item1.avgPriceWavg       = 1.0f;
    item1.cost               = 100000.0;
    item1.part               = 10.0;
    item1.yield              = 0.0f;
    item1.yieldPercent       = 0.0f;
    item1.dailyYield         = 0.0f;
    item1.priceForDailyYield = 0.0f;
    item1.costForDailyYield  = 0.0;
    item1.dailyYieldPercent  = 0.0f;
    item1.pricePrecision     = 2;

    item2.instrumentId       = "aaaaa";
    item2.instrumentTicker   = "ABBA";
    item2.instrumentName     = "Abstract Basics";
    item2.showPrices         = true;
    item2.available          = 100.0;
    item2.price              = 101.0f;
    item2.avgPriceFifo       = 102.0f;
    item2.avgPriceWavg       = 103.0f;
    item2.cost               = 400000.0;
    item2.part               = 40.0;
    item2.yield              = -105.0f;
    item2.yieldPercent       = -106.0f;
    item2.dailyYield         = 107.0f;
    item2.priceForDailyYield = 108.0f;
    item2.costForDailyYield  = 109.0;
    item2.dailyYieldPercent  = -110.0f;
    item2.pricePrecision     = 3;

    item3.instrumentId       = "bbbbb";
    item3.instrumentTicker   = "BASE";
    item3.instrumentName     = "Basketball enhancement";
    item3.showPrices         = true;
    item3.available          = 200.0;
    item3.price              = 201.0f;
    item3.avgPriceFifo       = 202.0f;
    item3.avgPriceWavg       = 203.0f;
    item3.cost               = 500000.0;
    item3.part               = 50.0;
    item3.yield              = 205.0f;
    item3.yieldPercent       = 206.0f;
    item3.dailyYield         = 207.0f;
    item3.priceForDailyYield = 208.0f;
    item3.costForDailyYield  = 209.0;
    item3.dailyYieldPercent  = 210.0f;
    item3.pricePrecision     = 4;

    category1.id   = 0;
    category1.name = "Currency and metals";
    category1.cost = 100000.0;
    category1.part = 10.0;
    category1.items << item1;

    category2.id   = 1;
    category2.name = "Share";
    category2.cost = 900000.0;
    category2.part = 90.0;
    category2.items << item2 << item3;

    portfolio.positions << category1 << category2;

    model->portfolioChanged(portfolio);

    ASSERT_NEAR(model->totalYield(), 100.0, 0.0001);

    portfolio.positions[1].items[0].yield = 105.0;

    model->portfolioChanged(portfolio);

    ASSERT_NEAR(model->totalYield(), 310.0, 0.0001);
}

TEST_F(Test_PortfolioTreeModel, Test_totalDailyCost)
{
    ASSERT_NEAR(model->totalDailyCost(), 0, 0.0001);

    Portfolio             portfolio;
    PortfolioCategoryItem category1;
    PortfolioCategoryItem category2;
    PortfolioItem         item1;
    PortfolioItem         item2;
    PortfolioItem         item3;

    item1.instrumentId       = RUBLE_UID;
    item1.instrumentTicker   = "RUBLE";
    item1.instrumentName     = "Ruble";
    item1.showPrices         = false;
    item1.available          = 100000.12;
    item1.price              = 1.0f;
    item1.avgPriceFifo       = 1.0f;
    item1.avgPriceWavg       = 1.0f;
    item1.cost               = 100000.0;
    item1.part               = 10.0;
    item1.yield              = 0.0f;
    item1.yieldPercent       = 0.0f;
    item1.dailyYield         = 0.0f;
    item1.priceForDailyYield = 0.0f;
    item1.costForDailyYield  = 0.0;
    item1.dailyYieldPercent  = 0.0f;
    item1.pricePrecision     = 2;

    item2.instrumentId       = "aaaaa";
    item2.instrumentTicker   = "ABBA";
    item2.instrumentName     = "Abstract Basics";
    item2.showPrices         = true;
    item2.available          = 100.0;
    item2.price              = 101.0f;
    item2.avgPriceFifo       = 102.0f;
    item2.avgPriceWavg       = 103.0f;
    item2.cost               = 400000.0;
    item2.part               = 40.0;
    item2.yield              = -105.0f;
    item2.yieldPercent       = -106.0f;
    item2.dailyYield         = 107.0f;
    item2.priceForDailyYield = 108.0f;
    item2.costForDailyYield  = 109.0;
    item2.dailyYieldPercent  = -110.0f;
    item2.pricePrecision     = 3;

    item3.instrumentId       = "bbbbb";
    item3.instrumentTicker   = "BASE";
    item3.instrumentName     = "Basketball enhancement";
    item3.showPrices         = true;
    item3.available          = 200.0;
    item3.price              = 201.0f;
    item3.avgPriceFifo       = 202.0f;
    item3.avgPriceWavg       = 203.0f;
    item3.cost               = 500000.0;
    item3.part               = 50.0;
    item3.yield              = 205.0f;
    item3.yieldPercent       = 206.0f;
    item3.dailyYield         = 207.0f;
    item3.priceForDailyYield = 208.0f;
    item3.costForDailyYield  = 209.0;
    item3.dailyYieldPercent  = 210.0f;
    item3.pricePrecision     = 4;

    category1.id   = 0;
    category1.name = "Currency and metals";
    category1.cost = 100000.0;
    category1.part = 10.0;
    category1.items << item1;

    category2.id   = 1;
    category2.name = "Share";
    category2.cost = 900000.0;
    category2.part = 90.0;
    category2.items << item2 << item3;

    portfolio.positions << category1 << category2;

    model->portfolioChanged(portfolio);

    ASSERT_NEAR(model->totalDailyCost(), 318.0, 0.0001);

    portfolio.positions[1].items[0].costForDailyYield = 291.0;

    model->portfolioChanged(portfolio);

    ASSERT_NEAR(model->totalDailyCost(), 500.0, 0.0001);
}

TEST_F(Test_PortfolioTreeModel, Test_totalDailyYield)
{
    ASSERT_NEAR(model->totalDailyYield(), 0, 0.0001);

    Portfolio             portfolio;
    PortfolioCategoryItem category1;
    PortfolioCategoryItem category2;
    PortfolioItem         item1;
    PortfolioItem         item2;
    PortfolioItem         item3;

    item1.instrumentId       = RUBLE_UID;
    item1.instrumentTicker   = "RUBLE";
    item1.instrumentName     = "Ruble";
    item1.showPrices         = false;
    item1.available          = 100000.12;
    item1.price              = 1.0f;
    item1.avgPriceFifo       = 1.0f;
    item1.avgPriceWavg       = 1.0f;
    item1.cost               = 100000.0;
    item1.part               = 10.0;
    item1.yield              = 0.0f;
    item1.yieldPercent       = 0.0f;
    item1.dailyYield         = 0.0f;
    item1.priceForDailyYield = 0.0f;
    item1.costForDailyYield  = 0.0;
    item1.dailyYieldPercent  = 0.0f;
    item1.pricePrecision     = 2;

    item2.instrumentId       = "aaaaa";
    item2.instrumentTicker   = "ABBA";
    item2.instrumentName     = "Abstract Basics";
    item2.showPrices         = true;
    item2.available          = 100.0;
    item2.price              = 101.0f;
    item2.avgPriceFifo       = 102.0f;
    item2.avgPriceWavg       = 103.0f;
    item2.cost               = 400000.0;
    item2.part               = 40.0;
    item2.yield              = -105.0f;
    item2.yieldPercent       = -106.0f;
    item2.dailyYield         = 107.0f;
    item2.priceForDailyYield = 108.0f;
    item2.costForDailyYield  = 109.0;
    item2.dailyYieldPercent  = -110.0f;
    item2.pricePrecision     = 3;

    item3.instrumentId       = "bbbbb";
    item3.instrumentTicker   = "BASE";
    item3.instrumentName     = "Basketball enhancement";
    item3.showPrices         = true;
    item3.available          = 200.0;
    item3.price              = 201.0f;
    item3.avgPriceFifo       = 202.0f;
    item3.avgPriceWavg       = 203.0f;
    item3.cost               = 500000.0;
    item3.part               = 50.0;
    item3.yield              = 205.0f;
    item3.yieldPercent       = 206.0f;
    item3.dailyYield         = 207.0f;
    item3.priceForDailyYield = 208.0f;
    item3.costForDailyYield  = 209.0;
    item3.dailyYieldPercent  = 210.0f;
    item3.pricePrecision     = 4;

    category1.id   = 0;
    category1.name = "Currency and metals";
    category1.cost = 100000.0;
    category1.part = 10.0;
    category1.items << item1;

    category2.id   = 1;
    category2.name = "Share";
    category2.cost = 900000.0;
    category2.part = 90.0;
    category2.items << item2 << item3;

    portfolio.positions << category1 << category2;

    model->portfolioChanged(portfolio);

    ASSERT_NEAR(model->totalDailyYield(), 314.0, 0.0001);

    portfolio.positions[1].items[0].dailyYield = -107.0;

    model->portfolioChanged(portfolio);

    ASSERT_NEAR(model->totalDailyYield(), 100.0, 0.0001);
}
