#include "src/widgets/logstablewidget/logstablewidget.h"
#include "ui_logstablewidget.h"

#include <QDebug>



// TODO: Move to TableRecord
#define LOGS_TIME_COLUMN    0
#define LOGS_MESSAGE_COLUMN 1



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

void LogsTableWidget::loadWindowState(const QString& type)
{
    // clang-format off
    ui->tableWidget->setColumnWidth(LOGS_TIME_COLUMN,    mSettingsEditor->value(type + "/columnWidth_Time",    42).toInt());
    ui->tableWidget->setColumnWidth(LOGS_MESSAGE_COLUMN, mSettingsEditor->value(type + "/columnWidth_Message", 72).toInt());
    // clang-format on
}
