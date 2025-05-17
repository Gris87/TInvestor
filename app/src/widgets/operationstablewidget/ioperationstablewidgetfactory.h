#pragma once



#include "src/widgets/operationstablewidget/ioperationstablewidget.h"

#include "src/utils/filedialog/ifiledialogfactory.h"
#include "src/utils/messagebox/imessageboxutils.h"
#include "src/utils/settingseditor/isettingseditor.h"
#include "src/widgets/tablerecords/operationstablerecord/ioperationstablerecordfactory.h"



class IOperationsTableWidgetFactory
{
public:
    IOperationsTableWidgetFactory()          = default;
    virtual ~IOperationsTableWidgetFactory() = default;

    IOperationsTableWidgetFactory(const IOperationsTableWidgetFactory& another)            = delete;
    IOperationsTableWidgetFactory& operator=(const IOperationsTableWidgetFactory& another) = delete;

    virtual IOperationsTableWidget* newInstance(
        IOperationsTableRecordFactory*     operationsTableRecordFactory,
        IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
        IUserStorage*                      userStorage,
        IInstrumentsStorage*               instrumentsStorage,
        IFileDialogFactory*                fileDialogFactory,
        IMessageBoxUtils*                  messageBoxUtils,
        ISettingsEditor*                   settingsEditor,
        QWidget*                           parent
    ) const = 0;
};
