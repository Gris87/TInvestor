#pragma once



#include "src/widgets/portfoliotablewidget/iportfoliotablewidgetfactory.h"

#include <gmock/gmock.h>



class PortfolioTableWidgetFactoryMock : public IPortfolioTableWidgetFactory
{
public:
    PortfolioTableWidgetFactoryMock() :
        IPortfolioTableWidgetFactory()
    {
    }
    ~PortfolioTableWidgetFactoryMock() override = default;

    PortfolioTableWidgetFactoryMock(const PortfolioTableWidgetFactoryMock& another)            = delete;
    PortfolioTableWidgetFactoryMock& operator=(const PortfolioTableWidgetFactoryMock& another) = delete;

    MOCK_METHOD(IPortfolioTableWidget*, newInstance, (ISettingsEditor * settingsEditor, QWidget* parent), (const, override));
};
