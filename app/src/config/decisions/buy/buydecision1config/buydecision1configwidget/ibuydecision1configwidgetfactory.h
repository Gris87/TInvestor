#pragma once



#include "src/config/decisions/buy/ibuydecision1configwidget.h"
#include "src/config/decisions/buy/ibuydecision1config.h"



class IBuyDecision1ConfigWidgetFactory
{
public:
    IBuyDecision1ConfigWidgetFactory() {}
    virtual ~IBuyDecision1ConfigWidgetFactory() = default;

    IBuyDecision1ConfigWidgetFactory(const IBuyDecision1ConfigWidgetFactory &another) = delete;
    IBuyDecision1ConfigWidgetFactory& operator=(const IBuyDecision1ConfigWidgetFactory &another) = delete;

    virtual IBuyDecision1ConfigWidget* newInstance(IBuyDecision1Config *config, QWidget *parent) = 0;
};
