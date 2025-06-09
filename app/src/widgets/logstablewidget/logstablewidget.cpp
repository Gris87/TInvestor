#include "src/widgets/logstablewidget/logstablewidget.h"
#include "ui_logstablewidget.h"

#include <QDebug>
#include <QMenu>

#include "src/qxlsx/xlsxdocument.h"



// TODO: Update widths
#ifdef Q_OS_WINDOWS
const int COLUMN_WIDTHS[LOGS_COLUMN_COUNT] = {110, 32, 111, 900};
#else
const int COLUMN_WIDTHS[LOGS_COLUMN_COUNT] = {110, 32, 10, 900};
#endif

const QColor HEADER_BACKGROUND_COLOR = QColor("#354450"); // clazy:exclude=non-pod-global-static
const QColor HEADER_FONT_COLOR       = QColor("#699BA2"); // clazy:exclude=non-pod-global-static

constexpr double COLUMN_GAP = 0.71;



LogsTableWidget::LogsTableWidget(
    ILogsTableRecordFactory*           logsTableRecordFactory,
    ILogLevelTableItemWidgetFactory*   logLevelTableItemWidgetFactory,
    IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
    IUserStorage*                      userStorage,
    IInstrumentsStorage*               instrumentsStorage,
    IFileDialogFactory*                fileDialogFactory,
    ISettingsEditor*                   settingsEditor,
    QWidget*                           parent
) :
    ILogsTableWidget(parent),
    ui(new Ui::LogsTableWidget),
    mLogsTableRecordFactory(logsTableRecordFactory),
    mLogLevelTableItemWidgetFactory(logLevelTableItemWidgetFactory),
    mInstrumentTableItemWidgetFactory(instrumentTableItemWidgetFactory),
    mUserStorage(userStorage),
    mInstrumentsStorage(instrumentsStorage),
    mFileDialogFactory(fileDialogFactory),
    mSettingsEditor(settingsEditor),
    mFilterLogLevel(),
    mRecords()
{
    qDebug() << "Create LogsTableWidget";

    ui->setupUi(this);

    ui->tableWidget->sortByColumn(LOGS_TIME_COLUMN, Qt::DescendingOrder);
}

LogsTableWidget::~LogsTableWidget()
{
    qDebug() << "Destroy LogsTableWidget";

    delete ui;
}

void LogsTableWidget::setFilter(LogLevel level)
{
    if (mFilterLogLevel != level)
    {
        ui->tableWidget->setUpdatesEnabled(false);
        mFilterLogLevel = level;

        for (ILogsTableRecord* record : std::as_const(mRecords))
        {
            record->filter(ui->tableWidget, mFilterLogLevel);
        }

        ui->tableWidget->setUpdatesEnabled(true);
    }
}

void LogsTableWidget::logsRead(const QList<LogEntry>& entries)
{
    ui->tableWidget->setUpdatesEnabled(false);
    ui->tableWidget->setSortingEnabled(false);

    if (mRecords.size() > entries.size())
    {
        while (mRecords.size() > entries.size())
        {
            delete mRecords.takeLast();
        }

        ui->tableWidget->setRowCount(entries.size());
    }

    while (mRecords.size() < entries.size())
    {
        ILogsTableRecord* record = mLogsTableRecordFactory->newInstance(
            ui->tableWidget,
            mLogLevelTableItemWidgetFactory,
            mInstrumentTableItemWidgetFactory,
            mUserStorage,
            mInstrumentsStorage,
            this
        );
        mRecords.append(record);
    }

    for (int i = 0; i < entries.size(); ++i)
    {
        ILogsTableRecord* record = mRecords.at(i);

        record->setLogEntry(entries.at(i));
        record->filter(ui->tableWidget, mFilterLogLevel);
    }

    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->setUpdatesEnabled(true);
}

void LogsTableWidget::logAdded(const LogEntry& entry)
{
    ui->tableWidget->setUpdatesEnabled(false);
    ui->tableWidget->setSortingEnabled(false);

    ILogsTableRecord* record = mLogsTableRecordFactory->newInstance(
        ui->tableWidget,
        mLogLevelTableItemWidgetFactory,
        mInstrumentTableItemWidgetFactory,
        mUserStorage,
        mInstrumentsStorage,
        this
    );
    record->setLogEntry(entry);
    record->filter(ui->tableWidget, mFilterLogLevel);

    mRecords.append(record);

    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->setUpdatesEnabled(true);
}

void LogsTableWidget::on_tableWidget_customContextMenuRequested(const QPoint& pos)
{
    QMenu* contextMenu = new QMenu(this);

    contextMenu->addAction(tr("Export to Excel"), this, SLOT(actionExportToExcelTriggered()));

    contextMenu->popup(ui->tableWidget->viewport()->mapToGlobal(pos));
}

void LogsTableWidget::actionExportToExcelTriggered()
{
    const QString lastFile = mSettingsEditor->value("MainWindow/LogsTableWidget/exportToExcelFile", "").toString();

    const std::shared_ptr<IFileDialog> fileDialog = mFileDialogFactory->newInstance(
        this, tr("Export"), lastFile.left(lastFile.lastIndexOf("/")), tr("Excel file") + " (*.xlsx)"
    );
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);
    fileDialog->setDefaultSuffix("xlsx");

    fileDialog->selectFile(lastFile);

    if (fileDialog->exec() == QDialog::Accepted)
    {
        const QString path = fileDialog->selectedFiles().at(0);
        mSettingsEditor->setValue("MainWindow/LogsTableWidget/exportToExcelFile", path);

        exportToExcel(path);
    }
}

void LogsTableWidget::exportToExcel(const QString& path) const
{
    QXlsx::Document doc;
    doc.addSheet(tr("Logs"));

    QXlsx::Format headerStyle;
    headerStyle.setFontBold(true);
    headerStyle.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    headerStyle.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    headerStyle.setFillPattern(QXlsx::Format::PatternSolid);
    headerStyle.setBorderStyle(QXlsx::Format::BorderThin);
    headerStyle.setPatternBackgroundColor(HEADER_BACKGROUND_COLOR);
    headerStyle.setFontColor(HEADER_FONT_COLOR);

    for (int i = 0; i < ui->tableWidget->columnCount(); ++i)
    {
        doc.write(1, i + 1, ui->tableWidget->horizontalHeaderItem(i)->text(), headerStyle);
    }

    for (ILogsTableRecord* record : std::as_const(mRecords))
    {
        record->exportToExcel(doc);
    }

    // NOLINTBEGIN(readability-magic-numbers)
    // clang-format off
    doc.setColumnWidth(LOGS_TIME_COLUMN + 1,  17.57 + COLUMN_GAP);
    doc.setColumnWidth(LOGS_LEVEL_COLUMN + 1, 9.29  + COLUMN_GAP);
    doc.autosizeColumnWidth(LOGS_NAME_COLUMN + 1);
    doc.autosizeColumnWidth(LOGS_MESSAGE_COLUMN + 1);
    // clang-format on
    // NOLINTEND(readability-magic-numbers)

    doc.saveAs(path);
}

void LogsTableWidget::saveWindowState(const QString& type)
{
    // clang-format off
    mSettingsEditor->setValue(type + "/columnWidth_Time",    ui->tableWidget->columnWidth(LOGS_TIME_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Level",   ui->tableWidget->columnWidth(LOGS_LEVEL_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Name",    ui->tableWidget->columnWidth(LOGS_NAME_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Message", ui->tableWidget->columnWidth(LOGS_MESSAGE_COLUMN));
    // clang-format on
}

void LogsTableWidget::loadWindowState(const QString& type)
{
    // clang-format off
    ui->tableWidget->setColumnWidth(LOGS_TIME_COLUMN,    mSettingsEditor->value(type + "/columnWidth_Time",    COLUMN_WIDTHS[LOGS_TIME_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(LOGS_LEVEL_COLUMN,   mSettingsEditor->value(type + "/columnWidth_Level",   COLUMN_WIDTHS[LOGS_LEVEL_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(LOGS_NAME_COLUMN,    mSettingsEditor->value(type + "/columnWidth_Name",    COLUMN_WIDTHS[LOGS_NAME_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(LOGS_MESSAGE_COLUMN, mSettingsEditor->value(type + "/columnWidth_Message", COLUMN_WIDTHS[LOGS_MESSAGE_COLUMN]).toInt());
    // clang-format on
}
