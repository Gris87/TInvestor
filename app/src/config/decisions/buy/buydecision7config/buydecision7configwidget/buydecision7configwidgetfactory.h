#pragma once



#include "src/config/decisions/buy/buydecision7config/buydecision7configwidget/ibuydecision7configwidgetfactory.h"



class BuyDecision7ConfigWidgetFactory : public IBuyDecision7ConfigWidgetFactory
{
public:
    BuyDecision7ConfigWidgetFactory();
    ~BuyDecision7ConfigWidgetFactory() override;

    BuyDecision7ConfigWidgetFactory(const BuyDecision7ConfigWidgetFactory& another)            = delete;
    BuyDecision7ConfigWidgetFactory& operator=(const BuyDecision7ConfigWidgetFactory& another) = delete;

    IBuyDecision7ConfigWidget* newInstance(IBuyDecision7Config* config, QWidget* parent) const override;
};
