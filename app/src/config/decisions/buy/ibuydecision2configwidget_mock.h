#pragma once



#include "src/config/decisions/buy/ibuydecision2configwidget.h"

#include <gmock/gmock.h>



class BuyDecision2ConfigWidgetMock : public IBuyDecision2ConfigWidget
{
public:
    BuyDecision2ConfigWidgetMock(QWidget *parent = nullptr) : IBuyDecision2ConfigWidget(parent) {}
    ~BuyDecision2ConfigWidgetMock() override = default;

    BuyDecision2ConfigWidgetMock(const BuyDecision2ConfigWidgetMock &another) = delete;
    BuyDecision2ConfigWidgetMock& operator=(const BuyDecision2ConfigWidgetMock &another) = delete;

    MOCK_METHOD1(setBuyDecision2Config, void(IBuyDecision2Config *config));
    MOCK_METHOD0(updateUiFromConfig, void());
};
