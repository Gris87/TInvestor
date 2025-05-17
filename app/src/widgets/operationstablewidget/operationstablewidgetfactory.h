#pragma once



#include "src/widgets/operationstablewidget/ioperationstablewidgetfactory.h"



class OperationsTableWidgetFactory : public IOperationsTableWidgetFactory
{
public:
    OperationsTableWidgetFactory();
    ~OperationsTableWidgetFactory() override;

    OperationsTableWidgetFactory(const OperationsTableWidgetFactory& another)            = delete;
    OperationsTableWidgetFactory& operator=(const OperationsTableWidgetFactory& another) = delete;

    IOperationsTableWidget* newInstance(
        IOperationsTableRecordFactory*     operationsTableRecordFactory,
        IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
        IUserStorage*                      userStorage,
        IInstrumentsStorage*               instrumentsStorage,
        IFileDialogFactory*                fileDialogFactory,
        IMessageBoxUtils*                  mMessageBoxUtils,
        ISettingsEditor*                   settingsEditor,
        QWidget*                           parent
    ) const override;
};
