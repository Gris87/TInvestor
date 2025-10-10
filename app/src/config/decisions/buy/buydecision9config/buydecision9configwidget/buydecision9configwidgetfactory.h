#pragma once



#include "src/config/decisions/buy/buydecision9config/buydecision9configwidget/ibuydecision9configwidgetfactory.h"



class BuyDecision9ConfigWidgetFactory : public IBuyDecision9ConfigWidgetFactory
{
public:
    BuyDecision9ConfigWidgetFactory();
    ~BuyDecision9ConfigWidgetFactory() override;

    BuyDecision9ConfigWidgetFactory(const BuyDecision9ConfigWidgetFactory& another)            = delete;
    BuyDecision9ConfigWidgetFactory& operator=(const BuyDecision9ConfigWidgetFactory& another) = delete;

    IBuyDecision9ConfigWidget* newInstance(IBuyDecision9Config* config, QWidget* parent) const override;
};
