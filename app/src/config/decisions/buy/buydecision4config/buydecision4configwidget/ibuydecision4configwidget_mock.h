#pragma once



#include "src/config/decisions/buy/buydecision4config/buydecision4configwidget/ibuydecision4configwidget.h"

#include <gmock/gmock.h>



class BuyDecision4ConfigWidgetMock : public IBuyDecision4ConfigWidget
{
    Q_OBJECT

public:
    explicit BuyDecision4ConfigWidgetMock(QWidget* parent = nullptr) :
        IBuyDecision4ConfigWidget(parent)
    {
    }
    ~BuyDecision4ConfigWidgetMock() override = default;

    BuyDecision4ConfigWidgetMock(const BuyDecision4ConfigWidgetMock& another)            = delete;
    BuyDecision4ConfigWidgetMock& operator=(const BuyDecision4ConfigWidgetMock& another) = delete;

    MOCK_METHOD(void, updateUiFromConfig, (), (const, override));
    MOCK_METHOD(void, makeReadOnly, (), (const, override));
};
