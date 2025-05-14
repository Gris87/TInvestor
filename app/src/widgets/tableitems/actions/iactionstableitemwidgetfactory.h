#pragma once



#include "src/widgets/tableitems/actions/iactionstableitemwidget.h"

#include "src/dialogs/orderwavesdialog/iorderwavesdialogfactory.h"
#include "src/domain/stock/stock.h"
#include "src/threads/orderbook/iorderbookthread.h"
#include "src/utils/http/ihttpclient.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory.h"



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
        Stock*                    stock,
        qint8                     precision,
        QWidget*                  parent
    ) const = 0;
};
