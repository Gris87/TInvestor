#pragma once



#include "src/widgets/orderwaveswidget/iorderwaveswidget.h"



class IOrderWavesWidgetFactory
{
public:
    IOrderWavesWidgetFactory()          = default;
    virtual ~IOrderWavesWidgetFactory() = default;

    IOrderWavesWidgetFactory(const IOrderWavesWidgetFactory& another)            = delete;
    IOrderWavesWidgetFactory& operator=(const IOrderWavesWidgetFactory& another) = delete;

    virtual IOrderWavesWidget* newInstance(int precision, float priceIncrement, QWidget* parent) const = 0;
};
