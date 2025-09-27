#pragma once



#include "src/config/decisions/buy/buydecision5config/buydecision5configwidget/ibuydecision5configwidget.h"



class IBuyDecision5ConfigWidgetFactory
{
public:
    IBuyDecision5ConfigWidgetFactory()          = default;
    virtual ~IBuyDecision5ConfigWidgetFactory() = default;

    IBuyDecision5ConfigWidgetFactory(const IBuyDecision5ConfigWidgetFactory& another)            = delete;
    IBuyDecision5ConfigWidgetFactory& operator=(const IBuyDecision5ConfigWidgetFactory& another) = delete;

    virtual IBuyDecision5ConfigWidget* newInstance(IBuyDecision5Config* config, QWidget* parent) const = 0;
};
