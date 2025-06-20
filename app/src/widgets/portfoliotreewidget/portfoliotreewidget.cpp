#include "src/widgets/portfoliotreewidget/portfoliotreewidget.h"
#include "ui_portfoliotreewidget.h"

#include <QDebug>
#include <QMenu>

#include "src/qxlsx/xlsxdocument.h"
#include "src/widgets/treeitems/portfoliotreeitem.h"



#ifdef Q_OS_WINDOWS
const int COLUMN_WIDTHS[PORTFOLIO_COLUMN_COUNT] = {178, 80, 56, 106, 87, 56, 68, 79, 1100};
#else
const int COLUMN_WIDTHS[PORTFOLIO_COLUMN_COUNT] = {89, 87, 59, 114, 95, 59, 66, 83, 93};
#endif

const QColor HEADER_BACKGROUND_COLOR   = QColor("#354450"); // clazy:exclude=non-pod-global-static
const QColor HEADER_FONT_COLOR         = QColor("#699BA2"); // clazy:exclude=non-pod-global-static
const QColor CATEGORY_BACKGROUND_COLOR = QColor("#354450"); // clazy:exclude=non-pod-global-static
const QColor CATEGORY_FONT_COLOR       = QColor("#BFBFBF"); // clazy:exclude=non-pod-global-static
const QColor GREEN_COLOR               = QColor("#2BD793"); // clazy:exclude=non-pod-global-static
const QColor RED_COLOR                 = QColor("#ED6F7E"); // clazy:exclude=non-pod-global-static
const QColor NORMAL_COLOR              = QColor("#97AEC4"); // clazy:exclude=non-pod-global-static

constexpr QChar  RUBLE           = QChar(0x20BD);
constexpr float  ZERO_LIMIT      = 0.0001f;
constexpr float  HUNDRED_PERCENT = 100.0f;
constexpr double COLUMN_GAP      = 0.71;



PortfolioTreeWidget::PortfolioTreeWidget(
    IPortfolioTreeRecordFactory* portfolioTreeRecordFactory,
    IInstrumentWidgetFactory*    instrumentWidgetFactory,
    IUserStorage*                userStorage,
    IInstrumentsStorage*         instrumentsStorage,
    IFileDialogFactory*          fileDialogFactory,
    ISettingsEditor*             settingsEditor,
    QWidget*                     parent
) :
    IPortfolioTreeWidget(parent),
    ui(new Ui::PortfolioTreeWidget),
    mPortfolioTreeRecordFactory(portfolioTreeRecordFactory),
    mInstrumentWidgetFactory(instrumentWidgetFactory),
    mUserStorage(userStorage),
    mInstrumentsStorage(instrumentsStorage),
    mFileDialogFactory(fileDialogFactory),
    mSettingsEditor(settingsEditor),
    mSortedCategories(),
    mCategoryNames(),
    mCategories(),
    mRecords(),
    mLastPricesUpdates(),
    mTotalCost(),
    mTotalYield(),
    mTotalDailyCost(),
    mTotalDailyYield()
{
    qDebug() << "Create PortfolioTreeWidget";

    ui->setupUi(this);

    mSortedCategories << "currency" << "share" << "etf" << "bond" << "futures" << "options";

    mCategoryNames["currency"] = tr("Currency and metals");
    mCategoryNames["share"]    = tr("Share");
    mCategoryNames["etf"]      = tr("ETF");
    mCategoryNames["bond"]     = tr("Bond");
    mCategoryNames["futures"]  = tr("Futures");
    mCategoryNames["options"]  = tr("Options");
}

PortfolioTreeWidget::~PortfolioTreeWidget()
{
    qDebug() << "Destroy PortfolioTreeWidget";

    delete ui;
}

void PortfolioTreeWidget::setAccountName(const QString& name)
{
    ui->accountNameLabel->setText(name);
}

void PortfolioTreeWidget::portfolioChanged(const Portfolio& portfolio)
{
    mTotalCost       = 0.0;
    mTotalYield      = 0.0;
    mTotalDailyCost  = 0.0;
    mTotalDailyYield = 0.0;

    ui->treeWidget->setUpdatesEnabled(false);
    ui->treeWidget->setSortingEnabled(false);

    deleteObsoleteCategories(portfolio);

    for (const QString& category : std::as_const(mSortedCategories))
    {
        if (!portfolio.positions.contains(category))
        {
            continue;
        }

        CategoryTreeItem* categoryTreeItem = mCategories.value(category);

        if (categoryTreeItem == nullptr)
        {
            Q_ASSERT_X(mCategoryNames.contains(category), __FUNCTION__, "Missing translation");
            categoryTreeItem = new CategoryTreeItem(ui->treeWidget, mCategoryNames.value(category, "UNKNOWN"));

            mCategories[category] = categoryTreeItem;
        }

        updateCategory(categoryTreeItem, portfolio.positions[category]);
    }

    ui->treeWidget->setSortingEnabled(true);
    ui->treeWidget->setUpdatesEnabled(true);

    ui->costLabel->setText(QString::number(mTotalCost, 'f', 2) + " " + RUBLE);
    updateAllTimeLabel();
    updateForTodayLabel();
}

void PortfolioTreeWidget::lastPriceChanged(const QString& instrumentId, float price)
{
    mLastPricesUpdates[instrumentId] = price;
}

void PortfolioTreeWidget::updateLastPrices()
{
    if (!mLastPricesUpdates.isEmpty())
    {
        ui->treeWidget->setUpdatesEnabled(false);
        ui->treeWidget->setSortingEnabled(false);

        for (auto it = mLastPricesUpdates.constBegin(); it != mLastPricesUpdates.constEnd(); ++it)
        {
            IPortfolioTreeRecord* record = mRecords.value(it.key());

            if (record != nullptr)
            {
                record->updatePrice(it.value());
            }
        }

        mLastPricesUpdates.clear();

        ui->treeWidget->setSortingEnabled(true);
        ui->treeWidget->setUpdatesEnabled(true);

        mTotalYield      = 0.0;
        mTotalDailyYield = 0.0;

        for (auto it = mRecords.constBegin(); it != mRecords.constEnd(); ++it)
        {
            IPortfolioTreeRecord* record = it.value();

            mTotalYield      += record->yield();
            mTotalDailyYield += record->dailyYield();
        }

        updateAllTimeLabel();
        updateForTodayLabel();
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

        for (int i = 0; i < categoryTreeItem->childCount(); ++i)
        {
            PortfolioTreeItem* portfolioTreeItem = dynamic_cast<PortfolioTreeItem*>(categoryTreeItem->child(i));
            delete mRecords.take(portfolioTreeItem->instrumentId());
        }

        delete categoryTreeItem;
    }
}

void PortfolioTreeWidget::updateCategory(CategoryTreeItem* categoryTreeItem, const PortfolioItems& portfolioItems)
{
    const PortfolioItem& categoryTotal = portfolioItems["total"];

    mTotalCost += categoryTotal.cost;

    categoryTreeItem->setCost(categoryTotal.cost);
    categoryTreeItem->setPart(categoryTotal.part);

    deleteObsoleteRecords(categoryTreeItem, portfolioItems);

    for (auto it = portfolioItems.constBegin(), end = portfolioItems.constEnd(); it != end; ++it)
    {
        const QString& instrumentId = it.key();

        if (instrumentId == "total")
        {
            continue;
        }

        const PortfolioItem& portfolioItem = it.value();

        IPortfolioTreeRecord* record = mRecords.value(instrumentId);

        if (record == nullptr)
        {
            record = mPortfolioTreeRecordFactory->newInstance(
                mInstrumentWidgetFactory, mUserStorage, mInstrumentsStorage, categoryTreeItem, instrumentId, this
            );
            mRecords[instrumentId] = record;
        }

        record->setPortfolioItem(portfolioItem);

        mTotalYield      += portfolioItem.yield;
        mTotalDailyCost  += portfolioItem.costForDailyYield;
        mTotalDailyYield += portfolioItem.dailyYield;
    }
}

void PortfolioTreeWidget::deleteObsoleteRecords(CategoryTreeItem* categoryTreeItem, const PortfolioItems& portfolioItems)
{
    QList<PortfolioTreeItem*> itemsToDelete;

    itemsToDelete.reserve(categoryTreeItem->childCount());

    for (int i = 0; i < categoryTreeItem->childCount(); ++i)
    {
        PortfolioTreeItem* portfolioTreeItem = dynamic_cast<PortfolioTreeItem*>(categoryTreeItem->child(i));

        if (!portfolioItems.contains(portfolioTreeItem->instrumentId()))
        {
            itemsToDelete.append(portfolioTreeItem);
        }
    }

    for (PortfolioTreeItem* item : itemsToDelete)
    {
        delete mRecords.take(item->instrumentId());
        delete item;
    }
}

void PortfolioTreeWidget::on_treeWidget_customContextMenuRequested(const QPoint& pos)
{
    QMenu* contextMenu = new QMenu(this);

    contextMenu->addAction(tr("Export to Excel"), this, SLOT(actionExportToExcelTriggered()));

    contextMenu->popup(ui->treeWidget->viewport()->mapToGlobal(pos));
}

void PortfolioTreeWidget::actionExportToExcelTriggered()
{
    const QString lastFile = mSettingsEditor->value("MainWindow/PortfolioTreeWidget/exportToExcelFile", "").toString();

    const std::shared_ptr<IFileDialog> fileDialog = mFileDialogFactory->newInstance(
        this, tr("Export"), lastFile.left(lastFile.lastIndexOf("/")), tr("Excel file") + " (*.xlsx)"
    );
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);
    fileDialog->setDefaultSuffix("xlsx");

    fileDialog->selectFile(lastFile);

    if (fileDialog->exec() == QDialog::Accepted)
    {
        const QString path = fileDialog->selectedFiles().at(0);
        mSettingsEditor->setValue("MainWindow/PortfolioTreeWidget/exportToExcelFile", path);

        exportToExcel(path);
    }
}

void PortfolioTreeWidget::updateAllTimeLabel()
{
    updateYieldLabel(ui->allTimeLabel, mTotalYield, mTotalCost);
}

void PortfolioTreeWidget::updateForTodayLabel()
{
    updateYieldLabel(ui->forTodayLabel, mTotalDailyYield, mTotalDailyCost);
}

void PortfolioTreeWidget::updateYieldLabel(QLabel* label, double yield, double cost)
{
    const QString prefix  = yield > 0 ? "+" : "";
    const float   percent = cost > 0 ? (yield / cost) * HUNDRED_PERCENT : 0.0f;

    label->setText(
        prefix + QString::number(yield, 'f', 2) + " " + RUBLE + "(" + prefix + QString::number(percent, 'f', 2) + "%)"
    );

    QColor color;

    if (yield > -ZERO_LIMIT && yield < ZERO_LIMIT)
    {
        color = NORMAL_COLOR;
    }
    else
    {
        if (yield > 0)
        {
            color = GREEN_COLOR;
        }
        else
        {
            color = RED_COLOR;
        }
    }

    QPalette palette;
    palette.setColor(QPalette::WindowText, color);
    label->setPalette(palette);
}

void PortfolioTreeWidget::exportToExcel(const QString& path) const
{
    QXlsx::Document doc;
    doc.addSheet(tr("Portfolio"));

    QXlsx::Format headerStyle;
    headerStyle.setFontBold(true);
    headerStyle.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    headerStyle.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    headerStyle.setFillPattern(QXlsx::Format::PatternSolid);
    headerStyle.setBorderStyle(QXlsx::Format::BorderThin);
    headerStyle.setPatternBackgroundColor(HEADER_BACKGROUND_COLOR);
    headerStyle.setFontColor(HEADER_FONT_COLOR);

    QXlsx::Format categoryStyle;
    categoryStyle.setFontBold(true);
    categoryStyle.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    categoryStyle.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    categoryStyle.setFillPattern(QXlsx::Format::PatternSolid);
    categoryStyle.setPatternBackgroundColor(CATEGORY_BACKGROUND_COLOR);
    categoryStyle.setFontColor(CATEGORY_FONT_COLOR);

    QXlsx::Format categoryRubleStyle;
    categoryRubleStyle.setFontBold(true);
    categoryRubleStyle.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    categoryRubleStyle.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    categoryRubleStyle.setNumberFormat("0.00 \u20BD");
    categoryRubleStyle.setFillPattern(QXlsx::Format::PatternSolid);
    categoryRubleStyle.setPatternBackgroundColor(CATEGORY_BACKGROUND_COLOR);
    categoryRubleStyle.setFontColor(CATEGORY_FONT_COLOR);

    QXlsx::Format categoryPercentStyle;
    categoryPercentStyle.setFontBold(true);
    categoryPercentStyle.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    categoryPercentStyle.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    categoryPercentStyle.setNumberFormat("0.00%");
    categoryPercentStyle.setFillPattern(QXlsx::Format::PatternSolid);
    categoryPercentStyle.setPatternBackgroundColor(CATEGORY_BACKGROUND_COLOR);
    categoryPercentStyle.setFontColor(CATEGORY_FONT_COLOR);

    for (int i = 0; i < ui->treeWidget->columnCount(); ++i)
    {
        doc.write(1, i + 1, ui->treeWidget->headerItem()->text(i), headerStyle);
    }

    int curRow = 2;

    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i)
    {
        CategoryTreeItem* categoryTreeItem = dynamic_cast<CategoryTreeItem*>(ui->treeWidget->topLevelItem(i));

        // clang-format off
        doc.write(curRow, PORTFOLIO_NAME_COLUMN + 1,          categoryTreeItem->name(), categoryStyle);
        doc.write(curRow, PORTFOLIO_AVAILABLE_COLUMN + 1,     "", categoryStyle);
        doc.write(curRow, PORTFOLIO_PRICE_COLUMN + 1,         "", categoryStyle);
        doc.write(curRow, PORTFOLIO_AVG_PRICE_COLUMN + 1,     "", categoryStyle);
        doc.write(curRow, PORTFOLIO_COST_COLUMN + 1,          categoryTreeItem->cost(), categoryRubleStyle);
        doc.write(curRow, PORTFOLIO_PART_COLUMN + 1,          categoryTreeItem->part() / HUNDRED_PERCENT, categoryPercentStyle);
        doc.write(curRow, PORTFOLIO_YIELD_COLUMN + 1,         "", categoryStyle);
        doc.write(curRow, PORTFOLIO_YIELD_PERCENT_COLUMN + 1, "", categoryStyle);
        doc.write(curRow, PORTFOLIO_DAILY_YIELD_COLUMN + 1,   "", categoryStyle);
        // clang-format on

        ++curRow;

        for (int j = 0; j < categoryTreeItem->childCount(); ++j)
        {
            PortfolioTreeItem*    portfolioTreeItem = dynamic_cast<PortfolioTreeItem*>(categoryTreeItem->child(j));
            IPortfolioTreeRecord* record            = mRecords.value(portfolioTreeItem->instrumentId());

            record->exportToExcel(doc, curRow);

            ++curRow;
        }
    }

    // NOLINTBEGIN(readability-magic-numbers)
    // clang-format off
    doc.autosizeColumnWidth(PORTFOLIO_NAME_COLUMN + 1);
    doc.setColumnWidth(PORTFOLIO_AVAILABLE_COLUMN + 1,     11.29 + COLUMN_GAP);
    doc.setColumnWidth(PORTFOLIO_PRICE_COLUMN + 1,         8.43  + COLUMN_GAP);
    doc.setColumnWidth(PORTFOLIO_AVG_PRICE_COLUMN + 1,     13.14 + COLUMN_GAP);
    doc.setColumnWidth(PORTFOLIO_COST_COLUMN + 1,          10.43 + COLUMN_GAP);
    doc.setColumnWidth(PORTFOLIO_PART_COLUMN + 1,          6.43  + COLUMN_GAP);
    doc.setColumnWidth(PORTFOLIO_YIELD_COLUMN + 1,         10.43 + COLUMN_GAP);
    doc.setColumnWidth(PORTFOLIO_YIELD_PERCENT_COLUMN + 1, 8.86  + COLUMN_GAP);
    doc.setColumnWidth(PORTFOLIO_DAILY_YIELD_COLUMN + 1,   9.86  + COLUMN_GAP);
    // clang-format on
    // NOLINTEND(readability-magic-numbers)

    doc.saveAs(path);
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
