#pragma once



#include "src/config/decisions/buy/buydecision7config/buydecision7configwidget/ibuydecision7configwidget.h"

#include <gmock/gmock.h>



class BuyDecision7ConfigWidgetMock : public IBuyDecision7ConfigWidget
{
    Q_OBJECT

public:
    explicit BuyDecision7ConfigWidgetMock(QWidget* parent = nullptr) :
        IBuyDecision7ConfigWidget(parent)
    {
    }
    ~BuyDecision7ConfigWidgetMock() override = default;

    BuyDecision7ConfigWidgetMock(const BuyDecision7ConfigWidgetMock& another)            = delete;
    BuyDecision7ConfigWidgetMock& operator=(const BuyDecision7ConfigWidgetMock& another) = delete;

    MOCK_METHOD(void, updateUiFromConfig, (), (const, override));
    MOCK_METHOD(void, makeReadOnly, (), (const, override));
};
