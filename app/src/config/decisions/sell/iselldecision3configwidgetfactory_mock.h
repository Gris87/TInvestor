#pragma once



#include "src/config/decisions/sell/iselldecision3configwidgetfactory.h"

#include <gmock/gmock.h>



class SellDecision3ConfigWidgetFactoryMock : public ISellDecision3ConfigWidgetFactory
{
public:
    SellDecision3ConfigWidgetFactoryMock() : ISellDecision3ConfigWidgetFactory() {}
    ~SellDecision3ConfigWidgetFactoryMock() override = default;

    SellDecision3ConfigWidgetFactoryMock(const SellDecision3ConfigWidgetFactoryMock &another) = delete;
    SellDecision3ConfigWidgetFactoryMock& operator=(const SellDecision3ConfigWidgetFactoryMock &another) = delete;

    MOCK_METHOD2(newInstance, ISellDecision3ConfigWidget*(ISellDecision3Config *config, QWidget *parent));
};
