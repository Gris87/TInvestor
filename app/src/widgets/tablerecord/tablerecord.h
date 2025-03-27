#pragma once



#include "src/widgets/tablerecord/itablerecord.h"

#include "src/domain/stocks/stock.h"
#include "src/threads/orderbook/iorderbookthread.h"
#include "src/utils/http/ihttpclient.h"
#include "src/widgets/tablerecord/items/actions/iactionstableitemwidgetfactory.h"
#include "src/widgets/tablerecord/items/paybacktableitem.h"
#include "src/widgets/tablerecord/items/pricechangetableitem.h"
#include "src/widgets/tablerecord/items/pricetableitem.h"
#include "src/widgets/tablerecord/items/stock/istocktableitemwidgetfactory.h"



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
    void updatePayback() override;
    void filter(QTableWidget* tableWidget, const Filter& filter) override;

private:
    Stock*                 mStock;
    IStockTableItemWidget* mStockTableItemWidget;
    PriceTableItem*        mPriceTableWidgetItem;
    PriceChangeTableItem*  mDayChangeTableWidgetItem;
    PriceChangeTableItem*  mDateChangeTableWidgetItem;
    PaybackTableItem*      mPaybackTableWidgetItem;
    int                    mPrecision;
};
