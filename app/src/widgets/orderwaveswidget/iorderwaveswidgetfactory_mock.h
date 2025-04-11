#pragma once



#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory.h"

#include <gmock/gmock.h>



class OrderWavesWidgetFactoryMock : public IOrderWavesWidgetFactory
{
public:
    OrderWavesWidgetFactoryMock() :
        IOrderWavesWidgetFactory()
    {
    }
    ~OrderWavesWidgetFactoryMock() override = default;

    OrderWavesWidgetFactoryMock(const OrderWavesWidgetFactoryMock& another)            = delete;
    OrderWavesWidgetFactoryMock& operator=(const OrderWavesWidgetFactoryMock& another) = delete;

    MOCK_METHOD(IOrderWavesWidget*, newInstance, (int precision, float priceIncrement, QWidget* parent), (const, override));
};
