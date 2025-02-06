#pragma once



#include "src/config/decisions/buy/buydecision1config/buydecision1configwidget/ibuydecision1configwidgetfactory.h"



class BuyDecision1ConfigWidgetFactory : public IBuyDecision1ConfigWidgetFactory
{
public:
    BuyDecision1ConfigWidgetFactory();
    ~BuyDecision1ConfigWidgetFactory();

    BuyDecision1ConfigWidgetFactory(const BuyDecision1ConfigWidgetFactory &another) = delete;
    BuyDecision1ConfigWidgetFactory& operator=(const BuyDecision1ConfigWidgetFactory &another) = delete;

    IBuyDecision1ConfigWidget* newInstance(IBuyDecision1Config *config, QWidget *parent) override;
};
