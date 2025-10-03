#pragma once



#include "src/config/decisions/sell/selldecision5config/selldecision5configwidget/iselldecision5configwidgetfactory.h"



class SellDecision5ConfigWidgetFactory : public ISellDecision5ConfigWidgetFactory
{
public:
    SellDecision5ConfigWidgetFactory();
    ~SellDecision5ConfigWidgetFactory() override;

    SellDecision5ConfigWidgetFactory(const SellDecision5ConfigWidgetFactory& another)            = delete;
    SellDecision5ConfigWidgetFactory& operator=(const SellDecision5ConfigWidgetFactory& another) = delete;

    ISellDecision5ConfigWidget* newInstance(ISellDecision5Config* config, QWidget* parent) const override;
};
