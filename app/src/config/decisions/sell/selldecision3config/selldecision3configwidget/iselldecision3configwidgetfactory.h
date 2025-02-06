#pragma once



#include "src/config/decisions/sell/selldecision3config/selldecision3configwidget/iselldecision3configwidget.h"
#include "src/config/decisions/sell/selldecision3config/iselldecision3config.h"



class ISellDecision3ConfigWidgetFactory
{
public:
    ISellDecision3ConfigWidgetFactory() {}
    virtual ~ISellDecision3ConfigWidgetFactory() = default;

    ISellDecision3ConfigWidgetFactory(const ISellDecision3ConfigWidgetFactory &another) = delete;
    ISellDecision3ConfigWidgetFactory& operator=(const ISellDecision3ConfigWidgetFactory &another) = delete;

    virtual ISellDecision3ConfigWidget* newInstance(ISellDecision3Config *config, QWidget *parent) = 0;
};
