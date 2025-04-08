#pragma once



#include "src/config/decisions/buy/buydecision2config/buydecision2configwidget/ibuydecision2configwidget.h"



class IBuyDecision2ConfigWidgetFactory
{
public:
    IBuyDecision2ConfigWidgetFactory()          = default;
    virtual ~IBuyDecision2ConfigWidgetFactory() = default;

    IBuyDecision2ConfigWidgetFactory(const IBuyDecision2ConfigWidgetFactory& another)            = delete;
    IBuyDecision2ConfigWidgetFactory& operator=(const IBuyDecision2ConfigWidgetFactory& another) = delete;

    virtual IBuyDecision2ConfigWidget* newInstance(IBuyDecision2Config* config, QWidget* parent) const = 0;
};
