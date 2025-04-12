#pragma once



#include "src/config/decisions/sell/selldecision2config/selldecision2configwidget/iselldecision2configwidgetfactory.h"



class SellDecision2ConfigWidgetFactory : public ISellDecision2ConfigWidgetFactory
{
public:
    SellDecision2ConfigWidgetFactory();
    ~SellDecision2ConfigWidgetFactory() override;

    SellDecision2ConfigWidgetFactory(const SellDecision2ConfigWidgetFactory& another)            = delete;
    SellDecision2ConfigWidgetFactory& operator=(const SellDecision2ConfigWidgetFactory& another) = delete;

    ISellDecision2ConfigWidget* newInstance(ISellDecision2Config* config, QWidget* parent) const override;
};
