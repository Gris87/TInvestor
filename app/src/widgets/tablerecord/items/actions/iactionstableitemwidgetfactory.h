#pragma once



#include "src/widgets/tablerecord/items/actions/iactionstableitemwidget.h"

#include "src/dialogs/marketwavesdialog/imarketwavesdialogfactory.h"
#include "src/domain/stocks/stock.h"
#include "src/threads/marketwaves/imarketwavesthread.h"
#include "src/utils/http/ihttpclient.h"



class IActionsTableItemWidgetFactory
{
public:
    IActionsTableItemWidgetFactory()
    {
    }
    virtual ~IActionsTableItemWidgetFactory() = default;

    IActionsTableItemWidgetFactory(const IActionsTableItemWidgetFactory& another)            = delete;
    IActionsTableItemWidgetFactory& operator=(const IActionsTableItemWidgetFactory& another) = delete;

    virtual IActionsTableItemWidget* newInstance(
        IMarketWavesDialogFactory* marketWavesDialogFactory,
        IMarketWavesThread*        marketWavesThread,
        IHttpClient*               httpClient,
        Stock*                     stock,
        QWidget*                   parent
    ) = 0;
};
