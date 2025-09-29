#pragma once



#include "src/config/decisions/buy/buydecision6config/buydecision6configwidget/ibuydecision6configwidgetfactory.h"



class BuyDecision6ConfigWidgetFactory : public IBuyDecision6ConfigWidgetFactory
{
public:
    BuyDecision6ConfigWidgetFactory();
    ~BuyDecision6ConfigWidgetFactory() override;

    BuyDecision6ConfigWidgetFactory(const BuyDecision6ConfigWidgetFactory& another)            = delete;
    BuyDecision6ConfigWidgetFactory& operator=(const BuyDecision6ConfigWidgetFactory& another) = delete;

    IBuyDecision6ConfigWidget* newInstance(IBuyDecision6Config* config, QWidget* parent) const override;
};
