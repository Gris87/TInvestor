#pragma once



#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory.h"



class OrderWavesWidgetFactory : public IOrderWavesWidgetFactory
{
public:
    OrderWavesWidgetFactory();
    ~OrderWavesWidgetFactory();

    OrderWavesWidgetFactory(const OrderWavesWidgetFactory& another)            = delete;
    OrderWavesWidgetFactory& operator=(const OrderWavesWidgetFactory& another) = delete;

    IOrderWavesWidget* newInstance(int precision, float priceIncrement, QWidget* parent) override;
};
