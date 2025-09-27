#pragma once



#include "src/config/decisions/buy/buydecision5config/buydecision5configwidget/ibuydecision5configwidgetfactory.h"

#include <gmock/gmock.h>



class BuyDecision5ConfigWidgetFactoryMock : public IBuyDecision5ConfigWidgetFactory
{
public:
    BuyDecision5ConfigWidgetFactoryMock() :
        IBuyDecision5ConfigWidgetFactory()
    {
    }
    ~BuyDecision5ConfigWidgetFactoryMock() override = default;

    BuyDecision5ConfigWidgetFactoryMock(const BuyDecision5ConfigWidgetFactoryMock& another)            = delete;
    BuyDecision5ConfigWidgetFactoryMock& operator=(const BuyDecision5ConfigWidgetFactoryMock& another) = delete;

    MOCK_METHOD(IBuyDecision5ConfigWidget*, newInstance, (IBuyDecision5Config * config, QWidget* parent), (const, override));
};
