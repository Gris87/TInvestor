#pragma once



#include "src/config/decisions/sell/selldecision5config/selldecision5configwidget/iselldecision5configwidget.h"



class ISellDecision5ConfigWidgetFactory
{
public:
    ISellDecision5ConfigWidgetFactory()          = default;
    virtual ~ISellDecision5ConfigWidgetFactory() = default;

    ISellDecision5ConfigWidgetFactory(const ISellDecision5ConfigWidgetFactory& another)            = delete;
    ISellDecision5ConfigWidgetFactory& operator=(const ISellDecision5ConfigWidgetFactory& another) = delete;

    virtual ISellDecision5ConfigWidget* newInstance(ISellDecision5Config* config, QWidget* parent) const = 0;
};
