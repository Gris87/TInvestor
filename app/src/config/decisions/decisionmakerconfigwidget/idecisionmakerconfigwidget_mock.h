#pragma once



#include "src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidget.h"

#include <gmock/gmock.h>



class DecisionMakerConfigWidgetMock : public IDecisionMakerConfigWidget
{
public:
    explicit DecisionMakerConfigWidgetMock(QWidget *parent = nullptr) : IDecisionMakerConfigWidget(parent) {}
    ~DecisionMakerConfigWidgetMock() override = default;

    DecisionMakerConfigWidgetMock(const DecisionMakerConfigWidgetMock &another) = delete;
    DecisionMakerConfigWidgetMock& operator=(const DecisionMakerConfigWidgetMock &another) = delete;

    MOCK_METHOD0(updateUiFromConfig, void());
};
