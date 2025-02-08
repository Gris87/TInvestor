#pragma once



#include "src/config/decisions/buy/buydecision3config/buydecision3configwidget/ibuydecision3configwidgetfactory.h"



class BuyDecision3ConfigWidgetFactory : public IBuyDecision3ConfigWidgetFactory
{
public:
    BuyDecision3ConfigWidgetFactory();
    ~BuyDecision3ConfigWidgetFactory();

    BuyDecision3ConfigWidgetFactory(const BuyDecision3ConfigWidgetFactory& another)            = delete;
    BuyDecision3ConfigWidgetFactory& operator=(const BuyDecision3ConfigWidgetFactory& another) = delete;

    IBuyDecision3ConfigWidget* newInstance(IBuyDecision3Config* config, QWidget* parent) override;
};
