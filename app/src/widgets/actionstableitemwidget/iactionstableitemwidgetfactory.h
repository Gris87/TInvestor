#pragma once



#include "src/widgets/actionstableitemwidget/iactionstableitemwidget.h"

#include "src/dialogs/orderwavesdialog/iorderwavesdialogfactory.h"
#include "src/threads/orderbook/iorderbookthread.h"
#include "src/utils/http/ihttpclient.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory.h"
#include "src/widgets/tablemodels/stockstablemodel/istockstablemodel.h"



class IActionsTableItemWidgetFactory
{
public:
    IActionsTableItemWidgetFactory()          = default;
    virtual ~IActionsTableItemWidgetFactory() = default;

    IActionsTableItemWidgetFactory(const IActionsTableItemWidgetFactory& another)            = delete;
    IActionsTableItemWidgetFactory& operator=(const IActionsTableItemWidgetFactory& another) = delete;

    virtual IActionsTableItemWidget* newInstance(
        IOrderWavesDialogFactory* orderWavesDialogFactory,
        IOrderWavesWidgetFactory* orderWavesWidgetFactory,
        IOrderBookThread*         orderBookThread,
        IHttpClient*              httpClient,
        IStocksTableModel*        stocksTableModel,
        int                       tableRow,
        QWidget*                  parent
    ) const = 0;
};
