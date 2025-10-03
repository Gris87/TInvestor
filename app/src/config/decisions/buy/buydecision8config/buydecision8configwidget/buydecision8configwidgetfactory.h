#pragma once



#include "src/config/decisions/buy/buydecision8config/buydecision8configwidget/ibuydecision8configwidgetfactory.h"



class BuyDecision8ConfigWidgetFactory : public IBuyDecision8ConfigWidgetFactory
{
public:
    BuyDecision8ConfigWidgetFactory();
    ~BuyDecision8ConfigWidgetFactory() override;

    BuyDecision8ConfigWidgetFactory(const BuyDecision8ConfigWidgetFactory& another)            = delete;
    BuyDecision8ConfigWidgetFactory& operator=(const BuyDecision8ConfigWidgetFactory& another) = delete;

    IBuyDecision8ConfigWidget* newInstance(IBuyDecision8Config* config, QWidget* parent) const override;
};
