#pragma once



#include "src/config/decisions/buy/ibuydecision1configwidget.h"



class IBuyDecision1ConfigWidgetFactory
{
public:
    IBuyDecision1ConfigWidgetFactory() {}
    virtual ~IBuyDecision1ConfigWidgetFactory() = default;

    IBuyDecision1ConfigWidgetFactory(const IBuyDecision1ConfigWidgetFactory &another) = delete;
    IBuyDecision1ConfigWidgetFactory& operator=(const IBuyDecision1ConfigWidgetFactory &another) = delete;

    virtual IBuyDecision1ConfigWidget* newInstance(QWidget *parent) = 0;
};
