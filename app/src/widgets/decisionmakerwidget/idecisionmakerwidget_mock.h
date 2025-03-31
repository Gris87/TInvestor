#pragma once



#include "src/widgets/decisionmakerwidget/idecisionmakerwidget.h"

#include <gmock/gmock.h>



class DecisionMakerWidgetMock : public IDecisionMakerWidget
{
public:
    explicit DecisionMakerWidgetMock(QWidget* parent = nullptr) :
        IDecisionMakerWidget(parent)
    {
    }
    ~DecisionMakerWidgetMock() override = default;

    DecisionMakerWidgetMock(const DecisionMakerWidgetMock& another)            = delete;
    DecisionMakerWidgetMock& operator=(const DecisionMakerWidgetMock& another) = delete;
};
