#pragma once



#include "src/config/decisions/sell/iselldecision2configwidget.h"



class ISellDecision2ConfigWidgetFactory
{
public:
    ISellDecision2ConfigWidgetFactory() {}
    virtual ~ISellDecision2ConfigWidgetFactory() = default;

    ISellDecision2ConfigWidgetFactory(const ISellDecision2ConfigWidgetFactory &another) = delete;
    ISellDecision2ConfigWidgetFactory& operator=(const ISellDecision2ConfigWidgetFactory &another) = delete;

    virtual ISellDecision2ConfigWidget* newInstance(QWidget *parent) = 0;
};
