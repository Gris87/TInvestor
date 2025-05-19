#pragma once



#include "src/widgets/tablerecords/operationstablerecord/ioperationstablerecord.h"

#include <QTableWidget>

#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidgetfactory.h"
#include "src/widgets/tableitems/integertableitem.h"
#include "src/widgets/tableitems/pricechangetableitem.h"
#include "src/widgets/tableitems/pricetableitem.h"
#include "src/widgets/tableitems/timetableitem.h"



class OperationsTableRecord : public IOperationsTableRecord
{
    Q_OBJECT

public:
    explicit OperationsTableRecord(
        QTableWidget*                      tableWidget,
        IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
        IUserStorage*                      userStorage,
        IInstrumentsStorage*               instrumentsStorage,
        QObject*                           parent = nullptr
    );
    ~OperationsTableRecord() override;

    OperationsTableRecord(const OperationsTableRecord& another)            = delete;
    OperationsTableRecord& operator=(const OperationsTableRecord& another) = delete;

    void setOperation(const Operation& operation) override;
    void exportToExcel(QXlsx::Document& doc) override;

private:
    [[nodiscard]]
    QXlsx::Format createRubleFormat(int precision) const;

    TimeTableItem*              mTimeTableWidgetItem;
    IInstrumentTableItemWidget* mInstrumentTableItemWidget;
    QTableWidgetItem*           mDescriptionTableWidgetItem;
    PriceTableItem*             mPriceTableWidgetItem;
    PriceTableItem*             mAvgPriceTableWidgetItem;
    IntegerTableItem*           mQuantityTableWidgetItem;
    IntegerTableItem*           mRemainedQuantityTableWidgetItem;
    PriceTableItem*             mPaymentTableWidgetItem;
    PriceTableItem*             mCommissionTableWidgetItem;
    PriceTableItem*             mYieldTableWidgetItem;
    PriceTableItem*             mYieldWithCommissionTableWidgetItem;
    PriceChangeTableItem*       mYieldWithCommissionPercentTableWidgetItem;
    PriceTableItem*             mRemainedMoneyTableWidgetItem;
    PriceTableItem*             mTotalMoneyTableWidgetItem;
    IInstrumentsStorage*        mInstrumentsStorage;
};
