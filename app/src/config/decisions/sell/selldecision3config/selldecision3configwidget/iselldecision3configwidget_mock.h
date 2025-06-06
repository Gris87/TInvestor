#pragma once



#include "src/config/decisions/sell/selldecision3config/selldecision3configwidget/iselldecision3configwidget.h"

#include <gmock/gmock.h>



class SellDecision3ConfigWidgetMock : public ISellDecision3ConfigWidget
{
    Q_OBJECT

public:
    explicit SellDecision3ConfigWidgetMock(QWidget* parent = nullptr) :
        ISellDecision3ConfigWidget(parent)
    {
    }
    ~SellDecision3ConfigWidgetMock() override = default;

    SellDecision3ConfigWidgetMock(const SellDecision3ConfigWidgetMock& another)            = delete;
    SellDecision3ConfigWidgetMock& operator=(const SellDecision3ConfigWidgetMock& another) = delete;

    MOCK_METHOD(void, updateUiFromConfig, (), (const, override));
};
