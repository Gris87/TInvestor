#pragma once



#include "src/config/decisions/sell/selldecision2config/selldecision2configwidget/iselldecision2configwidgetfactory.h"

#include <gmock/gmock.h>



class SellDecision2ConfigWidgetFactoryMock : public ISellDecision2ConfigWidgetFactory
{
public:
    SellDecision2ConfigWidgetFactoryMock() : ISellDecision2ConfigWidgetFactory() {}
    ~SellDecision2ConfigWidgetFactoryMock() override = default;

    SellDecision2ConfigWidgetFactoryMock(const SellDecision2ConfigWidgetFactoryMock &another) = delete;
    SellDecision2ConfigWidgetFactoryMock& operator=(const SellDecision2ConfigWidgetFactoryMock &another) = delete;

    MOCK_METHOD2(newInstance, ISellDecision2ConfigWidget*(ISellDecision2Config *config, QWidget *parent));
};
