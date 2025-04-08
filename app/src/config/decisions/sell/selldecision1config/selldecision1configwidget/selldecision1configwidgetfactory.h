#pragma once



#include "src/config/decisions/sell/selldecision1config/selldecision1configwidget/iselldecision1configwidgetfactory.h"



class SellDecision1ConfigWidgetFactory : public ISellDecision1ConfigWidgetFactory
{
public:
    SellDecision1ConfigWidgetFactory();
    ~SellDecision1ConfigWidgetFactory();

    SellDecision1ConfigWidgetFactory(const SellDecision1ConfigWidgetFactory& another)            = delete;
    SellDecision1ConfigWidgetFactory& operator=(const SellDecision1ConfigWidgetFactory& another) = delete;

    ISellDecision1ConfigWidget* newInstance(ISellDecision1Config* config, QWidget* parent) const override;
};
