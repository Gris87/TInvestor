#pragma once



#include "src/config/decisions/sell/selldecision3config/selldecision3configwidget/iselldecision3configwidgetfactory.h"

#include <gmock/gmock.h>



class SellDecision3ConfigWidgetFactoryMock : public ISellDecision3ConfigWidgetFactory
{
public:
    SellDecision3ConfigWidgetFactoryMock() :
        ISellDecision3ConfigWidgetFactory()
    {
    }
    ~SellDecision3ConfigWidgetFactoryMock() override = default;

    SellDecision3ConfigWidgetFactoryMock(const SellDecision3ConfigWidgetFactoryMock& another)            = delete;
    SellDecision3ConfigWidgetFactoryMock& operator=(const SellDecision3ConfigWidgetFactoryMock& another) = delete;

    MOCK_METHOD(ISellDecision3ConfigWidget*, newInstance, (ISellDecision3Config * config, QWidget* parent), (override));
};
