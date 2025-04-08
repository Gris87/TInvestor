#pragma once



#include "src/config/decisions/sell/selldecision2config/selldecision2configwidget/iselldecision2configwidget.h"

#include <gmock/gmock.h>



class SellDecision2ConfigWidgetMock : public ISellDecision2ConfigWidget
{
    Q_OBJECT

public:
    explicit SellDecision2ConfigWidgetMock(QWidget* parent = nullptr) :
        ISellDecision2ConfigWidget(parent)
    {
    }
    ~SellDecision2ConfigWidgetMock() override = default;

    SellDecision2ConfigWidgetMock(const SellDecision2ConfigWidgetMock& another)            = delete;
    SellDecision2ConfigWidgetMock& operator=(const SellDecision2ConfigWidgetMock& another) = delete;

    MOCK_METHOD(void, updateUiFromConfig, (), (const, override));
};
