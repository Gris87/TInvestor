#pragma once



#include "src/config/decisions/buy/buydecision9config/buydecision9configwidget/ibuydecision9configwidget.h"



class IBuyDecision9ConfigWidgetFactory
{
public:
    IBuyDecision9ConfigWidgetFactory()          = default;
    virtual ~IBuyDecision9ConfigWidgetFactory() = default;

    IBuyDecision9ConfigWidgetFactory(const IBuyDecision9ConfigWidgetFactory& another)            = delete;
    IBuyDecision9ConfigWidgetFactory& operator=(const IBuyDecision9ConfigWidgetFactory& another) = delete;

    virtual IBuyDecision9ConfigWidget* newInstance(IBuyDecision9Config* config, QWidget* parent) const = 0;
};
