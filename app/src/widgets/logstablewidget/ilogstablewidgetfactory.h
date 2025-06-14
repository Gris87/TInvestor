#pragma once



#include "src/widgets/logstablewidget/ilogstablewidget.h"

#include "src/storage/logos/ilogosstorage.h"
#include "src/utils/filedialog/ifiledialogfactory.h"
#include "src/utils/settingseditor/isettingseditor.h"
#include "src/widgets/tablemodels/logstablemodel/ilogstablemodelfactory.h"



class ILogsTableWidgetFactory
{
public:
    ILogsTableWidgetFactory()          = default;
    virtual ~ILogsTableWidgetFactory() = default;

    ILogsTableWidgetFactory(const ILogsTableWidgetFactory& another)            = delete;
    ILogsTableWidgetFactory& operator=(const ILogsTableWidgetFactory& another) = delete;

    virtual ILogsTableWidget* newInstance(
        ILogsTableModelFactory* logsTableModelFactory,
        ILogosStorage*          logosStorage,
        IFileDialogFactory*     fileDialogFactory,
        ISettingsEditor*        settingsEditor,
        QWidget*                parent
    ) const = 0;
};
