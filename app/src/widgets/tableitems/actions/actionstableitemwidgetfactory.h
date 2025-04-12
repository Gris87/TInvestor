#pragma once



#include "src/widgets/tableitems/actions/iactionstableitemwidgetfactory.h"



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
        Stock*                    stock,
        int                       precision,
        QWidget*                  parent
    ) const override;
};
