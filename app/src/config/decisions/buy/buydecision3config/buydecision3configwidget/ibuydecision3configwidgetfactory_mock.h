#pragma once



#include "src/config/decisions/buy/buydecision3config/buydecision3configwidget/ibuydecision3configwidgetfactory.h"

#include <gmock/gmock.h>



class BuyDecision3ConfigWidgetFactoryMock : public IBuyDecision3ConfigWidgetFactory
{
public:
    BuyDecision3ConfigWidgetFactoryMock() :
        IBuyDecision3ConfigWidgetFactory()
    {
    }
    ~BuyDecision3ConfigWidgetFactoryMock() override = default;

    BuyDecision3ConfigWidgetFactoryMock(const BuyDecision3ConfigWidgetFactoryMock& another)            = delete;
    BuyDecision3ConfigWidgetFactoryMock& operator=(const BuyDecision3ConfigWidgetFactoryMock& another) = delete;

    MOCK_METHOD2(newInstance, IBuyDecision3ConfigWidget*(IBuyDecision3Config* config, QWidget* parent));
};
