#pragma once



#include "src/config/decisions/buy/buydecision5config/buydecision5configwidget/ibuydecision5configwidgetfactory.h"



class BuyDecision5ConfigWidgetFactory : public IBuyDecision5ConfigWidgetFactory
{
public:
    BuyDecision5ConfigWidgetFactory();
    ~BuyDecision5ConfigWidgetFactory() override;

    BuyDecision5ConfigWidgetFactory(const BuyDecision5ConfigWidgetFactory& another)            = delete;
    BuyDecision5ConfigWidgetFactory& operator=(const BuyDecision5ConfigWidgetFactory& another) = delete;

    IBuyDecision5ConfigWidget* newInstance(IBuyDecision5Config* config, QWidget* parent) const override;
};
