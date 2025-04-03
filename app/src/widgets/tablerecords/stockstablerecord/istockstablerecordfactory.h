#pragma once



#include "src/widgets/tablerecords/stockstablerecord/istockstablerecord.h"

#include <QTableWidget>

#include "src/domain/stocks/stock.h"
#include "src/threads/orderbook/iorderbookthread.h"
#include "src/utils/http/ihttpclient.h"
#include "src/widgets/tableitems/actions/iactionstableitemwidgetfactory.h"
#include "src/widgets/tableitems/stock/istocktableitemwidgetfactory.h"



class IStocksTableRecordFactory
{
public:
    IStocksTableRecordFactory()
    {
    }
    virtual ~IStocksTableRecordFactory() = default;

    IStocksTableRecordFactory(const IStocksTableRecordFactory& another)            = delete;
    IStocksTableRecordFactory& operator=(const IStocksTableRecordFactory& another) = delete;

    virtual IStocksTableRecord* newInstance(
        QTableWidget*                   tableWidget,
        IStockTableItemWidgetFactory*   stockTableItemWidgetFactory,
        IActionsTableItemWidgetFactory* actionsTableItemWidgetFactory,
        IOrderWavesDialogFactory*       orderWavesDialogFactory,
        IOrderWavesWidgetFactory*       orderWavesWidgetFactory,
        IUserStorage*                   userStorage,
        IOrderBookThread*               orderBookThread,
        IHttpClient*                    httpClient,
        Stock*                          stock,
        QObject*                        parent
    ) = 0;
};
