#include "src/widgets/portfoliotreewidget/portfoliotreewidget.h"
#include "ui_portfoliotreewidget.h"

#include <QDebug>



#ifdef Q_OS_WINDOWS
const int COLUMN_WIDTHS[PORTFOLIO_COLUMN_COUNT] = {82, 80, 56, 106, 87, 56, 63, 79, 86};
#else
const int COLUMN_WIDTHS[PORTFOLIO_COLUMN_COUNT] = {89, 87, 59, 114, 95, 59, 66, 83, 93};
#endif



PortfolioTreeWidget::PortfolioTreeWidget(
    IPortfolioTreeRecordFactory* portfolioTreeRecordFactory,
    IInstrumentsStorage*         instrumentsStorage,
    ISettingsEditor*             settingsEditor,
    QWidget*                     parent
) :
    IPortfolioTreeWidget(parent),
    ui(new Ui::PortfolioTreeWidget),
    mPortfolioTreeRecordFactory(portfolioTreeRecordFactory),
    mInstrumentsStorage(instrumentsStorage),
    mSettingsEditor(settingsEditor),
    mSortedCategories(),
    mCategoryNames(),
    mCategories(),
    mRecords()
{
    qDebug() << "Create PortfolioTreeWidget";

    ui->setupUi(this);

    mSortedCategories << "currency" << "share";

    mCategoryNames["currency"] = tr("Currency and metals");
    mCategoryNames["share"]    = tr("Share");
}

PortfolioTreeWidget::~PortfolioTreeWidget()
{
    qDebug() << "Destroy PortfolioTreeWidget";

    delete ui;
}

void PortfolioTreeWidget::portfolioChanged(const Portfolio& portfolio)
{
    deleteObsoleteCategories(portfolio);

    for (const QString& category : std::as_const(mSortedCategories))
    {
        if (!portfolio.positions.contains(category))
        {
            continue;
        }

        CategoryTreeItem* categoryTreeItem = mCategories.value(category, nullptr);

        if (categoryTreeItem == nullptr)
        {
            Q_ASSERT_X(mCategoryNames.contains(category), "PortfolioTreeWidget::portfolioChanged()", "Missing translation");
            categoryTreeItem = new CategoryTreeItem(ui->treeWidget, mCategoryNames.value(category, "UNKNOWN"));

            mCategories[category] = categoryTreeItem;
        }

        updateCategory(categoryTreeItem, portfolio.positions[category]);
    }
}

void PortfolioTreeWidget::deleteObsoleteCategories(const Portfolio& portfolio)
{
    QStringList categoriesToDelete;

    for (auto it = mCategories.constBegin(), end = mCategories.constEnd(); it != end; ++it)
    {
        if (!portfolio.positions.contains(it.key()))
        {
            categoriesToDelete.append(it.key());
        }
    }

    for (const QString& category : categoriesToDelete)
    {
        CategoryTreeItem* categoryTreeItem = mCategories.take(category);

        // TODO: Remove children
        delete categoryTreeItem;
    }
}

void PortfolioTreeWidget::updateCategory(CategoryTreeItem* categoryTreeItem, const PortfolioItems& portfolioItems)
{
    const PortfolioItem& categoryTotal = portfolioItems["total"];

    categoryTreeItem->setCost(categoryTotal.cost);
    categoryTreeItem->setPart(categoryTotal.part);

    for (auto it = portfolioItems.constBegin(), end = portfolioItems.constEnd(); it != end; ++it)
    {
        const QString& instrumentId = it.key();

        if (instrumentId == "total")
        {
            continue;
        }

        IPortfolioTreeRecord* record = mRecords[instrumentId];

        if (record == nullptr)
        {
            record = mPortfolioTreeRecordFactory->newInstance(mInstrumentsStorage, categoryTreeItem, this);

            mRecords[instrumentId] = record;
        }
    }
}

void PortfolioTreeWidget::saveWindowState(const QString& type)
{
    // clang-format off
    mSettingsEditor->setValue(type + "/columnWidth_Name",         ui->treeWidget->columnWidth(PORTFOLIO_NAME_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Available",    ui->treeWidget->columnWidth(PORTFOLIO_AVAILABLE_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Price",        ui->treeWidget->columnWidth(PORTFOLIO_PRICE_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_AvgPrice",     ui->treeWidget->columnWidth(PORTFOLIO_AVG_PRICE_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Cost",         ui->treeWidget->columnWidth(PORTFOLIO_COST_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Part",         ui->treeWidget->columnWidth(PORTFOLIO_PART_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Yield",        ui->treeWidget->columnWidth(PORTFOLIO_YIELD_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_YieldPercent", ui->treeWidget->columnWidth(PORTFOLIO_YIELD_PERCENT_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_DailyYield",   ui->treeWidget->columnWidth(PORTFOLIO_DAILY_YIELD_COLUMN));
    // clang-format on
}

// NOLINTBEGIN(readability-magic-numbers)
void PortfolioTreeWidget::loadWindowState(const QString& type)
{
    // clang-format off
    ui->treeWidget->setColumnWidth(PORTFOLIO_NAME_COLUMN,          mSettingsEditor->value(type + "/columnWidth_Name",         COLUMN_WIDTHS[PORTFOLIO_NAME_COLUMN]).toInt());
    ui->treeWidget->setColumnWidth(PORTFOLIO_AVAILABLE_COLUMN,     mSettingsEditor->value(type + "/columnWidth_Available",    COLUMN_WIDTHS[PORTFOLIO_AVAILABLE_COLUMN]).toInt());
    ui->treeWidget->setColumnWidth(PORTFOLIO_PRICE_COLUMN,         mSettingsEditor->value(type + "/columnWidth_Price",        COLUMN_WIDTHS[PORTFOLIO_PRICE_COLUMN]).toInt());
    ui->treeWidget->setColumnWidth(PORTFOLIO_AVG_PRICE_COLUMN,     mSettingsEditor->value(type + "/columnWidth_AvgPrice",     COLUMN_WIDTHS[PORTFOLIO_AVG_PRICE_COLUMN]).toInt());
    ui->treeWidget->setColumnWidth(PORTFOLIO_COST_COLUMN,          mSettingsEditor->value(type + "/columnWidth_Cost",         COLUMN_WIDTHS[PORTFOLIO_COST_COLUMN]).toInt());
    ui->treeWidget->setColumnWidth(PORTFOLIO_PART_COLUMN,          mSettingsEditor->value(type + "/columnWidth_Part",         COLUMN_WIDTHS[PORTFOLIO_PART_COLUMN]).toInt());
    ui->treeWidget->setColumnWidth(PORTFOLIO_YIELD_COLUMN,         mSettingsEditor->value(type + "/columnWidth_Yield",        COLUMN_WIDTHS[PORTFOLIO_YIELD_COLUMN]).toInt());
    ui->treeWidget->setColumnWidth(PORTFOLIO_YIELD_PERCENT_COLUMN, mSettingsEditor->value(type + "/columnWidth_YieldPercent", COLUMN_WIDTHS[PORTFOLIO_YIELD_PERCENT_COLUMN]).toInt());
    ui->treeWidget->setColumnWidth(PORTFOLIO_DAILY_YIELD_COLUMN,   mSettingsEditor->value(type + "/columnWidth_DailyYield",   COLUMN_WIDTHS[PORTFOLIO_DAILY_YIELD_COLUMN]).toInt());
    // clang-format on
}
// NOLINTEND(readability-magic-numbers)
