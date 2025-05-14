#pragma once



#include <QTableWidget>

#include "src/domain/stock/stock.h"
#include "src/threads/orderbook/iorderbookthread.h"
#include "src/utils/http/ihttpclient.h"
#include "src/widgets/tableitems/actions/iactionstableitemwidgetfactory.h"
#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidgetfactory.h"
#include "src/widgets/tablerecords/stockstablerecord/istockstablerecord.h"



class IStocksTableRecordFactory
{
public:
    IStocksTableRecordFactory()          = default;
    virtual ~IStocksTableRecordFactory() = default;

    IStocksTableRecordFactory(const IStocksTableRecordFactory& another)            = delete;
    IStocksTableRecordFactory& operator=(const IStocksTableRecordFactory& another) = delete;

    virtual IStocksTableRecord* newInstance(
        QTableWidget*                      tableWidget,
        IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
        IActionsTableItemWidgetFactory*    actionsTableItemWidgetFactory,
        IOrderWavesDialogFactory*          orderWavesDialogFactory,
        IOrderWavesWidgetFactory*          orderWavesWidgetFactory,
        IUserStorage*                      userStorage,
        IOrderBookThread*                  orderBookThread,
        IHttpClient*                       httpClient,
        Stock*                             stock,
        QObject*                           parent
    ) const = 0;
};
