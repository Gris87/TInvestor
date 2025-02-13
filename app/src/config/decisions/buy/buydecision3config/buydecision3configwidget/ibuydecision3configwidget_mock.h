#pragma once



#include "src/config/decisions/buy/buydecision3config/buydecision3configwidget/ibuydecision3configwidget.h"

#include <gmock/gmock.h>



class BuyDecision3ConfigWidgetMock : public IBuyDecision3ConfigWidget
{
public:
    explicit BuyDecision3ConfigWidgetMock(QWidget* parent = nullptr) :
        IBuyDecision3ConfigWidget(parent)
    {
    }
    ~BuyDecision3ConfigWidgetMock() override = default;

    BuyDecision3ConfigWidgetMock(const BuyDecision3ConfigWidgetMock& another)            = delete;
    BuyDecision3ConfigWidgetMock& operator=(const BuyDecision3ConfigWidgetMock& another) = delete;

    MOCK_METHOD(void, updateUiFromConfig, (), (override));
};
