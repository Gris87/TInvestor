#pragma once



#include "src/config/decisions/buy/buydecision8config/buydecision8configwidget/ibuydecision8configwidgetfactory.h"

#include <gmock/gmock.h>



class BuyDecision8ConfigWidgetFactoryMock : public IBuyDecision8ConfigWidgetFactory
{
public:
    BuyDecision8ConfigWidgetFactoryMock() :
        IBuyDecision8ConfigWidgetFactory()
    {
    }
    ~BuyDecision8ConfigWidgetFactoryMock() override = default;

    BuyDecision8ConfigWidgetFactoryMock(const BuyDecision8ConfigWidgetFactoryMock& another)            = delete;
    BuyDecision8ConfigWidgetFactoryMock& operator=(const BuyDecision8ConfigWidgetFactoryMock& another) = delete;

    MOCK_METHOD(IBuyDecision8ConfigWidget*, newInstance, (IBuyDecision8Config * config, QWidget* parent), (const, override));
};
