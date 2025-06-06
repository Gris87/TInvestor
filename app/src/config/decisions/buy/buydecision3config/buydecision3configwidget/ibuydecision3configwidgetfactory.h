#pragma once



#include "src/config/decisions/buy/buydecision3config/buydecision3configwidget/ibuydecision3configwidget.h"



class IBuyDecision3ConfigWidgetFactory
{
public:
    IBuyDecision3ConfigWidgetFactory()          = default;
    virtual ~IBuyDecision3ConfigWidgetFactory() = default;

    IBuyDecision3ConfigWidgetFactory(const IBuyDecision3ConfigWidgetFactory& another)            = delete;
    IBuyDecision3ConfigWidgetFactory& operator=(const IBuyDecision3ConfigWidgetFactory& another) = delete;

    virtual IBuyDecision3ConfigWidget* newInstance(IBuyDecision3Config* config, QWidget* parent) const = 0;
};
