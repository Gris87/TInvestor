#pragma once



#include "src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidgetfactory.h"

#include <gmock/gmock.h>



class DecisionMakerConfigWidgetFactoryMock : public IDecisionMakerConfigWidgetFactory
{
public:
    DecisionMakerConfigWidgetFactoryMock() :
        IDecisionMakerConfigWidgetFactory()
    {
    }
    ~DecisionMakerConfigWidgetFactoryMock() override = default;

    DecisionMakerConfigWidgetFactoryMock(const DecisionMakerConfigWidgetFactoryMock& another)            = delete;
    DecisionMakerConfigWidgetFactoryMock& operator=(const DecisionMakerConfigWidgetFactoryMock& another) = delete;

    MOCK_METHOD(
        IDecisionMakerConfigWidget*,
        newInstance,
        (const DecisionMakerConfigWidgetFactoryNewInstanceArgsMore15& args, QWidget* parent),
        (const, override)
    );
};
