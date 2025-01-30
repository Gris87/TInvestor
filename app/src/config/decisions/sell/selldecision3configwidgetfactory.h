#pragma once



#include "src/config/decisions/sell/iselldecision3configwidgetfactory.h"



class SellDecision3ConfigWidgetFactory : public ISellDecision3ConfigWidgetFactory
{
public:
    SellDecision3ConfigWidgetFactory();
    ~SellDecision3ConfigWidgetFactory();

    SellDecision3ConfigWidgetFactory(const SellDecision3ConfigWidgetFactory &another) = delete;
    SellDecision3ConfigWidgetFactory& operator=(const SellDecision3ConfigWidgetFactory &another) = delete;

    ISellDecision3ConfigWidget* newInstance(QWidget *parent) override;
};
