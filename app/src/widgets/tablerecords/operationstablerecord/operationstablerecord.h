#pragma once



#include "src/widgets/tablerecords/operationstablerecord/ioperationstablerecord.h"

#include <QTableWidget>

#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidgetfactory.h"
#include "src/widgets/tableitems/integertableitem.h"
#include "src/widgets/tableitems/moneychangepercenttableitem.h"
#include "src/widgets/tableitems/moneychangetableitem.h"
#include "src/widgets/tableitems/moneytableitem.h"
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
    void exportToExcel(QXlsx::Document& doc) const override;

private:
    [[nodiscard]]
    QXlsx::Format createRubleFormat(const QColor& color, bool withPlus, int precision) const;

    [[nodiscard]]
    QXlsx::Format createPercentFormat(const QColor& color, bool withPlus) const;

    TimeTableItem*               mTimeTableWidgetItem;
    IInstrumentTableItemWidget*  mInstrumentTableItemWidget;
    QTableWidgetItem*            mDescriptionTableWidgetItem;
    MoneyTableItem*              mPriceTableWidgetItem;
    MoneyTableItem*              mAvgPriceTableWidgetItem;
    IntegerTableItem*            mQuantityTableWidgetItem;
    IntegerTableItem*            mRemainedQuantityTableWidgetItem;
    MoneyChangeTableItem*        mPaymentTableWidgetItem;
    MoneyChangeTableItem*        mCommissionTableWidgetItem;
    MoneyChangeTableItem*        mYieldTableWidgetItem;
    MoneyChangeTableItem*        mYieldWithCommissionTableWidgetItem;
    MoneyChangePercentTableItem* mYieldWithCommissionPercentTableWidgetItem;
    MoneyTableItem*              mRemainedMoneyTableWidgetItem;
    MoneyTableItem*              mTotalMoneyTableWidgetItem;
    IInstrumentsStorage*         mInstrumentsStorage;
};
