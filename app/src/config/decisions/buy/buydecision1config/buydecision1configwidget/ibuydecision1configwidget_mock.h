#pragma once



#include "src/config/decisions/buy/buydecision1config/buydecision1configwidget/ibuydecision1configwidget.h"

#include <gmock/gmock.h>



class BuyDecision1ConfigWidgetMock : public IBuyDecision1ConfigWidget
{
public:
    explicit BuyDecision1ConfigWidgetMock(QWidget* parent = nullptr) :
        IBuyDecision1ConfigWidget(parent)
    {
    }
    ~BuyDecision1ConfigWidgetMock() override = default;

    BuyDecision1ConfigWidgetMock(const BuyDecision1ConfigWidgetMock& another)            = delete;
    BuyDecision1ConfigWidgetMock& operator=(const BuyDecision1ConfigWidgetMock& another) = delete;

    MOCK_METHOD(void, updateUiFromConfig, (), (override));
};
