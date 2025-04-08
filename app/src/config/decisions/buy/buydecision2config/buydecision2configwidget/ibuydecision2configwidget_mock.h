#pragma once



#include "src/config/decisions/buy/buydecision2config/buydecision2configwidget/ibuydecision2configwidget.h"

#include <gmock/gmock.h>



class BuyDecision2ConfigWidgetMock : public IBuyDecision2ConfigWidget
{
    Q_OBJECT

public:
    explicit BuyDecision2ConfigWidgetMock(QWidget* parent = nullptr) :
        IBuyDecision2ConfigWidget(parent)
    {
    }
    ~BuyDecision2ConfigWidgetMock() override = default;

    BuyDecision2ConfigWidgetMock(const BuyDecision2ConfigWidgetMock& another)            = delete;
    BuyDecision2ConfigWidgetMock& operator=(const BuyDecision2ConfigWidgetMock& another) = delete;

    MOCK_METHOD(void, updateUiFromConfig, (), (const, override));
};
