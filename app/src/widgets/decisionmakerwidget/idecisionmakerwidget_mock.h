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

    MOCK_METHOD(void, setAccountName, (const QString& name), (override));

    MOCK_METHOD(void, showSpinners, (), (override));
    MOCK_METHOD(void, operationsRead, (const QList<Operation>& operations), (override));
    MOCK_METHOD(void, operationsAdded, (const QList<Operation>& operations), (override));
    MOCK_METHOD(void, logsRead, (const QList<LogEntry>& entries), (override));
    MOCK_METHOD(void, logAdded, (const LogEntry& entry), (override));
    MOCK_METHOD(void, portfolioChanged, (const Portfolio& portfolio), (override));
    MOCK_METHOD(void, lastPriceChanged, (const QString& instrumentId, float price), (override));
    MOCK_METHOD(void, updateLastPrices, (), (override));

    MOCK_METHOD(void, saveWindowState, (const QString& type), (override));
    MOCK_METHOD(void, loadWindowState, (const QString& type), (override));
};
