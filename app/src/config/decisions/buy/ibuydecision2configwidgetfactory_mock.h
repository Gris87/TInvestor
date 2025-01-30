#pragma once



#include "src/config/decisions/buy/ibuydecision2configwidgetfactory.h"

#include <gmock/gmock.h>



class BuyDecision2ConfigWidgetFactoryMock : public IBuyDecision2ConfigWidgetFactory
{
public:
    BuyDecision2ConfigWidgetFactoryMock() {}
    ~BuyDecision2ConfigWidgetFactoryMock() override = default;

    BuyDecision2ConfigWidgetFactoryMock(const BuyDecision2ConfigWidgetFactoryMock &another) = delete;
    BuyDecision2ConfigWidgetFactoryMock& operator=(const BuyDecision2ConfigWidgetFactoryMock &another) = delete;

    MOCK_METHOD1(newInstance, IBuyDecision2ConfigWidget*(QWidget *parent));
};
