#pragma once



#include "src/widgets/actionstableitemwidget/iactionstableitemwidgetfactory.h"

#include <gmock/gmock.h>



class ActionsTableItemWidgetFactoryMock : public IActionsTableItemWidgetFactory
{
public:
    ActionsTableItemWidgetFactoryMock() :
        IActionsTableItemWidgetFactory()
    {
    }
    ~ActionsTableItemWidgetFactoryMock() override = default;

    ActionsTableItemWidgetFactoryMock(const ActionsTableItemWidgetFactoryMock& another)            = delete;
    ActionsTableItemWidgetFactoryMock& operator=(const ActionsTableItemWidgetFactoryMock& another) = delete;

    MOCK_METHOD(
        IActionsTableItemWidget*,
        newInstance,
        (IOrderWavesDialogFactory * orderWavesDialogFactory,
         IOrderWavesWidgetFactory* orderWavesWidgetFactory,
         IOrderBookThread*         orderBookThread,
         IHttpClient*              httpClient,
         IStocksTableModel*        stocksTableModel,
         int                       tableRow,
         QWidget*                  parent),
        (const, override)
    );
};
