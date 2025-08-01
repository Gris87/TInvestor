#pragma once



#include "src/config/decisions/buy/buydecision4config/buydecision4configwidget/ibuydecision4configwidget.h"



class IBuyDecision4ConfigWidgetFactory
{
public:
    IBuyDecision4ConfigWidgetFactory()          = default;
    virtual ~IBuyDecision4ConfigWidgetFactory() = default;

    IBuyDecision4ConfigWidgetFactory(const IBuyDecision4ConfigWidgetFactory& another)            = delete;
    IBuyDecision4ConfigWidgetFactory& operator=(const IBuyDecision4ConfigWidgetFactory& another) = delete;

    virtual IBuyDecision4ConfigWidget* newInstance(IBuyDecision4Config* config, QWidget* parent) const = 0;
};
