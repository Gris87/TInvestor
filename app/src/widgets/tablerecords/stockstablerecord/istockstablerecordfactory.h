#pragma once



#include "src/widgets/tablerecords/stockstablerecord/istockstablerecord.h"

#include <QTableWidget>

#include "src/domain/stocks/stock.h"
#include "src/threads/orderbook/iorderbookthread.h"
#include "src/utils/http/ihttpclient.h"
#include "src/widgets/tableitems/actions/iactionstableitemwidgetfactory.h"
#include "src/widgets/tableitems/stock/istocktableitemwidgetfactory.h"



class ITableRecordFactory
{
public:
    ITableRecordFactory()
    {
    }
    virtual ~ITableRecordFactory() = default;

    ITableRecordFactory(const ITableRecordFactory& another)            = delete;
    ITableRecordFactory& operator=(const ITableRecordFactory& another) = delete;

    virtual ITableRecord* newInstance(
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
