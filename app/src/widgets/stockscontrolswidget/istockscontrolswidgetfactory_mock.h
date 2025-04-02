#pragma once



#include "src/widgets/stockscontrolswidget/istockscontrolswidgetfactory.h"

#include <gmock/gmock.h>



class StocksControlsWidgetFactoryMock : public IStocksControlsWidgetFactory
{
public:
    StocksControlsWidgetFactoryMock() :
        IStocksControlsWidgetFactory()
    {
    }
    ~StocksControlsWidgetFactoryMock() override = default;

    StocksControlsWidgetFactoryMock(const StocksControlsWidgetFactoryMock& another)            = delete;
    StocksControlsWidgetFactoryMock& operator=(const StocksControlsWidgetFactoryMock& another) = delete;

    MOCK_METHOD(
        IStocksControlsWidget*,
        newInstance,
        (IStocksStorage * stocksStorage, ISettingsEditor* settingsEditor, QWidget* parent),
        (override)
    );
};
