#pragma once



#include "src/config/decisions/sell/iselldecision3configwidget.h"



class ISellDecision3ConfigWidgetFactory
{
public:
    ISellDecision3ConfigWidgetFactory() {}
    virtual ~ISellDecision3ConfigWidgetFactory() = default;

    ISellDecision3ConfigWidgetFactory(const ISellDecision3ConfigWidgetFactory &another) = delete;
    ISellDecision3ConfigWidgetFactory& operator=(const ISellDecision3ConfigWidgetFactory &another) = delete;

    virtual ISellDecision3ConfigWidget* newInstance(QWidget *parent) = 0;
};
