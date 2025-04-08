#pragma once



#include "src/config/decisions/sell/selldecision1config/selldecision1configwidget/iselldecision1configwidget.h"



class ISellDecision1ConfigWidgetFactory
{
public:
    ISellDecision1ConfigWidgetFactory()          = default;
    virtual ~ISellDecision1ConfigWidgetFactory() = default;

    ISellDecision1ConfigWidgetFactory(const ISellDecision1ConfigWidgetFactory& another)            = delete;
    ISellDecision1ConfigWidgetFactory& operator=(const ISellDecision1ConfigWidgetFactory& another) = delete;

    virtual ISellDecision1ConfigWidget* newInstance(ISellDecision1Config* config, QWidget* parent) const = 0;
};
