#pragma once



#include "src/widgets/stockstablewidget/istockstablewidget.h"

#include "src/dialogs/orderwavesdialog/iorderwavesdialogfactory.h"
#include "src/storage/user/iuserstorage.h"
#include "src/threads/orderbook/iorderbookthread.h"
#include "src/utils/http/ihttpclient.h"
#include "src/utils/settingseditor/isettingseditor.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory.h"
#include "src/widgets/tablerecord/itablerecordfactory.h"



class IStocksTableWidgetFactory
{
public:
    IStocksTableWidgetFactory()
    {
    }
    virtual ~IStocksTableWidgetFactory() = default;

    IStocksTableWidgetFactory(const IStocksTableWidgetFactory& another)            = delete;
    IStocksTableWidgetFactory& operator=(const IStocksTableWidgetFactory& another) = delete;

    virtual IStocksTableWidget* newInstance(
        ITableRecordFactory*            tableRecordFactory,
        IStockTableItemWidgetFactory*   stockTableItemWidgetFactory,
        IActionsTableItemWidgetFactory* actionsTableItemWidgetFactory,
        IOrderWavesDialogFactory*       orderWavesDialogFactory,
        IOrderWavesWidgetFactory*       orderWavesWidgetFactory,
        IUserStorage*                   userStorage,
        IOrderBookThread*               orderBookThread,
        IHttpClient*                    httpClient,
        ISettingsEditor*                settingsEditor,
        QWidget*                        parent
    ) = 0;
};
