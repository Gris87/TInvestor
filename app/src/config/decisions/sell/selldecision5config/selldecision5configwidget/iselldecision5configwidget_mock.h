#pragma once



#include "src/config/decisions/sell/selldecision5config/selldecision5configwidget/iselldecision5configwidget.h"

#include <gmock/gmock.h>



class SellDecision5ConfigWidgetMock : public ISellDecision5ConfigWidget
{
    Q_OBJECT

public:
    explicit SellDecision5ConfigWidgetMock(QWidget* parent = nullptr) :
        ISellDecision5ConfigWidget(parent)
    {
    }
    ~SellDecision5ConfigWidgetMock() override = default;

    SellDecision5ConfigWidgetMock(const SellDecision5ConfigWidgetMock& another)            = delete;
    SellDecision5ConfigWidgetMock& operator=(const SellDecision5ConfigWidgetMock& another) = delete;

    MOCK_METHOD(void, updateUiFromConfig, (), (const, override));
    MOCK_METHOD(void, makeReadOnly, (), (const, override));
};
