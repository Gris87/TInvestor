#pragma once



#include "src/config/decisions/sell/selldecision4config/selldecision4configwidget/iselldecision4configwidgetfactory.h"

#include <gmock/gmock.h>



class SellDecision4ConfigWidgetFactoryMock : public ISellDecision4ConfigWidgetFactory
{
public:
    SellDecision4ConfigWidgetFactoryMock() :
        ISellDecision4ConfigWidgetFactory()
    {
    }
    ~SellDecision4ConfigWidgetFactoryMock() override = default;

    SellDecision4ConfigWidgetFactoryMock(const SellDecision4ConfigWidgetFactoryMock& another)            = delete;
    SellDecision4ConfigWidgetFactoryMock& operator=(const SellDecision4ConfigWidgetFactoryMock& another) = delete;

    MOCK_METHOD(ISellDecision4ConfigWidget*, newInstance, (ISellDecision4Config * config, QWidget* parent), (const, override));
};
