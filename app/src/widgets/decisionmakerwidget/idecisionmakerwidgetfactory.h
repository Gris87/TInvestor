#pragma once



#include "src/widgets/decisionmakerwidget/idecisionmakerwidget.h"



class IDecisionMakerWidgetFactory
{
public:
    IDecisionMakerWidgetFactory()
    {
    }
    virtual ~IDecisionMakerWidgetFactory() = default;

    IDecisionMakerWidgetFactory(const IDecisionMakerWidgetFactory& another)            = delete;
    IDecisionMakerWidgetFactory& operator=(const IDecisionMakerWidgetFactory& another) = delete;

    virtual IDecisionMakerWidget* newInstance(QWidget* parent) = 0;
};
