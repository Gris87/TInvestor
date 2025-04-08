#pragma once



#include "src/config/decisions/buy/buydecision2config/buydecision2configwidget/ibuydecision2configwidgetfactory.h"

#include <gmock/gmock.h>



class BuyDecision2ConfigWidgetFactoryMock : public IBuyDecision2ConfigWidgetFactory
{
public:
    BuyDecision2ConfigWidgetFactoryMock() :
        IBuyDecision2ConfigWidgetFactory()
    {
    }
    ~BuyDecision2ConfigWidgetFactoryMock() override = default;

    BuyDecision2ConfigWidgetFactoryMock(const BuyDecision2ConfigWidgetFactoryMock& another)            = delete;
    BuyDecision2ConfigWidgetFactoryMock& operator=(const BuyDecision2ConfigWidgetFactoryMock& another) = delete;

    MOCK_METHOD(IBuyDecision2ConfigWidget*, newInstance, (IBuyDecision2Config * config, QWidget* parent), (const, override));
};
