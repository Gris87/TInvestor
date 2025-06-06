#pragma once



#include "src/config/decisions/buy/buydecision1config/buydecision1configwidget/ibuydecision1configwidget.h"



class IBuyDecision1ConfigWidgetFactory
{
public:
    IBuyDecision1ConfigWidgetFactory()          = default;
    virtual ~IBuyDecision1ConfigWidgetFactory() = default;

    IBuyDecision1ConfigWidgetFactory(const IBuyDecision1ConfigWidgetFactory& another)            = delete;
    IBuyDecision1ConfigWidgetFactory& operator=(const IBuyDecision1ConfigWidgetFactory& another) = delete;

    virtual IBuyDecision1ConfigWidget* newInstance(IBuyDecision1Config* config, QWidget* parent) const = 0;
};
