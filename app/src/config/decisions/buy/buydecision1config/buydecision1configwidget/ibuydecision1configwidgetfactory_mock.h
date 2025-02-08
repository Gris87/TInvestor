#pragma once



#include "src/config/decisions/buy/buydecision1config/buydecision1configwidget/ibuydecision1configwidgetfactory.h"

#include <gmock/gmock.h>



class BuyDecision1ConfigWidgetFactoryMock : public IBuyDecision1ConfigWidgetFactory
{
public:
    BuyDecision1ConfigWidgetFactoryMock() :
        IBuyDecision1ConfigWidgetFactory()
    {
    }
    ~BuyDecision1ConfigWidgetFactoryMock() override = default;

    BuyDecision1ConfigWidgetFactoryMock(const BuyDecision1ConfigWidgetFactoryMock& another)            = delete;
    BuyDecision1ConfigWidgetFactoryMock& operator=(const BuyDecision1ConfigWidgetFactoryMock& another) = delete;

    MOCK_METHOD2(newInstance, IBuyDecision1ConfigWidget*(IBuyDecision1Config* config, QWidget* parent));
};
