#pragma once



#include "src/config/decisions/sell/selldecision3config/selldecision3configwidget/iselldecision3configwidget.h"



class ISellDecision3ConfigWidgetFactory
{
public:
    ISellDecision3ConfigWidgetFactory()          = default;
    virtual ~ISellDecision3ConfigWidgetFactory() = default;

    ISellDecision3ConfigWidgetFactory(const ISellDecision3ConfigWidgetFactory& another)            = delete;
    ISellDecision3ConfigWidgetFactory& operator=(const ISellDecision3ConfigWidgetFactory& another) = delete;

    virtual ISellDecision3ConfigWidget* newInstance(ISellDecision3Config* config, QWidget* parent) const = 0;
};
