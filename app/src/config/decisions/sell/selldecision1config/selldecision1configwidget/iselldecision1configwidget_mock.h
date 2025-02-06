#pragma once



#include "src/config/decisions/sell/selldecision1config/selldecision1configwidget/iselldecision1configwidget.h"

#include <gmock/gmock.h>



class SellDecision1ConfigWidgetMock : public ISellDecision1ConfigWidget
{
public:
    explicit SellDecision1ConfigWidgetMock(QWidget *parent = nullptr) : ISellDecision1ConfigWidget(parent) {}
    ~SellDecision1ConfigWidgetMock() override = default;

    SellDecision1ConfigWidgetMock(const SellDecision1ConfigWidgetMock &another) = delete;
    SellDecision1ConfigWidgetMock& operator=(const SellDecision1ConfigWidgetMock &another) = delete;

    MOCK_METHOD0(updateUiFromConfig, void());
};
