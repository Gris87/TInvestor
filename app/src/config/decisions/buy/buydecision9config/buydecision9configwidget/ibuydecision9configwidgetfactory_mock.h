#pragma once



#include "src/config/decisions/buy/buydecision9config/buydecision9configwidget/ibuydecision9configwidgetfactory.h"

#include <gmock/gmock.h>



class BuyDecision9ConfigWidgetFactoryMock : public IBuyDecision9ConfigWidgetFactory
{
public:
    BuyDecision9ConfigWidgetFactoryMock() :
        IBuyDecision9ConfigWidgetFactory()
    {
    }
    ~BuyDecision9ConfigWidgetFactoryMock() override = default;

    BuyDecision9ConfigWidgetFactoryMock(const BuyDecision9ConfigWidgetFactoryMock& another)            = delete;
    BuyDecision9ConfigWidgetFactoryMock& operator=(const BuyDecision9ConfigWidgetFactoryMock& another) = delete;

    MOCK_METHOD(IBuyDecision9ConfigWidget*, newInstance, (IBuyDecision9Config * config, QWidget* parent), (const, override));
};
