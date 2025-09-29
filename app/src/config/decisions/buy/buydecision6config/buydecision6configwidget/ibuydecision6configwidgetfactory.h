#pragma once



#include "src/config/decisions/buy/buydecision6config/buydecision6configwidget/ibuydecision6configwidget.h"



class IBuyDecision6ConfigWidgetFactory
{
public:
    IBuyDecision6ConfigWidgetFactory()          = default;
    virtual ~IBuyDecision6ConfigWidgetFactory() = default;

    IBuyDecision6ConfigWidgetFactory(const IBuyDecision6ConfigWidgetFactory& another)            = delete;
    IBuyDecision6ConfigWidgetFactory& operator=(const IBuyDecision6ConfigWidgetFactory& another) = delete;

    virtual IBuyDecision6ConfigWidget* newInstance(IBuyDecision6Config* config, QWidget* parent) const = 0;
};
