#include "src/widgets/logstablewidget/logstablewidgetfactory.h"

#include <QDebug>

#include "src/widgets/logstablewidget/logstablewidget.h"



LogsTableWidgetFactory::LogsTableWidgetFactory() :
    ILogsTableWidgetFactory()
{
    qDebug() << "Create LogsTableWidgetFactory";
}

LogsTableWidgetFactory::~LogsTableWidgetFactory()
{
    qDebug() << "Destroy LogsTableWidgetFactory";
}

ILogsTableWidget* LogsTableWidgetFactory::newInstance(
    ILogsTableRecordFactory*         logsTableRecordFactory,
    ILogLevelTableItemWidgetFactory* logLevelTableItemWidgetFactory,
    IFileDialogFactory*              fileDialogFactory,
    ISettingsEditor*                 settingsEditor,
    QWidget*                         parent
) const
{
    return new LogsTableWidget(logsTableRecordFactory, logLevelTableItemWidgetFactory, fileDialogFactory, settingsEditor, parent);
}
