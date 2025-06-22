#pragma once



#include "src/widgets/stockstablewidget/istockstablewidget.h"

#include "src/dialogs/orderwavesdialog/iorderwavesdialogfactory.h"
#include "src/storage/logos/ilogosstorage.h"
#include "src/storage/user/iuserstorage.h"
#include "src/threads/orderbook/iorderbookthread.h"
#include "src/utils/filedialog/ifiledialogfactory.h"
#include "src/utils/http/ihttpclient.h"
#include "src/utils/settingseditor/isettingseditor.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory.h"
#include "src/widgets/tablemodels/stockstablemodel/istockstablemodelfactory.h"
#include "src/widgets/tablerecords/stockstablerecord/istockstablerecordfactory.h"



class IStocksTableWidgetFactory
{
public:
    IStocksTableWidgetFactory()          = default;
    virtual ~IStocksTableWidgetFactory() = default;

    IStocksTableWidgetFactory(const IStocksTableWidgetFactory& another)            = delete;
    IStocksTableWidgetFactory& operator=(const IStocksTableWidgetFactory& another) = delete;

    virtual IStocksTableWidget* newInstance(
        IStocksTableModelFactory*          stocksTableModelFactory,
        IStocksTableRecordFactory*         stockTableRecordFactory,
        IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
        IActionsTableItemWidgetFactory*    actionsTableItemWidgetFactory,
        IOrderWavesDialogFactory*          orderWavesDialogFactory,
        IOrderWavesWidgetFactory*          orderWavesWidgetFactory,
        ILogosStorage*                     logosStorage,
        IUserStorage*                      userStorage,
        IOrderBookThread*                  orderBookThread,
        IHttpClient*                       httpClient,
        IFileDialogFactory*                fileDialogFactory,
        ISettingsEditor*                   settingsEditor,
        QWidget*                           parent
    ) const = 0;
};
