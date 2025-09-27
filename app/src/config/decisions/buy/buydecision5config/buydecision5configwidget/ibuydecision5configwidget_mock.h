#pragma once



#include "src/config/decisions/buy/buydecision5config/buydecision5configwidget/ibuydecision5configwidget.h"

#include <gmock/gmock.h>



class BuyDecision5ConfigWidgetMock : public IBuyDecision5ConfigWidget
{
    Q_OBJECT

public:
    explicit BuyDecision5ConfigWidgetMock(QWidget* parent = nullptr) :
        IBuyDecision5ConfigWidget(parent)
    {
    }
    ~BuyDecision5ConfigWidgetMock() override = default;

    BuyDecision5ConfigWidgetMock(const BuyDecision5ConfigWidgetMock& another)            = delete;
    BuyDecision5ConfigWidgetMock& operator=(const BuyDecision5ConfigWidgetMock& another) = delete;

    MOCK_METHOD(void, updateUiFromConfig, (), (const, override));
    MOCK_METHOD(void, makeReadOnly, (), (const, override));
};
