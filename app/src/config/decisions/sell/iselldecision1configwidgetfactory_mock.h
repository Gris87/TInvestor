#pragma once



#include "src/config/decisions/sell/iselldecision1configwidgetfactory.h"

#include <gmock/gmock.h>



class SellDecision1ConfigWidgetFactoryMock : public ISellDecision1ConfigWidgetFactory
{
public:
    SellDecision1ConfigWidgetFactoryMock() {}
    ~SellDecision1ConfigWidgetFactoryMock() override = default;

    SellDecision1ConfigWidgetFactoryMock(const SellDecision1ConfigWidgetFactoryMock &another) = delete;
    SellDecision1ConfigWidgetFactoryMock& operator=(const SellDecision1ConfigWidgetFactoryMock &another) = delete;

    MOCK_METHOD1(newInstance, ISellDecision1ConfigWidget*(QWidget *parent));
};
