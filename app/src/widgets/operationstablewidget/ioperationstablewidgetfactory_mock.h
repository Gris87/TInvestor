#pragma once



#include "src/widgets/operationstablewidget/ioperationstablewidgetfactory.h"

#include <gmock/gmock.h>



class OperationsTableWidgetFactoryMock : public IOperationsTableWidgetFactory
{
public:
    OperationsTableWidgetFactoryMock() :
        IOperationsTableWidgetFactory()
    {
    }
    ~OperationsTableWidgetFactoryMock() override = default;

    OperationsTableWidgetFactoryMock(const OperationsTableWidgetFactoryMock& another)            = delete;
    OperationsTableWidgetFactoryMock& operator=(const OperationsTableWidgetFactoryMock& another) = delete;

    MOCK_METHOD(
        IOperationsTableWidget*,
        newInstance,
        (IOperationsTableRecordFactory * operationsTableRecordFactory,
         IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
         IUserStorage*                      userStorage,
         IInstrumentsStorage*               instrumentsStorage,
         IFileFactory*                      fileFactory,
         IFileDialogFactory*                fileDialogFactory,
         IMessageBoxUtils*                  mMessageBoxUtils,
         ISettingsEditor*                   settingsEditor,
         QWidget*                           parent),
        (const, override)
    );
};
