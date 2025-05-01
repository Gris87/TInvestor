#include "src/widgets/logstablewidget/logstablewidget.h"
#include "ui_logstablewidget.h"

#include <QDebug>



// TODO: Move to LogsTableRecord
enum LogsTableColumns : qint8
{
    LOGS_TIME_COLUMN,
    LOGS_MESSAGE_COLUMN,
    COLUMN_COUNT,
};



#ifdef Q_OS_WINDOWS
const int COLUMN_WIDTHS[COLUMN_COUNT] = {64, 94};
#else
const int COLUMN_WIDTHS[COLUMN_COUNT] = {67, 100};
#endif



LogsTableWidget::LogsTableWidget(ISettingsEditor* settingsEditor, QWidget* parent) :
    ILogsTableWidget(parent),
    ui(new Ui::LogsTableWidget),
    mSettingsEditor(settingsEditor)
{
    qDebug() << "Create LogsTableWidget";

    ui->setupUi(this);
}

LogsTableWidget::~LogsTableWidget()
{
    qDebug() << "Destroy LogsTableWidget";

    delete ui;
}

void LogsTableWidget::saveWindowState(const QString& type)
{
    // clang-format off
    mSettingsEditor->setValue(type + "/columnWidth_Time",    ui->tableWidget->columnWidth(LOGS_TIME_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Message", ui->tableWidget->columnWidth(LOGS_MESSAGE_COLUMN));
    // clang-format on
}

// NOLINTBEGIN(readability-magic-numbers)
void LogsTableWidget::loadWindowState(const QString& type)
{
    // clang-format off
    ui->tableWidget->setColumnWidth(LOGS_TIME_COLUMN,    mSettingsEditor->value(type + "/columnWidth_Time",    COLUMN_WIDTHS[LOGS_TIME_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(LOGS_MESSAGE_COLUMN, mSettingsEditor->value(type + "/columnWidth_Message", COLUMN_WIDTHS[LOGS_MESSAGE_COLUMN]).toInt());
    // clang-format on
}
// NOLINTEND(readability-magic-numbers)
