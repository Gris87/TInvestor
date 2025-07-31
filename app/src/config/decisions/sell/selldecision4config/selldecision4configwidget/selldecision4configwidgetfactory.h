#pragma once



#include "src/config/decisions/sell/selldecision4config/selldecision4configwidget/iselldecision4configwidgetfactory.h"



class SellDecision4ConfigWidgetFactory : public ISellDecision4ConfigWidgetFactory
{
public:
    SellDecision4ConfigWidgetFactory();
    ~SellDecision4ConfigWidgetFactory() override;

    SellDecision4ConfigWidgetFactory(const SellDecision4ConfigWidgetFactory& another)            = delete;
    SellDecision4ConfigWidgetFactory& operator=(const SellDecision4ConfigWidgetFactory& another) = delete;

    ISellDecision4ConfigWidget* newInstance(ISellDecision4Config* config, QWidget* parent) const override;
};
