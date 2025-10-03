#pragma once



#include "src/config/decisions/buy/buydecision8config/buydecision8configwidget/ibuydecision8configwidget.h"



class IBuyDecision8ConfigWidgetFactory
{
public:
    IBuyDecision8ConfigWidgetFactory()          = default;
    virtual ~IBuyDecision8ConfigWidgetFactory() = default;

    IBuyDecision8ConfigWidgetFactory(const IBuyDecision8ConfigWidgetFactory& another)            = delete;
    IBuyDecision8ConfigWidgetFactory& operator=(const IBuyDecision8ConfigWidgetFactory& another) = delete;

    virtual IBuyDecision8ConfigWidget* newInstance(IBuyDecision8Config* config, QWidget* parent) const = 0;
};
