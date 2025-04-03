#pragma once



#include "src/widgets/stockstablewidget/istockstablewidgetfactory.h"

#include <gmock/gmock.h>



class StocksTableWidgetFactoryMock : public IStocksTableWidgetFactory
{
public:
    StocksTableWidgetFactoryMock() :
        IStocksTableWidgetFactory()
    {
    }
    ~StocksTableWidgetFactoryMock() override = default;

    StocksTableWidgetFactoryMock(const StocksTableWidgetFactoryMock& another)            = delete;
    StocksTableWidgetFactoryMock& operator=(const StocksTableWidgetFactoryMock& another) = delete;

    MOCK_METHOD(
        IStocksTableWidget*,
        newInstance,
        (IStocksTableRecordFactory * stockTableRecordFactory,
         IStockTableItemWidgetFactory*   stockTableItemWidgetFactory,
         IActionsTableItemWidgetFactory* actionsTableItemWidgetFactory,
         IOrderWavesDialogFactory*       orderWavesDialogFactory,
         IOrderWavesWidgetFactory*       orderWavesWidgetFactory,
         IUserStorage*                   userStorage,
         IOrderBookThread*               orderBookThread,
         IHttpClient*                    httpClient,
         ISettingsEditor*                settingsEditor,
         QWidget*                        parent),
        (override)
    );
};
