#pragma once



#include "src/config/decisions/buy/ibuydecision2configwidget.h"



class IBuyDecision2ConfigWidgetFactory
{
public:
    IBuyDecision2ConfigWidgetFactory() {}
    virtual ~IBuyDecision2ConfigWidgetFactory() = default;

    IBuyDecision2ConfigWidgetFactory(const IBuyDecision2ConfigWidgetFactory &another) = delete;
    IBuyDecision2ConfigWidgetFactory& operator=(const IBuyDecision2ConfigWidgetFactory &another) = delete;

    virtual IBuyDecision2ConfigWidget* newInstance(QWidget *parent) = 0;
};
