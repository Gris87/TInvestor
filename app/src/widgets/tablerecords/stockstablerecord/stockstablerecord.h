#pragma once



#include "src/widgets/tablerecords/stockstablerecord/istockstablerecord.h"

#include "src/domain/stocks/stock.h"
#include "src/threads/orderbook/iorderbookthread.h"
#include "src/utils/http/ihttpclient.h"
#include "src/widgets/tableitems/actions/iactionstableitemwidgetfactory.h"
#include "src/widgets/tableitems/paybacktableitem.h"
#include "src/widgets/tableitems/pricechangetableitem.h"
#include "src/widgets/tableitems/pricetableitem.h"
#include "src/widgets/tableitems/stock/istocktableitemwidgetfactory.h"
#include "src/widgets/tableitems/turnovertableitem.h"



class TableRecord : public ITableRecord
{
    Q_OBJECT

public:
    explicit TableRecord(
        QTableWidget*                   tableWidget,
        IStockTableItemWidgetFactory*   stockTableItemWidgetFactory,
        IActionsTableItemWidgetFactory* actionsTableItemWidgetFactory,
        IOrderWavesDialogFactory*       orderWavesDialogFactory,
        IOrderWavesWidgetFactory*       orderWavesWidgetFactory,
        IUserStorage*                   userStorage,
        IOrderBookThread*               orderBookThread,
        IHttpClient*                    httpClient,
        Stock*                          stock,
        QObject*                        parent = nullptr
    );
    ~TableRecord();

    TableRecord(const TableRecord& another)            = delete;
    TableRecord& operator=(const TableRecord& another) = delete;

    void updateAll() override;
    void updatePrice() override;
    void updatePeriodicData() override;
    void filter(QTableWidget* tableWidget, const Filter& filter) override;

private:
    Stock*                 mStock;
    IStockTableItemWidget* mStockTableItemWidget;
    PriceTableItem*        mPriceTableWidgetItem;
    PriceChangeTableItem*  mDayChangeTableWidgetItem;
    PriceChangeTableItem*  mDateChangeTableWidgetItem;
    TurnoverTableItem*     mTurnoverTableWidgetItem;
    PaybackTableItem*      mPaybackTableWidgetItem;
    int                    mPrecision;
};
