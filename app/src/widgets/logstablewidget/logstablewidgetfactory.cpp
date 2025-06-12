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
    ILogsTableModelFactory*            logsTableModelFactory,
    ILogsTableRecordFactory*           logsTableRecordFactory,
    ILogLevelTableItemWidgetFactory*   logLevelTableItemWidgetFactory,
    IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
    IUserStorage*                      userStorage,
    IInstrumentsStorage*               instrumentsStorage,
    IFileDialogFactory*                fileDialogFactory,
    ISettingsEditor*                   settingsEditor,
    QWidget*                           parent
) const
{
    return new LogsTableWidget(
        logsTableModelFactory,
        logsTableRecordFactory,
        logLevelTableItemWidgetFactory,
        instrumentTableItemWidgetFactory,
        userStorage,
        instrumentsStorage,
        fileDialogFactory,
        settingsEditor,
        parent
    );
}
