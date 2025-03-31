#pragma once



#include "src/widgets/decisionmakerwidget/idecisionmakerwidget.h"



namespace Ui
{
class DecisionMakerWidget;
}



class DecisionMakerWidget : public IDecisionMakerWidget
{
    Q_OBJECT

public:
    explicit DecisionMakerWidget(QWidget* parent = nullptr);
    ~DecisionMakerWidget();

    DecisionMakerWidget(const DecisionMakerWidget& another)            = delete;
    DecisionMakerWidget& operator=(const DecisionMakerWidget& another) = delete;

    Ui::DecisionMakerWidget* ui;
};
