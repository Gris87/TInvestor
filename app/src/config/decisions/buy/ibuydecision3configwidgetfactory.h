#pragma once



#include "src/config/decisions/buy/ibuydecision3configwidget.h"



class IBuyDecision3ConfigWidgetFactory
{
public:
    IBuyDecision3ConfigWidgetFactory() {}
    virtual ~IBuyDecision3ConfigWidgetFactory() = default;

    IBuyDecision3ConfigWidgetFactory(const IBuyDecision3ConfigWidgetFactory &another) = delete;
    IBuyDecision3ConfigWidgetFactory& operator=(const IBuyDecision3ConfigWidgetFactory &another) = delete;

    virtual IBuyDecision3ConfigWidget* newInstance(QWidget *parent) = 0;
};
