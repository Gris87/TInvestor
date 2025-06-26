#pragma once



#include "src/widgets/operationstablewidget/ioperationstablewidget.h"

#include "src/storage/logos/ilogosstorage.h"
#include "src/utils/filedialog/ifiledialogfactory.h"
#include "src/utils/settingseditor/isettingseditor.h"
#include "src/widgets/tablemodels/operationstablemodel/ioperationstablemodelfactory.h"



class IOperationsTableWidgetFactory
{
public:
    IOperationsTableWidgetFactory()          = default;
    virtual ~IOperationsTableWidgetFactory() = default;

    IOperationsTableWidgetFactory(const IOperationsTableWidgetFactory& another)            = delete;
    IOperationsTableWidgetFactory& operator=(const IOperationsTableWidgetFactory& another) = delete;

    virtual IOperationsTableWidget* newInstance(
        IOperationsTableModelFactory* operationsTableModelFactory,
        IFileDialogFactory*           fileDialogFactory,
        ISettingsEditor*              settingsEditor,
        QWidget*                      parent
    ) const = 0;
};
