#pragma once



#include "src/config/decisions/sell/selldecision2config/selldecision2configwidget/iselldecision2configwidget.h"



class ISellDecision2ConfigWidgetFactory
{
public:
    ISellDecision2ConfigWidgetFactory()          = default;
    virtual ~ISellDecision2ConfigWidgetFactory() = default;

    ISellDecision2ConfigWidgetFactory(const ISellDecision2ConfigWidgetFactory& another)            = delete;
    ISellDecision2ConfigWidgetFactory& operator=(const ISellDecision2ConfigWidgetFactory& another) = delete;

    virtual ISellDecision2ConfigWidget* newInstance(ISellDecision2Config* config, QWidget* parent) const = 0;
};
