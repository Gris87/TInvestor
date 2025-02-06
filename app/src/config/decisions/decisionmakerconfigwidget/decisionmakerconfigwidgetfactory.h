#pragma once



#include "src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidgetfactory.h"



class DecisionMakerConfigWidgetFactory : public IDecisionMakerConfigWidgetFactory
{
public:
    DecisionMakerConfigWidgetFactory();
    ~DecisionMakerConfigWidgetFactory();

    DecisionMakerConfigWidgetFactory(const DecisionMakerConfigWidgetFactory &another) = delete;
    DecisionMakerConfigWidgetFactory& operator=(const DecisionMakerConfigWidgetFactory &another) = delete;

    IDecisionMakerConfigWidget* newInstance(
        IDecisionMakerConfig *config,
        IBuyDecision1ConfigWidgetFactory *buyDecision1ConfigWidgetFactory,
        IBuyDecision2ConfigWidgetFactory *buyDecision2ConfigWidgetFactory,
        IBuyDecision3ConfigWidgetFactory *buyDecision3ConfigWidgetFactory,
        ISellDecision1ConfigWidgetFactory *sellDecision1ConfigWidgetFactory,
        ISellDecision2ConfigWidgetFactory *sellDecision2ConfigWidgetFactory,
        ISellDecision3ConfigWidgetFactory *sellDecision3ConfigWidgetFactory,
        QWidget *parent
    ) override;
};
