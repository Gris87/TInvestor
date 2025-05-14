#pragma once



#include "src/widgets/tablerecords/operationstablerecord/ioperationstablerecordfactory.h"

#include <gmock/gmock.h>



class OperationsTableRecordFactoryMock : public IOperationsTableRecordFactory
{
public:
    OperationsTableRecordFactoryMock() :
        IOperationsTableRecordFactory()
    {
    }
    ~OperationsTableRecordFactoryMock() override = default;

    OperationsTableRecordFactoryMock(const OperationsTableRecordFactoryMock& another)            = delete;
    OperationsTableRecordFactoryMock& operator=(const OperationsTableRecordFactoryMock& another) = delete;

    MOCK_METHOD(
        IOperationsTableRecord*,
        newInstance,
        (QTableWidget * tableWidget,
         IStockTableItemWidgetFactory* stockTableItemWidgetFactory,
         IUserStorage*                 userStorage,
         IInstrumentsStorage*          instrumentsStorage,
         QObject*                      parent),
        (const, override)
    );
};
