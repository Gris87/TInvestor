#pragma once



#include "src/widgets/decisionmakerwidget/idecisionmakerwidgetfactory.h"



class DecisionMakerWidgetFactory : public IDecisionMakerWidgetFactory
{
public:
    DecisionMakerWidgetFactory();
    ~DecisionMakerWidgetFactory();

    DecisionMakerWidgetFactory(const DecisionMakerWidgetFactory& another)            = delete;
    DecisionMakerWidgetFactory& operator=(const DecisionMakerWidgetFactory& another) = delete;

    IDecisionMakerWidget* newInstance(QWidget* parent) override;
};
