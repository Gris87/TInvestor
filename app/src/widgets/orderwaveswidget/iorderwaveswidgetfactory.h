#pragma once



#include "src/widgets/orderwaveswidget/iorderwaveswidget.h"



class IOrderWavesWidgetFactory
{
public:
    IOrderWavesWidgetFactory()
    {
    }
    virtual ~IOrderWavesWidgetFactory() = default;

    IOrderWavesWidgetFactory(const IOrderWavesWidgetFactory& another)            = delete;
    IOrderWavesWidgetFactory& operator=(const IOrderWavesWidgetFactory& another) = delete;

    virtual IOrderWavesWidget* newInstance(float priceIncrement, QWidget* parent) = 0;
};
