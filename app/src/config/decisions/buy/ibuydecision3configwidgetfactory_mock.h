#pragma once



#include "src/config/decisions/buy/ibuydecision3configwidgetfactory.h"

#include <gmock/gmock.h>



class BuyDecision3ConfigWidgetFactoryMock : public IBuyDecision3ConfigWidgetFactory
{
public:
    BuyDecision3ConfigWidgetFactoryMock() {}
    ~BuyDecision3ConfigWidgetFactoryMock() override = default;

    BuyDecision3ConfigWidgetFactoryMock(const BuyDecision3ConfigWidgetFactoryMock &another) = delete;
    BuyDecision3ConfigWidgetFactoryMock& operator=(const BuyDecision3ConfigWidgetFactoryMock &another) = delete;

    MOCK_METHOD1(newInstance, IBuyDecision3ConfigWidget*(QWidget *parent));
};
