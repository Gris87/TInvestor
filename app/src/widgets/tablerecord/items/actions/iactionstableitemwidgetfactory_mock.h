#pragma once



#include "src/widgets/tablerecord/items/actions/iactionstableitemwidgetfactory.h"

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
        (IMarketWavesDialogFactory * marketWavesDialogFactory,
         IMarketWavesThread* marketWavesThread,
         IHttpClient*        httpClient,
         Stock*              stock,
         QWidget*            parent),
        (override)
    );
};
