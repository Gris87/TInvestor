#pragma once



#include "src/config/decisions/sell/iselldecision2configwidget.h"

#include <gmock/gmock.h>



class SellDecision2ConfigWidgetMock : public ISellDecision2ConfigWidget
{
public:
    SellDecision2ConfigWidgetMock(QWidget *parent = nullptr) : ISellDecision2ConfigWidget(parent) {}
    ~SellDecision2ConfigWidgetMock() override = default;

    SellDecision2ConfigWidgetMock(const SellDecision2ConfigWidgetMock &another) = delete;
    SellDecision2ConfigWidgetMock& operator=(const SellDecision2ConfigWidgetMock &another) = delete;

    MOCK_METHOD1(setSellDecision2Config, void(ISellDecision2Config *config));
    MOCK_METHOD0(updateUiFromConfig, void());
};
