#pragma once



#include "src/widgets/logstablewidget/ilogstablewidgetfactory.h"



class LogsTableWidgetFactory : public ILogsTableWidgetFactory
{
public:
    LogsTableWidgetFactory();
    ~LogsTableWidgetFactory() override;

    LogsTableWidgetFactory(const LogsTableWidgetFactory& another)            = delete;
    LogsTableWidgetFactory& operator=(const LogsTableWidgetFactory& another) = delete;

    ILogsTableWidget* newInstance(
        ILogsTableModelFactory*            logsTableModelFactory,
        ILogsTableRecordFactory*           logsTableRecordFactory,
        ILogLevelTableItemWidgetFactory*   logLevelTableItemWidgetFactory,
        IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
        IUserStorage*                      userStorage,
        IInstrumentsStorage*               instrumentsStorage,
        ILogosStorage*                     logosStorage,
        IFileDialogFactory*                fileDialogFactory,
        ISettingsEditor*                   settingsEditor,
        QWidget*                           parent
    ) const override;
};
