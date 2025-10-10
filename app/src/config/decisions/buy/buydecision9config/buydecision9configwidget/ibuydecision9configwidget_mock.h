#pragma once



#include "src/config/decisions/buy/buydecision9config/buydecision9configwidget/ibuydecision9configwidget.h"

#include <gmock/gmock.h>



class BuyDecision9ConfigWidgetMock : public IBuyDecision9ConfigWidget
{
    Q_OBJECT

public:
    explicit BuyDecision9ConfigWidgetMock(QWidget* parent = nullptr) :
        IBuyDecision9ConfigWidget(parent)
    {
    }
    ~BuyDecision9ConfigWidgetMock() override = default;

    BuyDecision9ConfigWidgetMock(const BuyDecision9ConfigWidgetMock& another)            = delete;
    BuyDecision9ConfigWidgetMock& operator=(const BuyDecision9ConfigWidgetMock& another) = delete;

    MOCK_METHOD(void, updateUiFromConfig, (), (const, override));
    MOCK_METHOD(void, makeReadOnly, (), (const, override));
};
