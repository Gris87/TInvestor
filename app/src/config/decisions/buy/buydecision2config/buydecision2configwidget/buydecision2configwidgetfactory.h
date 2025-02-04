#pragma once



#include "src/config/decisions/buy/buydecision2config/buydecision2configwidget/ibuydecision2configwidgetfactory.h"



class BuyDecision2ConfigWidgetFactory : public IBuyDecision2ConfigWidgetFactory
{
public:
    BuyDecision2ConfigWidgetFactory();
    ~BuyDecision2ConfigWidgetFactory();

    BuyDecision2ConfigWidgetFactory(const BuyDecision2ConfigWidgetFactory &another) = delete;
    BuyDecision2ConfigWidgetFactory& operator=(const BuyDecision2ConfigWidgetFactory &another) = delete;

    IBuyDecision2ConfigWidget* newInstance(IBuyDecision2Config *config, QWidget *parent) override;
};
