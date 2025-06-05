#pragma once



#include "src/widgets/logstablewidget/ilogstablewidget.h"

#include "src/utils/filedialog/ifiledialogfactory.h"
#include "src/utils/settingseditor/isettingseditor.h"
#include "src/widgets/tablerecords/logstablerecord/ilogstablerecordfactory.h"



class ILogsTableWidgetFactory
{
public:
    ILogsTableWidgetFactory()          = default;
    virtual ~ILogsTableWidgetFactory() = default;

    ILogsTableWidgetFactory(const ILogsTableWidgetFactory& another)            = delete;
    ILogsTableWidgetFactory& operator=(const ILogsTableWidgetFactory& another) = delete;

    virtual ILogsTableWidget* newInstance(
        ILogsTableRecordFactory* logsTableRecordFactory,
        IFileDialogFactory*      fileDialogFactory,
        ISettingsEditor*         settingsEditor,
        QWidget*                 parent
    ) const = 0;
};
