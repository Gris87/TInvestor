#pragma once



#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory.h"



class OrderWavesWidgetFactory : public IOrderWavesWidgetFactory
{
public:
    OrderWavesWidgetFactory();
    ~OrderWavesWidgetFactory() override;

    OrderWavesWidgetFactory(const OrderWavesWidgetFactory& another)            = delete;
    OrderWavesWidgetFactory& operator=(const OrderWavesWidgetFactory& another) = delete;

    IOrderWavesWidget* newInstance(qint8 precision, float priceIncrement, QWidget* parent) const override;
};
