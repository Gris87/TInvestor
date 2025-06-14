#pragma once



#include "src/widgets/tablerecords/stockstablerecord/istockstablerecord.h"

#include "src/domain/stock/stock.h"
#include "src/threads/orderbook/iorderbookthread.h"
#include "src/utils/http/ihttpclient.h"
#include "src/widgets/tableitems/actions/iactionstableitemwidgetfactory.h"
#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidgetfactory.h"
#include "src/widgets/tableitems/moneychangepercenttableitem.h"
#include "src/widgets/tableitems/moneytableitem.h"
#include "src/widgets/tableitems/paybacktableitem.h"
#include "src/widgets/tableitems/turnovertableitem.h"



class StocksTableRecord : public IStocksTableRecord
{
    Q_OBJECT

public:
    explicit StocksTableRecord(
        QTableWidget*                      tableWidget,
        IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
        IActionsTableItemWidgetFactory*    actionsTableItemWidgetFactory,
        IOrderWavesDialogFactory*          orderWavesDialogFactory,
        IOrderWavesWidgetFactory*          orderWavesWidgetFactory,
        IUserStorage*                      userStorage,
        IOrderBookThread*                  orderBookThread,
        IHttpClient*                       httpClient,
        Stock*                             stock,
        QObject*                           parent = nullptr
    );
    ~StocksTableRecord() override;

    StocksTableRecord(const StocksTableRecord& another)            = delete;
    StocksTableRecord& operator=(const StocksTableRecord& another) = delete;

    void updateAll() override;
    void updatePrice() override;
    void updatePeriodicData() override;
    void filter(QTableWidget* tableWidget, const StockFilter& filter) override;
    void exportToExcel(QXlsx::Document& doc) const override;

private:
    [[nodiscard]]
    QXlsx::Format createRubleFormat(const QColor& color, int precision) const;

    [[nodiscard]]
    QXlsx::Format createPercentFormat(const QColor& color, bool withPlus) const;

    Stock*                       mStock;
    IInstrumentTableItemWidget*  mInstrumentTableItemWidget;
    MoneyTableItem*              mPriceTableWidgetItem;
    MoneyChangePercentTableItem* mDayChangeTableWidgetItem;
    MoneyChangePercentTableItem* mDateChangeTableWidgetItem;
    TurnoverTableItem*           mTurnoverTableWidgetItem;
    PaybackTableItem*            mPaybackTableWidgetItem;
    qint8                        mPrecision;
};
