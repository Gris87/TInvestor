#pragma once



#include "src/config/decisions/buy/buydecision6config/buydecision6configwidget/ibuydecision6configwidgetfactory.h"

#include <gmock/gmock.h>



class BuyDecision6ConfigWidgetFactoryMock : public IBuyDecision6ConfigWidgetFactory
{
public:
    BuyDecision6ConfigWidgetFactoryMock() :
        IBuyDecision6ConfigWidgetFactory()
    {
    }
    ~BuyDecision6ConfigWidgetFactoryMock() override = default;

    BuyDecision6ConfigWidgetFactoryMock(const BuyDecision6ConfigWidgetFactoryMock& another)            = delete;
    BuyDecision6ConfigWidgetFactoryMock& operator=(const BuyDecision6ConfigWidgetFactoryMock& another) = delete;

    MOCK_METHOD(IBuyDecision6ConfigWidget*, newInstance, (IBuyDecision6Config * config, QWidget* parent), (const, override));
};
