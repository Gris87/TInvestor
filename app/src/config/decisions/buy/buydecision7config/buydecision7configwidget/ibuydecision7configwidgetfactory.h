#pragma once



#include "src/config/decisions/buy/buydecision7config/buydecision7configwidget/ibuydecision7configwidget.h"



class IBuyDecision7ConfigWidgetFactory
{
public:
    IBuyDecision7ConfigWidgetFactory()          = default;
    virtual ~IBuyDecision7ConfigWidgetFactory() = default;

    IBuyDecision7ConfigWidgetFactory(const IBuyDecision7ConfigWidgetFactory& another)            = delete;
    IBuyDecision7ConfigWidgetFactory& operator=(const IBuyDecision7ConfigWidgetFactory& another) = delete;

    virtual IBuyDecision7ConfigWidget* newInstance(IBuyDecision7Config* config, QWidget* parent) const = 0;
};
