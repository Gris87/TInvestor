#pragma once



#include "src/config/decisions/sell/iselldecision2configwidgetfactory.h"



class SellDecision2ConfigWidgetFactory : public ISellDecision2ConfigWidgetFactory
{
public:
    SellDecision2ConfigWidgetFactory();
    ~SellDecision2ConfigWidgetFactory();

    SellDecision2ConfigWidgetFactory(const SellDecision2ConfigWidgetFactory &another) = delete;
    SellDecision2ConfigWidgetFactory& operator=(const SellDecision2ConfigWidgetFactory &another) = delete;

    ISellDecision2ConfigWidget* newInstance(ISellDecision2Config *config, QWidget *parent) override;
};
