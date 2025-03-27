#pragma once



#include "src/widgets/tablerecord/itablerecord.h"

#include <QTableWidget>

#include "src/domain/stocks/stock.h"
#include "src/threads/orderbook/iorderbookthread.h"
#include "src/utils/http/ihttpclient.h"
#include "src/widgets/tablerecord/items/actions/iactionstableitemwidgetfactory.h"
#include "src/widgets/tablerecord/items/stock/istocktableitemwidgetfactory.h"



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
