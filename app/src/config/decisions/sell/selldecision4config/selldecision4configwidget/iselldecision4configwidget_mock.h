#pragma once



#include "src/config/decisions/sell/selldecision4config/selldecision4configwidget/iselldecision4configwidget.h"

#include <gmock/gmock.h>



class SellDecision4ConfigWidgetMock : public ISellDecision4ConfigWidget
{
    Q_OBJECT

public:
    explicit SellDecision4ConfigWidgetMock(QWidget* parent = nullptr) :
        ISellDecision4ConfigWidget(parent)
    {
    }
    ~SellDecision4ConfigWidgetMock() override = default;

    SellDecision4ConfigWidgetMock(const SellDecision4ConfigWidgetMock& another)            = delete;
    SellDecision4ConfigWidgetMock& operator=(const SellDecision4ConfigWidgetMock& another) = delete;

    MOCK_METHOD(void, updateUiFromConfig, (), (const, override));
    MOCK_METHOD(void, makeReadOnly, (), (const, override));
};
