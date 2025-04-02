#include "src/widgets/logstablewidget/logstablewidget.h"
#include "ui_logstablewidget.h"

#include <QDebug>



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

void LogsTableWidget::saveWindowState(const QString& /*type*/)
{
}

void LogsTableWidget::loadWindowState(const QString& /*type*/)
{
}
