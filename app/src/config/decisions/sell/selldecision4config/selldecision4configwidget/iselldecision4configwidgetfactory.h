#pragma once



#include "src/config/decisions/sell/selldecision4config/selldecision4configwidget/iselldecision4configwidget.h"



class ISellDecision4ConfigWidgetFactory
{
public:
    ISellDecision4ConfigWidgetFactory()          = default;
    virtual ~ISellDecision4ConfigWidgetFactory() = default;

    ISellDecision4ConfigWidgetFactory(const ISellDecision4ConfigWidgetFactory& another)            = delete;
    ISellDecision4ConfigWidgetFactory& operator=(const ISellDecision4ConfigWidgetFactory& another) = delete;

    virtual ISellDecision4ConfigWidget* newInstance(ISellDecision4Config* config, QWidget* parent) const = 0;
};
