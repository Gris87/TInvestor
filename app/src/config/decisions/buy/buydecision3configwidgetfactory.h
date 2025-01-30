#pragma once



#include "src/config/decisions/buy/ibuydecision3configwidgetfactory.h"



class BuyDecision3ConfigWidgetFactory : public IBuyDecision3ConfigWidgetFactory
{
public:
    BuyDecision3ConfigWidgetFactory();
    ~BuyDecision3ConfigWidgetFactory();

    BuyDecision3ConfigWidgetFactory(const BuyDecision3ConfigWidgetFactory &another) = delete;
    BuyDecision3ConfigWidgetFactory& operator=(const BuyDecision3ConfigWidgetFactory &another) = delete;

    IBuyDecision3ConfigWidget* newInstance(QWidget *parent) override;
};
