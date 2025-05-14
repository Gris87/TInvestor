#pragma once



#include "src/widgets/tablerecords/operationstablerecord/ioperationstablerecordfactory.h"



class OperationsTableRecordFactory : public IOperationsTableRecordFactory
{
public:
    OperationsTableRecordFactory();
    ~OperationsTableRecordFactory() override;

    OperationsTableRecordFactory(const OperationsTableRecordFactory& another)            = delete;
    OperationsTableRecordFactory& operator=(const OperationsTableRecordFactory& another) = delete;

    IOperationsTableRecord* newInstance(
        QTableWidget*                 tableWidget,
        IStockTableItemWidgetFactory* stockTableItemWidgetFactory,
        IUserStorage*                 userStorage,
        IInstrumentsStorage*          instrumentsStorage,
        QObject*                      parent
    ) const override;
};
