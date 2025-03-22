#pragma once



#include "src/widgets/orderwaveswidget/iorderwaveswidget.h"

#include <gmock/gmock.h>



class OrderWavesWidgetMock : public IOrderWavesWidget
{
public:
    explicit OrderWavesWidgetMock(QWidget* parent = nullptr) :
        IOrderWavesWidget(parent)
    {
    }
    ~OrderWavesWidgetMock() override = default;

    OrderWavesWidgetMock(const OrderWavesWidgetMock& another)            = delete;
    OrderWavesWidgetMock& operator=(const OrderWavesWidgetMock& another) = delete;

    MOCK_METHOD(void, orderBookChanged, (const OrderBook& orderBook), (override));
    MOCK_METHOD(void, reset, (), (override));
};
