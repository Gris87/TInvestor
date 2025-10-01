#pragma once



#include "src/config/decisions/buy/buydecision7config/buydecision7configwidget/ibuydecision7configwidgetfactory.h"

#include <gmock/gmock.h>



class BuyDecision7ConfigWidgetFactoryMock : public IBuyDecision7ConfigWidgetFactory
{
public:
    BuyDecision7ConfigWidgetFactoryMock() :
        IBuyDecision7ConfigWidgetFactory()
    {
    }
    ~BuyDecision7ConfigWidgetFactoryMock() override = default;

    BuyDecision7ConfigWidgetFactoryMock(const BuyDecision7ConfigWidgetFactoryMock& another)            = delete;
    BuyDecision7ConfigWidgetFactoryMock& operator=(const BuyDecision7ConfigWidgetFactoryMock& another) = delete;

    MOCK_METHOD(IBuyDecision7ConfigWidget*, newInstance, (IBuyDecision7Config * config, QWidget* parent), (const, override));
};
