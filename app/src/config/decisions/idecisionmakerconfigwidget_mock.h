#pragma once



#include "src/config/decisions/idecisionmakerconfigwidget.h"

#include <gmock/gmock.h>



class DecisionMakerConfigWidgetMock : public IDecisionMakerConfigWidget
{
public:
    DecisionMakerConfigWidgetMock(QWidget *parent = nullptr) : IDecisionMakerConfigWidget(parent) {}
    ~DecisionMakerConfigWidgetMock() override = default;

    DecisionMakerConfigWidgetMock(const DecisionMakerConfigWidgetMock &another) = delete;
    DecisionMakerConfigWidgetMock& operator=(const DecisionMakerConfigWidgetMock &another) = delete;

    MOCK_METHOD0(updateUiFromConfig, void());
};
