#pragma once



#include "src/config/decisions/buy/buydecision8config/buydecision8configwidget/ibuydecision8configwidget.h"

#include <gmock/gmock.h>



class BuyDecision8ConfigWidgetMock : public IBuyDecision8ConfigWidget
{
    Q_OBJECT

public:
    explicit BuyDecision8ConfigWidgetMock(QWidget* parent = nullptr) :
        IBuyDecision8ConfigWidget(parent)
    {
    }
    ~BuyDecision8ConfigWidgetMock() override = default;

    BuyDecision8ConfigWidgetMock(const BuyDecision8ConfigWidgetMock& another)            = delete;
    BuyDecision8ConfigWidgetMock& operator=(const BuyDecision8ConfigWidgetMock& another) = delete;

    MOCK_METHOD(void, updateUiFromConfig, (), (const, override));
    MOCK_METHOD(void, makeReadOnly, (), (const, override));
};
