#pragma once



#include "src/config/decisions/sell/iselldecision2configwidgetfactory.h"

#include <gmock/gmock.h>



class SellDecision2ConfigWidgetFactoryMock : public ISellDecision2ConfigWidgetFactory
{
public:
    SellDecision2ConfigWidgetFactoryMock() {}
    ~SellDecision2ConfigWidgetFactoryMock() override = default;

    SellDecision2ConfigWidgetFactoryMock(const SellDecision2ConfigWidgetFactoryMock &another) = delete;
    SellDecision2ConfigWidgetFactoryMock& operator=(const SellDecision2ConfigWidgetFactoryMock &another) = delete;

    MOCK_METHOD1(newInstance, ISellDecision2ConfigWidget*(QWidget *parent));
};
