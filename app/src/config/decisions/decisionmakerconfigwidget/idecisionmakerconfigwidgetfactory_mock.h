#pragma once



#include "src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidgetfactory.h"

#include <gmock/gmock.h>



class DecisionMakerConfigWidgetFactoryMock : public IDecisionMakerConfigWidgetFactory
{
public:
    DecisionMakerConfigWidgetFactoryMock() : IDecisionMakerConfigWidgetFactory() {}
    ~DecisionMakerConfigWidgetFactoryMock() override = default;

    DecisionMakerConfigWidgetFactoryMock(const DecisionMakerConfigWidgetFactoryMock &another) = delete;
    DecisionMakerConfigWidgetFactoryMock& operator=(const DecisionMakerConfigWidgetFactoryMock &another) = delete;

    MOCK_METHOD8(newInstance, IDecisionMakerConfigWidget*(
        IDecisionMakerConfig *config,
        IBuyDecision1ConfigWidgetFactory *buyDecision1ConfigWidgetFactory,
        IBuyDecision2ConfigWidgetFactory *buyDecision2ConfigWidgetFactory,
        IBuyDecision3ConfigWidgetFactory *buyDecision3ConfigWidgetFactory,
        ISellDecision1ConfigWidgetFactory *sellDecision1ConfigWidgetFactory,
        ISellDecision2ConfigWidgetFactory *sellDecision2ConfigWidgetFactory,
        ISellDecision3ConfigWidgetFactory *sellDecision3ConfigWidgetFactory,
        QWidget *parent
    ));
};
