#pragma once



#include "src/widgets/actionstableitemwidget/iactionstableitemwidgetfactory.h"



class ActionsTableItemWidgetFactory : public IActionsTableItemWidgetFactory
{
public:
    ActionsTableItemWidgetFactory();
    ~ActionsTableItemWidgetFactory() override;

    ActionsTableItemWidgetFactory(const ActionsTableItemWidgetFactory& another)            = delete;
    ActionsTableItemWidgetFactory& operator=(const ActionsTableItemWidgetFactory& another) = delete;

    IActionsTableItemWidget* newInstance(
        IOrderWavesDialogFactory* orderWavesDialogFactory,
        IOrderWavesWidgetFactory* orderWavesWidgetFactory,
        IOrderBookThread*         orderBookThread,
        IHttpClient*              httpClient,
        IStocksTableModel*        stocksTableModel,
        int                       tableRow,
        QWidget*                  parent
    ) const override;
};
