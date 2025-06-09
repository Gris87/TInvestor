#pragma once



#include "src/widgets/logstablewidget/ilogstablewidget.h"

#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/utils/filedialog/ifiledialogfactory.h"
#include "src/utils/settingseditor/isettingseditor.h"
#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidgetfactory.h"
#include "src/widgets/tablerecords/logstablerecord/ilogstablerecordfactory.h"



class ILogsTableWidgetFactory
{
public:
    ILogsTableWidgetFactory()          = default;
    virtual ~ILogsTableWidgetFactory() = default;

    ILogsTableWidgetFactory(const ILogsTableWidgetFactory& another)            = delete;
    ILogsTableWidgetFactory& operator=(const ILogsTableWidgetFactory& another) = delete;

    virtual ILogsTableWidget* newInstance(
        ILogsTableRecordFactory*           logsTableRecordFactory,
        ILogLevelTableItemWidgetFactory*   logLevelTableItemWidgetFactory,
        IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
        IUserStorage*                      userStorage,
        IInstrumentsStorage*               instrumentsStorage,
        IFileDialogFactory*                fileDialogFactory,
        ISettingsEditor*                   settingsEditor,
        QWidget*                           parent
    ) const = 0;
};
