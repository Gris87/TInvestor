#include "src/widgets/portfoliotreewidget/portfoliotreewidget.h"
#include "ui_portfoliotreewidget.h"

#include <QDebug>
#include <QMenu>

#include "src/qxlsx/xlsxdocument.h"
#include "src/widgets/tabledelegates/instrumentitemdelegate.h"



#ifdef Q_OS_WINDOWS
const int COLUMN_WIDTHS[PORTFOLIO_COLUMN_COUNT] = {172, 80, 56, 124, 87, 56, 68, 79, 1100};
#else
const int COLUMN_WIDTHS[PORTFOLIO_COLUMN_COUNT] = {178, 87, 61, 132, 95, 59, 68, 83, 1100};
#endif

const QColor HEADER_BACKGROUND_COLOR = QColor("#354450"); // clazy:exclude=non-pod-global-static
const QColor HEADER_FONT_COLOR       = QColor("#699BA2"); // clazy:exclude=non-pod-global-static
const QColor GREEN_COLOR             = QColor("#2BD793"); // clazy:exclude=non-pod-global-static
const QColor RED_COLOR               = QColor("#ED6F7E"); // clazy:exclude=non-pod-global-static
const QColor NORMAL_COLOR            = QColor("#97AEC4"); // clazy:exclude=non-pod-global-static

constexpr float  ZERO_LIMIT      = 0.0001f;
constexpr float  HUNDRED_PERCENT = 100.0f;
constexpr double COLUMN_GAP      = 0.71;



PortfolioTreeWidget::PortfolioTreeWidget(
    IPortfolioTreeModelFactory* portfolioTreeModelFactory,
    IFileDialogFactory*         fileDialogFactory,
    ISettingsEditor*            settingsEditor,
    QWidget*                    parent
) :
    IPortfolioTreeWidget(parent),
    ui(new Ui::PortfolioTreeWidget),
    mFileDialogFactory(fileDialogFactory),
    mSettingsEditor(settingsEditor),
    mTotalCost(),
    mTotalDailyCost()
{
    qDebug() << "Create PortfolioTreeWidget";

    ui->setupUi(this);

    mPortfolioTreeModel = portfolioTreeModelFactory->newInstance(this);

    ui->treeView->setModel(mPortfolioTreeModel);
    ui->treeView->setItemDelegateForColumn(PORTFOLIO_NAME_COLUMN, new InstrumentItemDelegate(ui->treeView));
    ui->treeView->sortByColumn(PORTFOLIO_NAME_COLUMN, Qt::AscendingOrder);
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
    mPortfolioTreeModel->portfolioChanged(portfolio);
    ui->treeView->expandAll();

    mTotalCost      = mPortfolioTreeModel->totalCost();
    mTotalDailyCost = mPortfolioTreeModel->totalDailyCost();

    ui->costLabel->setText(QString::number(mTotalCost, 'f', 2) + " \u20BD");
    updateAllTimeLabel();
    updateForTodayLabel();
}

void PortfolioTreeWidget::lastPriceChanged(const QString& instrumentId, float price)
{
    mPortfolioTreeModel->lastPriceChanged(instrumentId, price);
}

void PortfolioTreeWidget::updateLastPrices()
{
    if (mPortfolioTreeModel->updateLastPrices())
    {
        updateAllTimeLabel();
        updateForTodayLabel();
    }
}

void PortfolioTreeWidget::updateAllTimeLabel()
{
    updateYieldLabel(ui->allTimeLabel, mPortfolioTreeModel->totalYield(), mTotalCost);
}

void PortfolioTreeWidget::updateForTodayLabel()
{
    updateYieldLabel(ui->forTodayLabel, mPortfolioTreeModel->totalDailyYield(), mTotalDailyCost);
}

void PortfolioTreeWidget::updateYieldLabel(QLabel* label, double yield, double cost)
{
    const QString prefix  = yield > 0 ? "+" : "";
    const float   percent = cost > 0 ? (yield / cost) * HUNDRED_PERCENT : 0.0f;

    label->setText(prefix + QString::number(yield, 'f', 2) + " \u20BD" + "(" + prefix + QString::number(percent, 'f', 2) + "%)");
    label->setToolTip(cost > 0 ? tr("From: %1").arg(cost, 0, 'f', 2) + " \u20BD" : "");

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

void PortfolioTreeWidget::on_treeView_customContextMenuRequested(const QPoint& pos)
{
    QMenu* contextMenu = new QMenu(this);

    contextMenu->addAction(tr("Export to Excel"), this, SLOT(actionExportToExcelTriggered()));

    contextMenu->popup(ui->treeView->viewport()->mapToGlobal(pos));
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

    for (int i = 0; i < PORTFOLIO_COLUMN_COUNT; ++i)
    {
        doc.write(1, i + 1, mPortfolioTreeModel->headerData(i, Qt::Horizontal).toString(), headerStyle);
    }

    mPortfolioTreeModel->exportToExcel(doc);

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
    mSettingsEditor->setValue(type + "/columnWidth_Name",         ui->treeView->columnWidth(PORTFOLIO_NAME_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Available",    ui->treeView->columnWidth(PORTFOLIO_AVAILABLE_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Price",        ui->treeView->columnWidth(PORTFOLIO_PRICE_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_AvgPrice",     ui->treeView->columnWidth(PORTFOLIO_AVG_PRICE_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Cost",         ui->treeView->columnWidth(PORTFOLIO_COST_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Part",         ui->treeView->columnWidth(PORTFOLIO_PART_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Yield",        ui->treeView->columnWidth(PORTFOLIO_YIELD_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_YieldPercent", ui->treeView->columnWidth(PORTFOLIO_YIELD_PERCENT_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_DailyYield",   ui->treeView->columnWidth(PORTFOLIO_DAILY_YIELD_COLUMN));
    // clang-format on
}

void PortfolioTreeWidget::loadWindowState(const QString& type)
{
    // clang-format off
    ui->treeView->setColumnWidth(PORTFOLIO_NAME_COLUMN,          mSettingsEditor->value(type + "/columnWidth_Name",         COLUMN_WIDTHS[PORTFOLIO_NAME_COLUMN]).toInt());
    ui->treeView->setColumnWidth(PORTFOLIO_AVAILABLE_COLUMN,     mSettingsEditor->value(type + "/columnWidth_Available",    COLUMN_WIDTHS[PORTFOLIO_AVAILABLE_COLUMN]).toInt());
    ui->treeView->setColumnWidth(PORTFOLIO_PRICE_COLUMN,         mSettingsEditor->value(type + "/columnWidth_Price",        COLUMN_WIDTHS[PORTFOLIO_PRICE_COLUMN]).toInt());
    ui->treeView->setColumnWidth(PORTFOLIO_AVG_PRICE_COLUMN,     mSettingsEditor->value(type + "/columnWidth_AvgPrice",     COLUMN_WIDTHS[PORTFOLIO_AVG_PRICE_COLUMN]).toInt());
    ui->treeView->setColumnWidth(PORTFOLIO_COST_COLUMN,          mSettingsEditor->value(type + "/columnWidth_Cost",         COLUMN_WIDTHS[PORTFOLIO_COST_COLUMN]).toInt());
    ui->treeView->setColumnWidth(PORTFOLIO_PART_COLUMN,          mSettingsEditor->value(type + "/columnWidth_Part",         COLUMN_WIDTHS[PORTFOLIO_PART_COLUMN]).toInt());
    ui->treeView->setColumnWidth(PORTFOLIO_YIELD_COLUMN,         mSettingsEditor->value(type + "/columnWidth_Yield",        COLUMN_WIDTHS[PORTFOLIO_YIELD_COLUMN]).toInt());
    ui->treeView->setColumnWidth(PORTFOLIO_YIELD_PERCENT_COLUMN, mSettingsEditor->value(type + "/columnWidth_YieldPercent", COLUMN_WIDTHS[PORTFOLIO_YIELD_PERCENT_COLUMN]).toInt());
    ui->treeView->setColumnWidth(PORTFOLIO_DAILY_YIELD_COLUMN,   mSettingsEditor->value(type + "/columnWidth_DailyYield",   COLUMN_WIDTHS[PORTFOLIO_DAILY_YIELD_COLUMN]).toInt());
    // clang-format on
}
