#pragma once



#include "src/config/decisions/buy/buydecision6config/buydecision6configwidget/ibuydecision6configwidget.h"

#include <gmock/gmock.h>



class BuyDecision6ConfigWidgetMock : public IBuyDecision6ConfigWidget
{
    Q_OBJECT

public:
    explicit BuyDecision6ConfigWidgetMock(QWidget* parent = nullptr) :
        IBuyDecision6ConfigWidget(parent)
    {
    }
    ~BuyDecision6ConfigWidgetMock() override = default;

    BuyDecision6ConfigWidgetMock(const BuyDecision6ConfigWidgetMock& another)            = delete;
    BuyDecision6ConfigWidgetMock& operator=(const BuyDecision6ConfigWidgetMock& another) = delete;

    MOCK_METHOD(void, updateUiFromConfig, (), (const, override));
    MOCK_METHOD(void, makeReadOnly, (), (const, override));
};
