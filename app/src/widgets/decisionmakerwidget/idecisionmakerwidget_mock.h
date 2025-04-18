#pragma once



#include "src/widgets/decisionmakerwidget/idecisionmakerwidget.h"

#include <gmock/gmock.h>



class DecisionMakerWidgetMock : public IDecisionMakerWidget
{
    Q_OBJECT

public:
    explicit DecisionMakerWidgetMock(QWidget* parent = nullptr) :
        IDecisionMakerWidget(parent)
    {
    }
    ~DecisionMakerWidgetMock() override = default;

    DecisionMakerWidgetMock(const DecisionMakerWidgetMock& another)            = delete;
    DecisionMakerWidgetMock& operator=(const DecisionMakerWidgetMock& another) = delete;

    MOCK_METHOD(void, saveWindowState, (const QString& type), (override));
    MOCK_METHOD(void, loadWindowState, (const QString& type), (override));
};
