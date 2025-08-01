#pragma once



#include "src/config/decisions/buy/buydecision4config/buydecision4configwidget/ibuydecision4configwidgetfactory.h"

#include <gmock/gmock.h>



class BuyDecision4ConfigWidgetFactoryMock : public IBuyDecision4ConfigWidgetFactory
{
public:
    BuyDecision4ConfigWidgetFactoryMock() :
        IBuyDecision4ConfigWidgetFactory()
    {
    }
    ~BuyDecision4ConfigWidgetFactoryMock() override = default;

    BuyDecision4ConfigWidgetFactoryMock(const BuyDecision4ConfigWidgetFactoryMock& another)            = delete;
    BuyDecision4ConfigWidgetFactoryMock& operator=(const BuyDecision4ConfigWidgetFactoryMock& another) = delete;

    MOCK_METHOD(IBuyDecision4ConfigWidget*, newInstance, (IBuyDecision4Config * config, QWidget* parent), (const, override));
};
