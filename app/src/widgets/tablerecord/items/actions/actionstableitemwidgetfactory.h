#pragma once



#include "src/widgets/tablerecord/items/actions/iactionstableitemwidgetfactory.h"



class ActionsTableItemWidgetFactory : public IActionsTableItemWidgetFactory
{
public:
    ActionsTableItemWidgetFactory();
    ~ActionsTableItemWidgetFactory();

    ActionsTableItemWidgetFactory(const ActionsTableItemWidgetFactory& another)            = delete;
    ActionsTableItemWidgetFactory& operator=(const ActionsTableItemWidgetFactory& another) = delete;

    IActionsTableItemWidget* newInstance(
        IMarketWavesDialogFactory* marketWavesDialogFactory, IHttpClient* httpClient, Stock* stock, QWidget* parent
    ) override;
};
