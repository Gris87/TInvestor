#pragma once



#include "src/config/decisions/buy/buydecision4config/buydecision4configwidget/ibuydecision4configwidgetfactory.h"



class BuyDecision4ConfigWidgetFactory : public IBuyDecision4ConfigWidgetFactory
{
public:
    BuyDecision4ConfigWidgetFactory();
    ~BuyDecision4ConfigWidgetFactory() override;

    BuyDecision4ConfigWidgetFactory(const BuyDecision4ConfigWidgetFactory& another)            = delete;
    BuyDecision4ConfigWidgetFactory& operator=(const BuyDecision4ConfigWidgetFactory& another) = delete;

    IBuyDecision4ConfigWidget* newInstance(IBuyDecision4Config* config, QWidget* parent) const override;
};
