#pragma once



#include "src/widgets/stockstablewidget/istockstablewidgetfactory.h"



class StocksTableWidgetFactory : public IStocksTableWidgetFactory
{
public:
    StocksTableWidgetFactory();
    ~StocksTableWidgetFactory() override;

    StocksTableWidgetFactory(const StocksTableWidgetFactory& another)            = delete;
    StocksTableWidgetFactory& operator=(const StocksTableWidgetFactory& another) = delete;

    IStocksTableWidget* newInstance(
        IStocksTableRecordFactory*      stockTableRecordFactory,
        IStockTableItemWidgetFactory*   stockTableItemWidgetFactory,
        IActionsTableItemWidgetFactory* actionsTableItemWidgetFactory,
        IOrderWavesDialogFactory*       orderWavesDialogFactory,
        IOrderWavesWidgetFactory*       orderWavesWidgetFactory,
        IUserStorage*                   userStorage,
        IOrderBookThread*               orderBookThread,
        IHttpClient*                    httpClient,
        ISettingsEditor*                settingsEditor,
        QWidget*                        parent
    ) const override;
};
