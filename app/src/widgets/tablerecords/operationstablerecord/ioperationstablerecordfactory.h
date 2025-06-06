#pragma once



#include <QTableWidget>

#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/storage/user/iuserstorage.h"
#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidgetfactory.h"
#include "src/widgets/tablerecords/operationstablerecord/ioperationstablerecord.h"



class IOperationsTableRecordFactory
{
public:
    IOperationsTableRecordFactory()          = default;
    virtual ~IOperationsTableRecordFactory() = default;

    IOperationsTableRecordFactory(const IOperationsTableRecordFactory& another)            = delete;
    IOperationsTableRecordFactory& operator=(const IOperationsTableRecordFactory& another) = delete;

    virtual IOperationsTableRecord* newInstance(
        QTableWidget*                      tableWidget,
        IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
        IUserStorage*                      userStorage,
        IInstrumentsStorage*               instrumentsStorage,
        QObject*                           parent
    ) const = 0;
};
