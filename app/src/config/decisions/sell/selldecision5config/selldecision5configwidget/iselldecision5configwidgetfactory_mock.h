#pragma once



#include "src/config/decisions/sell/selldecision5config/selldecision5configwidget/iselldecision5configwidgetfactory.h"

#include <gmock/gmock.h>



class SellDecision5ConfigWidgetFactoryMock : public ISellDecision5ConfigWidgetFactory
{
public:
    SellDecision5ConfigWidgetFactoryMock() :
        ISellDecision5ConfigWidgetFactory()
    {
    }
    ~SellDecision5ConfigWidgetFactoryMock() override = default;

    SellDecision5ConfigWidgetFactoryMock(const SellDecision5ConfigWidgetFactoryMock& another)            = delete;
    SellDecision5ConfigWidgetFactoryMock& operator=(const SellDecision5ConfigWidgetFactoryMock& another) = delete;

    MOCK_METHOD(ISellDecision5ConfigWidget*, newInstance, (ISellDecision5Config * config, QWidget* parent), (const, override));
};
