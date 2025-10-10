#pragma once



#include "src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidgetfactory.h"



class DecisionMakerConfigWidgetFactory : public IDecisionMakerConfigWidgetFactory
{
public:
    DecisionMakerConfigWidgetFactory();
    ~DecisionMakerConfigWidgetFactory() override;

    DecisionMakerConfigWidgetFactory(const DecisionMakerConfigWidgetFactory& another)            = delete;
    DecisionMakerConfigWidgetFactory& operator=(const DecisionMakerConfigWidgetFactory& another) = delete;

    IDecisionMakerConfigWidget*
    newInstance(const DecisionMakerConfigWidgetFactoryNewInstanceArgsMore15& args, QWidget* parent) const override;
};
