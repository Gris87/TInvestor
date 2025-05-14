#pragma once



#include "src/widgets/tablerecords/operationstablerecord/ioperationstablerecord.h"

#include <QTableWidget>

#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/widgets/tableitems/stock/istocktableitemwidgetfactory.h"
#include "src/widgets/tableitems/timetableitem.h"



class OperationsTableRecord : public IOperationsTableRecord
{
    Q_OBJECT

public:
    explicit OperationsTableRecord(
        QTableWidget*                 tableWidget,
        IStockTableItemWidgetFactory* stockTableItemWidgetFactory,
        IUserStorage*                 userStorage,
        IInstrumentsStorage*          instrumentsStorage,
        QObject*                      parent = nullptr
    );
    ~OperationsTableRecord() override;

    OperationsTableRecord(const OperationsTableRecord& another)            = delete;
    OperationsTableRecord& operator=(const OperationsTableRecord& another) = delete;

    void setOperation(const Operation& operation) override;

private:
    TimeTableItem* mTimeTableWidgetItem;
    IStockTableItemWidget* mStockTableItemWidget;
    IInstrumentsStorage*   mInstrumentsStorage;
};
