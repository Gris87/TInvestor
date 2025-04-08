#pragma once



#include "src/config/decisions/sell/selldecision1config/selldecision1configwidget/iselldecision1configwidgetfactory.h"

#include <gmock/gmock.h>



class SellDecision1ConfigWidgetFactoryMock : public ISellDecision1ConfigWidgetFactory
{
public:
    SellDecision1ConfigWidgetFactoryMock() :
        ISellDecision1ConfigWidgetFactory()
    {
    }
    ~SellDecision1ConfigWidgetFactoryMock() override = default;

    SellDecision1ConfigWidgetFactoryMock(const SellDecision1ConfigWidgetFactoryMock& another)            = delete;
    SellDecision1ConfigWidgetFactoryMock& operator=(const SellDecision1ConfigWidgetFactoryMock& another) = delete;

    MOCK_METHOD(ISellDecision1ConfigWidget*, newInstance, (ISellDecision1Config * config, QWidget* parent), (const, override));
};
